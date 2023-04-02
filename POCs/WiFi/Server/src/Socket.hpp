#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <ws2tcpip.h>
#include <winsock2.h>
#include <string>

namespace FindIt
{

class Socket;

using MessageReceivedHandler = void (*)(Socket *listener, int socketId, std::string message);

class Socket
{
private:
	std::string m_ipAddress;
	int m_port;
	MessageReceivedHandler m_messageReceived;
    
	SOCKET CreateSocket();
	SOCKET WaitForConnection(SOCKET listening);
public:
    Socket(const std::string& ipAddress, int port, MessageReceivedHandler handler);
	~Socket();
	void Send(int clientSocket, const std::string& message);
	bool Init();
	[[noreturn]] void Run();
};

} // namespace FindIt

#endif // SOCKET_HPP
