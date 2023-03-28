#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <string>

namespace FindIt
{

class Socket;

// typedef void (*MessageReceivedHandler)(TCPListener *listener, int socketId, std::string message);
using MessageReceivedHandler = void (*)(Socket *listener, int socketId, std::string message);

class Socket
{
private:
    SOCKET CreateSocket();
	SOCKET WaitForConnection(SOCKET listening);
	std::string m_ipAddress;
	int m_port;
	MessageReceivedHandler MessageReceived;
public:
    Socket(std::string ipAddress, int port, MessageReceivedHandler handler);
	~Socket();
	void Send(int clientSocket, std::string message);
	bool Init();
	[[noreturn]] void Run();
};

} // namespace FindIt

#endif // SOCKET_HPP
