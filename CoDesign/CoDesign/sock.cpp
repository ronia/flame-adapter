/**
* @(#)sock.cpp    1.0.0
*
* Copyright 2004 Vijay Mathew Pandyalakal. All rights reserved.
*
* Change history:
* Created on Dec 10, 2003
* Last Modified on July 08, 2004
*
*/

#include "stdafx.h"
#include <string>
#include <vector>
using namespace std;
#include <windows.h>
#include "sock.h"
using namespace openutils;


/**
* Defines a classes that wraps up the winsock API.These classes
* do not supports only TCP/IP.
* @author  Vijay Mathew Pandyalakal
* @version 1.0.0
*/ 

/**
* WSAStartup
* @since 1.0.0
*/
void CWinSock::Initialize() {
	WORD ver = MAKEWORD(1, 1);
	WSADATA wsadata;
	WSAStartup(ver, &wsadata);
}

/**
* WSACleanup
* @since 1.0.0
*/
void CWinSock::Finalize() {
	WSACleanup();
}

/**
* Default constructor
* @since 1.0.0
*/
CServerSocket::CServerSocket() {
	m_nPort = 80;
	m_nQueue = 10;
	Init();
}

/**
* Overloaded constructor.
* @param port Port on which the server listens
* @since 1.0.0
*/
CServerSocket::CServerSocket(int port) {
	m_nPort = port;
	m_nQueue = 10;
	Init();	
}

/**
* Overloaded constructor.
* @param port Port on which the server listens
* @param queue Number of clients that will wait for accept()
* @since 1.0.0
*/
CServerSocket::CServerSocket(int port,int queue) {
	m_nPort = port;
	m_nQueue = queue;
	Init();	
}

/**
* Binds the server to the given address.
* @param *sock_addr Socket address to bind to.
* @since 1.0.0
*/
void CServerSocket::Bind(CSocketAddress *sock_addr) {
	m_bBound = false;	
	Close();
	m_sockAddr = sock_addr;
	Accept();	
}

/**
* Listens and accepts a client.Returns the accepted connection.
* @return CSocket*.
* @throw CSocketException
* @since 1.0.0
*/
CSocket* CServerSocket::Accept() { // throw (CSocketException) {
	if(m_sockAddr != NULL) 
		m_sockAddrIn = m_sockAddr->GetSockAddrIn();
	if(!m_bBound) {	
		m_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		int nret = bind(m_socket, (LPSOCKADDR)&m_sockAddrIn, sizeof(struct sockaddr));
		if (nret == SOCKET_ERROR) {
			nret = WSAGetLastError();
			throw CSocketException(nret, "Failed to bind: Accept()");
		}
		m_bBound = true;
	}	
	int nret = listen(m_socket, m_nQueue);
	if (nret == SOCKET_ERROR) {
		nret = WSAGetLastError();
		throw CSocketException(nret, "Failed to listen: Accept()");
	}
	SOCKET theClient;
	SOCKADDR_IN clientAddr;
	int ssz = sizeof(struct sockaddr);
	theClient = accept(m_socket,(LPSOCKADDR)&clientAddr,&ssz);
	//theClient = accept(m_socket,NULL,NULL);
	if (theClient == INVALID_SOCKET) {
		int nret = WSAGetLastError();
		throw CSocketException(nret, "Invalid client socket: Accept()");		
	}
	CSocket *sockClient = new CSocket();
	sockClient->SetSocket(theClient);
	sockClient->SetClientAddr(clientAddr);		
	return sockClient;
}


/**
* Closes the socket.
* @throw CSocketException
* @since 1.0.0
*/
void CServerSocket::Close() {
	closesocket(m_socket);
	m_sockAddr = NULL;	
	m_bBound = false;
	m_bListening = false;
}


/**
* default initialization
* @throw CSocketException
* @since 1.0.0
*/
void CServerSocket::Init() {		
	m_sockAddrIn.sin_family = AF_INET;
	m_sockAddrIn.sin_addr.s_addr = INADDR_ANY;	
	m_sockAddrIn.sin_port = htons(m_nPort);
	
	m_sockAddr = NULL; // bind the same machine
	m_bBound = false;
	m_bListening = true;
}

/**
* Destructor. Releases all resource.
* @since 1.0.0
*/
CServerSocket::~CServerSocket() {
	Close();
}

/**
* Sets the port
* @param port Value of port
* @since 1.0.0
*/
void CServerSocket::SetPort(int port) {
	m_nPort = port;
	Init();
}

/**
* Sets the queue size
* @param port Value of port
* @since 1.0.0
*/
void CServerSocket::SetQueue(int q) {
	m_nQueue = q;	
}

/**
* Returns the port
* @return int
* @since 1.0.0
*/
int CServerSocket::GetPort() {
	return m_nPort;
}

/**
* Returns the queue size
* @return int
* @since 1.0.0
*/
int CServerSocket::GetQueue() {
	return m_nQueue;
}

/**
* Returns the socket address
* @return CSocketAddress*
* @since 1.0.0
*/
CSocketAddress* CServerSocket::GetSocketAddress() {
	return m_sockAddr;
}

