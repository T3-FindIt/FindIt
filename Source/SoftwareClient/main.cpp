#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <stdint.h>
#include <random>
#ifdef _WIN32
#include <ws2tcpip.h>
#endif // _WIN32

bool running = true;

void IOThread()
{
    while (running)
    {
        if (std::cin.get() == 'q')
            running = false;
    }
}

//https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
std::string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

SOCKET init()
{
    if (WSADATA wsData; WSAStartup(MAKEWORD(2, 2), &wsData) != NO_ERROR)
        {throw std::runtime_error("Failed to initialize Winsock");}

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
        {throw std::runtime_error("Failed to initialize socket");}

    SOCKADDR_IN target;
    target.sin_family = AF_INET;
    // SET SERVER INFO HERE
    target.sin_port = htons (54000);
    // target.sin_addr.s_addr = inet_addr ("192.168.56.1");
    target.sin_addr.s_addr = inet_addr ("127.0.0.1");

    if (connect(sock, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
        {throw std::runtime_error("Failed to connect to server");}

    DWORD timeout = 1000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    std::srand(std::time(nullptr));
    return sock;
}

void send_data_to_server(SOCKET sock, std::string data)
{
    if (send(sock, data.c_str(), data.length(), 0) == SOCKET_ERROR)
        {throw std::runtime_error("Failed to send data to server");}
}

std::string receive_data_from_server(SOCKET sock)
{
    char buffer[1024] = {0};
    int bytesReceived = recv(sock, buffer, 1024, 0);
    if (bytesReceived != SOCKET_ERROR && bytesReceived != 0)
    {
        std::string incomingMsg = std::string(buffer, 0, bytesReceived);
        return incomingMsg;
    }
    return "";
}

void print_screen(const std::string& name, const std::string& msg = "")
{
    system("cls");
    std::cout << "Enter 'q' to stop the client" << std::endl;
    std::cout << "Client name: " << name << std::endl;
    if (msg != "")
    {
        std::cout << msg << std::endl;
    }
}

int main()
{
    std::jthread t(IOThread);
    std::chrono::time_point<std::chrono::system_clock> prev, now;

    SOCKET sock = init();
    // Generate a random string for the node name
    std::string node_name = gen_random(10);
    print_screen(node_name);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    send_data_to_server(sock, "{\"Action\":\"SignIn\",\"Node\":\"" + node_name + "\",\"Places\":4}");
    std::cout << receive_data_from_server(sock) << std::endl;

    // add new product to db
    std::string product_name = gen_random(10);
    send_data_to_server(sock, "{\"Action\":\"NotifyNewProduct\",\"Product\":\"" + product_name + "\"}");
    print_screen(node_name, "Added new product to db!\nProduct name: " + product_name);


    std::string response = receive_data_from_server(sock);
    print_screen(node_name, response);

    while (running)
    {
        response = receive_data_from_server(sock);
        if (response != "")
        {
            print_screen(node_name, "Recv: " + response);
            if (response == "{\"Action\":\"HeartBeat\"}")
            {
                std::cout << "Heartbeat received" << std::endl;
                send_data_to_server(sock, "{\"Action\":\"HeartBeat\",\"Node\":\"" + node_name + "\",\"Places\":4}");
                // send_data_to_server(sock, "I am a faulty client, Disconnect me!");
            }
        }
    }

    std::cout << "Attempting to stop" << std::endl;

    // Close socket
    closesocket(sock);
    WSACleanup();

    std::cout << "Press any key to close the program..." << std::endl;
    std::cin.get();

    return 0;
}
