#include "TCPConnection.hpp"
#include <iostream>

namespace FindIt
{

Server::Server(unsigned short port)
{
    m_running = false;

    if (WSADATA wsData; WSAStartup(MAKEWORD(2, 2), &wsData) != NO_ERROR)
        {throw FailedWSAStartup("Failed to initialize Winsock");}
	
	m_listener = socket(AF_INET, SOCK_STREAM, 0);
	if (m_listener == INVALID_SOCKET)
        {throw FailedSocketInit("Failed to initialize listening socket");}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	
	bind(m_listener, (sockaddr*)&hint, sizeof(hint));
	listen(m_listener, SOMAXCONN);

	FD_ZERO(&m_master);
	FD_SET(m_listener, &m_master);
}

Server::~Server()
{
    FD_CLR(m_listener, &m_master);
    closesocket(m_listener);
    while (m_master.fd_count > 0)
    {
        SOCKET sock = m_master.fd_array[0];
        FD_CLR(sock, &m_master);
        closesocket(sock);
    }
    WSACleanup();
}

void Server::run()
{
    m_running = true;
    while (m_running)
    {
        fd_set copy = m_master;
        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = RECV_TIMEOUT;
        int socketCount = select(0, &copy, nullptr, nullptr, &timeout);
        for (int i = 0; i < socketCount; i++)
        {
            SOCKET sock = copy.fd_array[i];
            if (sock == m_listener)
            {
                SOCKET client = accept(m_listener, nullptr, nullptr);
                FD_SET(client, &m_master);
                DWORD timeout = RECV_TIMEOUT;
                setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
                if (m_on_client_connect != nullptr)
                    m_on_client_connect(client);
            }
            else
            {
                char buf[MAX_BUFFER_SIZE];
                ZeroMemory(buf, MAX_BUFFER_SIZE);
                int bytesIn = recv(sock, buf, MAX_BUFFER_SIZE, 0);

                std::scoped_lock<std::mutex> close_clients_lock(m_clients_to_close_mutex);
                if (bytesIn <= 0
                || std::find(m_clients_to_close.begin(), m_clients_to_close.end(), (uint64_t)sock) != m_clients_to_close.end()
                || (uint64_t)sock == m_clients_to_close.back())
                {
                    closesocket(sock);
                    FD_CLR(sock, &m_master);
                    if (m_on_client_disconnect != nullptr)
                        m_on_client_disconnect(sock);
                }
                else
                {
                    std::string inboundMsg = std::string(buf, bytesIn);
                    if (m_on_client_message != nullptr)
                        m_on_client_message(sock, inboundMsg);
                }
            }
        }

        for (u_int i = 0; i < m_master.fd_count; i++)
        {
            SOCKET sock = m_master.fd_array[i];
            if (sock == m_listener)
                continue;

            {
                std::scoped_lock<std::mutex> broadcast_lock(m_broadcast_messages_out_mutex);
                if (!m_broadcast_messages_out.empty())
                {
                    std::string message = m_broadcast_messages_out.front();
                    // std::cout << "Broadcasting: " << message << "\nTo: " << sock << std::endl;
                    send(sock, message.c_str(), message.size() + 1, 0);
                }
                if (i == m_master.fd_count - 1 && m_broadcast_messages_out.size() > 0)
                m_broadcast_messages_out.pop_front();
            }
            if (m_messages_out.contains(sock))
            {
                std::scoped_lock<std::mutex> message_lock(m_messages_out_mutex);
                send(sock, m_messages_out.at(sock).c_str(), m_messages_out.at(sock).size() + 1, 0);
                m_messages_out.erase(sock);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); //Save CPU
    }
}

void Server::stop()
{
    m_running = false;
}

void Server::setCallbacks(on_client_message_callback_t on_client_message,
                            on_client_connect_callback_t on_client_connect,
                            on_client_disconnect_callback_t on_client_disconnect)
{
    m_on_client_message = on_client_message;
    m_on_client_connect = on_client_connect;
    m_on_client_disconnect = on_client_disconnect;
}

void Server::broadcastMessage(const std::string &message)
{
    std::scoped_lock<std::mutex> lock(m_broadcast_messages_out_mutex);
    m_broadcast_messages_out.push_back(message);
}

void Server::sendMessage(const uint64_t client, const std::string &message)
{
    std::scoped_lock<std::mutex> lock(m_messages_out_mutex);
    m_messages_out.try_emplace(client, message);
}

void Server::closeClient(const uint64_t client)
{
    std::scoped_lock<std::mutex> lock(m_clients_to_close_mutex);
    m_clients_to_close.push_back(client);
}

}; // namespace FindIt
