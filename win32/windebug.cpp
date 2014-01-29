#include "windebug.h"

#include "x2d/math.h"

#include <WinSock2.h>
#include <ws2tcpip.h>

//----------------------------------------------------------------------------
// Debugger
//----------------------------------------------------------------------------
WinDebug::WinDebug() :
	m_server(INVALID_SOCKET),
	m_client(INVALID_SOCKET)
{
}

WinDebug::~WinDebug()
{
	disconnect();
}

bool WinDebug::init()
{
	// Initialize Winsock
	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(r < 0) {
		// Error initializing socket
		iosystem::print("WSAStartup() failed with error code '%i'!", r);
		return false;
	}
	return true;
}

bool WinDebug::listen(ushort port)
{
	// Create address struct
	struct addrinfo *result = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	int r = getaddrinfo(NULL, math::intToStr(port).c_str(), &hints, &result);
	if(r < 0) {
		iosystem::print("getaddrinfo() failed with error code '%i'!", r);
		WSACleanup();
		return false;
	}

	// Create a SOCKET for the server to listen for client connections
	m_server = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if(m_server == INVALID_SOCKET) {
		iosystem::print("socket() failed!");
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	// Set non-blocking mode (Needed for runtime debug)
	ulong mode = 1;
	ioctlsocket(m_server, FIONBIO, &mode);

	// Setup the TCP listening socket
    r = bind(m_server, result->ai_addr, (int)result->ai_addrlen);
    if(r == SOCKET_ERROR) {
        iosystem::print("bind() failed with error code '%i'!", r);
        freeaddrinfo(result);
        closesocket(m_server);
        WSACleanup();
        return false;
    }
	freeaddrinfo(result);

	// Listen at the port
	r = ::listen(m_server, SOMAXCONN);
	if(r == SOCKET_ERROR) {
		iosystem::print("listen() failed with error code '%i'!", r);
		closesocket(m_server);
		WSACleanup();
		return false;
	}
	return true;
}

bool WinDebug::accept()
{
	// Look for client
	return (m_client = ::accept(m_server, NULL, NULL)) != INVALID_SOCKET;
}

void WinDebug::disconnect()
{
	// Close client socket and clean up
	closesocket(m_client);
	WSACleanup();
}

bool WinDebug::send(const char *data)
{
	return ::send(m_client, data, 512, 0) != SOCKET_ERROR;
}

bool WinDebug::recv(char **data)
{
	return ::recv(m_client, (*data), 512, 0) != SOCKET_ERROR;
}

int WinDebug::bytesReady()
{
	// Check bytes available
	ulong numBytes;
	ioctlsocket(m_client, FIONREAD, &numBytes);
	return numBytes;
}