#include <iostream>	
#include "Socket.hpp"

void Listener_MessageReceived(FindIt::Socket *listener, int socketId, std::string message)
{
    std::string msg = "Hello from server\r";
    listener->Send(socketId, msg);
    std::cout << message << std::endl;
}

int main()
{
    FindIt::Socket server("IP", 8090, Listener_MessageReceived);

    if (server.Init())
    {
        server.Run();
    }

    return 0;
}