/**
* Returns the listening flag
* @return bool
* @since 1.0.0
*/
bool CServerSocket::IsListening() {
	return m_bListening;
}

// class CSocket

/**
* Default constructor
* @since 1.0.0
*/
CSocket::CSocket() {
	m_clientAddr = NULL;
}

/**
* Sets the SOCKET
* @param sock SOCKET
* @since 1.0.0
*/
void CSocket::SetSocket(SOCKET sock) {
	m_socket = sock;
}

/**
* Sets address details
* @param addr SOCKADDR_IN
* @since 1.0.0
*/
void CSocket::SetClientAddr(SOCKADDR_IN addr) {
	if(m_clientAddr != NULL) delete m_clientAddr;
	m_clientAddr = new CSocketAddress(addr);
}

/**
* Connects to a server
* @since 1.0.0
*/
void CSocket::Connect(std::string &msg) {
// throw (CSocketException) {
	if(m_clientAddr == NULL)
		throw CSocketException(-1,"Cannot connect to NULL host");
	Connect(m_clientAddr->GetName(),m_clientAddr->GetPort(), msg);
}

/**
* Connects to a server
* @param host_name Server name
* @param port Port to connect
* @since 1.0.0
*/
int CSocket::Connect(const char* host_name, int port, std::string &msg) {	
	
	// the return value
	int			ret			= SOCKET_OP_FAIL;

	// gets hostname
	LPHOSTENT hostEntry;
	hostEntry = gethostbyname(host_name);
	if (!hostEntry) {
		msg = "Failed to resolve hostname";
		return SOCKET_OP_FAIL;
	}
	
	// initializes the socket
	m_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET) {
		msg = "Failed to create client socket";
		return SOCKET_OP_FAIL;
	}

	// fills in the server address information
	SOCKADDR_IN serverInfo;
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);
	serverInfo.sin_port = htons(port);

	//////////////////////////////////
	// enables non-blocking mode
	//////////////////////////////////

	unsigned long on = 1;
	if (0 != ioctlsocket(m_socket, FIONBIO, &on))
	{
		/* Handle failure. */
		msg = "Failed to switch to non-blocking socket";
		return SOCKET_OP_FAIL;
	}
	
	//////////////////////////////////
	// begins connection
	//////////////////////////////////

	int connection_ret = connect(m_socket,(LPSOCKADDR)&serverInfo,sizeof(struct sockaddr));

	// Non-blocking socket always returns SOCKET_ERROR immediately
	if (connection_ret == SOCKET_ERROR) {
		
		// gets the winsock error code
		int err = WSAGetLastError();
		if(err == WSAEWOULDBLOCK) {
			// select value
			int val;

			fd_set write_fd, error_fd;
			TIMEVAL timeout;

			FD_ZERO(&write_fd);
			FD_ZERO(&error_fd);
			FD_SET(m_socket, &write_fd);
			FD_SET(m_socket, &error_fd);

			timeout.tv_sec	= SOCKET_OP_TIMEOUT_SEC;
			timeout.tv_usec	= 0;

			// select will block the process until it:
			// (1) senses a change or
			// (2) alerts for timeout
			val =	select(0,		// ignored
					NULL,			// read	check
					&write_fd,		// write check
					&error_fd,		// error check
					&timeout);

			// if select() timed out
			if(val == 0) {
				msg = "Timeout occurred";
			} 
			// if select() sensed a change
			else {
				// if the socket is writable, which means it is connected
				if(FD_ISSET(m_socket, &write_fd)) {
					msg = "Succeeded";
					ret = SOCKET_OP_SUCCESS;
				}

				// if the socket detected an error
				if(FD_ISSET(m_socket, &error_fd)) {
					msg = "Error occurred at select()";
				}
			}

		} else {

			// error handling
			switch (err) {
				case WSAECONNREFUSED:
					msg = "Connection refused";
					break;

				case WSAENETUNREACH:
					msg = "Host unreachable";
					break;
				
				case WSAETIMEDOUT:
					msg = "Connection timed out";
					break;

				default:
					msg = "Unspecified error at connect()";
					break;
			}
		}
	}

	//////////////////////////////////
	// disables non-blocking mode
	//////////////////////////////////

	unsigned long off = 0;
	if (0 != ioctlsocket(m_socket, FIONBIO, &off))
	{
		/* Handle failure. */
		msg = "Failed to switch to blocking socket";
		return SOCKET_OP_FAIL;
	}

	return ret;
}

/**
* Writes data to the socket. Returns number of bytes written
* @param data data to write
* @return int
* @throw CSocketException
* @since 1.0.0
*/
int CSocket::Send(const char* data) { // throw (CSocketException) {
	
	
	int nret = 0;

	// translate the length
	char length [3];
	length[0] = (unsigned char) ((int) (strlen(data) / 128));
	length[1] = (unsigned char) ((int) (strlen(data) % 128));
	length[2] = '\n';
	
	// send the length
	nret = send(m_socket, length, 2, 0);
	

	if(nret == SOCKET_ERROR) {
		nret = WSAGetLastError();
		//throw CSocketException(nret, "Network failure: Send() -- while sending the length of a message");		
	}

	// send the message
	nret = send(m_socket, data, strlen(data), 0);

	if(nret == SOCKET_ERROR) {
		nret = WSAGetLastError();
		//throw CSocketException(nret, "Network failure: Send() -- while sending the actual message");		
	}

	return nret;
}

