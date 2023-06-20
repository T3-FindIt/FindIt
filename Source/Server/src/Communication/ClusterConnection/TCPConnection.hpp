#ifndef SERVER_HPP
#define SERVER_HPP

#include "IClusterConnection.hpp"

#include "./Exceptions/FailedSocketInit.hpp"
#include "./Exceptions/FailedWSAStartup.hpp"

#include <string>
#include <thread>
#include <mutex>
#include <functional>
#include <stdint.h>
#include <list>
#ifdef _WIN32
#include <ws2tcpip.h>
#endif // _WIN32

namespace FindIt
{

#define MAX_BUFFER_SIZE (4096)
#define RECV_TIMEOUT    (100)

class Server : public IClusterConnection
{
public:
    Server(unsigned short port);
    ~Server();

    void run() final;
    void stop() final;

    void setCallbacks(on_client_message_callback_t on_client_message,
                      on_client_connect_callback_t on_client_connect,
                      on_client_disconnect_callback_t on_client_disconnect) final;

    void broadcastMessage(const std::string& message) final;

    void sendMessage(const uint64_t client, const std::string& message) final;

    void closeClient(const uint64_t client) final;

private:
    std::list<std::string> m_broadcast_messages_out;
    std::mutex m_broadcast_messages_out_mutex;

    std::list<uint64_t> m_clients_to_close;
    std::mutex m_clients_to_close_mutex;

    std::unordered_map<uint64_t, std::string> m_messages_out;
    std::mutex m_messages_out_mutex;

    fd_set m_master;
    SOCKET m_listener;
};

}; // namespace FindIt

#endif // SERVER_HPP