/**
* Reads data from the socket.Returns number of bytes actually read.
* @param buffer Data buffer
* @param len Number of bytes to read
* @throw CSocketException
* @since 1.0.0
*/
int CSocket::Read(char* buffer, int len) { // throw (CSocketException) {
	
	int nret = 0;	

	// if the buffer size is smaller than 2, that is an error
	if (len < 2)
		return -1;

	// receive the length (2 bytes)
	nret = recv(m_socket, buffer, 2, 0);

	if(nret == SOCKET_ERROR) {
		//AfxMessageBox("Error: Length packet broken");
		return -1;		
	}

	// translate the length
	int length = ((int) buffer[0] * 128) + (int) buffer[1];

	// check if the message is too big
	if (length > len) {
		//AfxMessageBox("Error: Received message too big");
		return -1;
	}

	// receive the message
	nret = recv(m_socket, buffer, length, 0);
	
	if(nret == SOCKET_ERROR) {
		AfxMessageBox("Error: Socket error");
		return -1;		
	}

	if(nret > 0) {
		buffer[nret] = '\0';
	}

	return nret;	
}

/**
* Closes the socket
* @since 1.0.0
*/
void CSocket::Close() {	
	closesocket(m_socket);
	if(m_clientAddr != NULL) delete m_clientAddr;
}

/**
* Destructor
* @since 1.0.0
*/
CSocket::~CSocket() {
	Close();
}

// CSocketAddress

/**
* Default constructor
* @param host Host name.
* @param port Host port
* @since 1.0.0
*/
CSocketAddress::CSocketAddress(const char* host,int port) {
	m_sockAddrIn.sin_family = AF_INET; 
	m_sockAddrIn.sin_addr.s_addr = INADDR_ANY; // initialized only in GetSockAddrIn()
	m_sockAddrIn.sin_port = htons(port);
	m_strHostName = host;
	m_nPort = port;
}

/**
* Constructor initialized by a SOCKADDR_IN
* @param sockAddr Address information
* @since 1.0.0
*/
CSocketAddress::CSocketAddress(SOCKADDR_IN sockAddr) {
	m_sockAddrIn.sin_family = sockAddr.sin_family;
	m_sockAddrIn.sin_addr.s_addr = sockAddr.sin_addr.s_addr;
	m_sockAddrIn.sin_port = sockAddr.sin_port;
	m_strHostName = inet_ntoa(m_sockAddrIn.sin_addr);
	m_nPort = sockAddr.sin_port;;
}

/**
* Returns the IP address
* @return const char*
* @since 1.0.0
*/
const char* CSocketAddress::GetIP() {
	return (const char*)inet_ntoa(m_sockAddrIn.sin_addr);
}

/**
* Returns the official address
* @return const char*
* @since 1.0.0
*/
const char* CSocketAddress::GetName() {
	HOSTENT *lpHost = gethostbyname(GetIP());
	if(lpHost == NULL) return NULL;
	return lpHost->h_name;
}

/**
* Returns aliases
* @param vector<string>* values are returned here
* @since 1.0.0
*/
void CSocketAddress::GetAliases(vector<string>* ret) {
	HOSTENT *lpHost = gethostbyname(GetIP());
	if(lpHost == NULL) return;
	char** tmp = (char**)lpHost->h_aliases;	
	if(tmp == NULL) return;
	else {
		int i = 0;
		while(true) {
			if(tmp[i] == NULL) break;
			else ret->push_back(tmp[i]);
		}
	}
}

/**
* Returns the sockaddr_in. tries to bind with the server.
* throws CSocketException on failure.
* @return SOCKADDR_IN
* @since 1.0.0
*/
SOCKADDR_IN CSocketAddress::GetSockAddrIn() { // throw (CSocketException) {
	m_lpHostEnt = gethostbyname(m_strHostName.c_str());
	if (!m_lpHostEnt) {
		int nret = WSAGetLastError();
		throw CSocketException(nret, "Failed to resolve host:gethostbyname()");
	}
	m_sockAddrIn.sin_addr = *((LPIN_ADDR)*m_lpHostEnt->h_addr_list);
	return m_sockAddrIn;
}

/**
* Assignment operation
* @param addr Object is copied into this.
* @since 1.0.0
*/
void CSocketAddress::operator = (CSocketAddress addr) {
	m_sockAddrIn = addr.m_sockAddrIn;
	m_strHostName = addr.m_strHostName;
	m_nPort = addr.m_nPort;
	m_lpHostEnt = addr.m_lpHostEnt;
}

/**
* Destructor
* @since 1.0.0
*/
CSocketAddress::~CSocketAddress() {
}
