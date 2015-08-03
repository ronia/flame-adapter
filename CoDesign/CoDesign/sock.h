/**
* @(#)sock.h    1.0.0
*
* Copyright 2004 Vijay Mathew Pandyalakal. All rights reserved.
*
* Change history:
* Created on Dec 10, 2003
* Last Modified on July 07, 2004
*
*/


#ifndef _SOCK_H_
#define _SOCK_H_

#include <string>
#include <vector>
using namespace std;

/**
* socket operations static values
*/

#define SOCKET_OP_SUCCESS		0
#define SOCKET_OP_FAIL			-1

#define SOCKET_OP_TIMEOUT_SEC	3


/**
* Defines a classes that wraps up the winsock API.These classes
* do not supports only TCP/IP.
* @author  Vijay Mathew Pandyalakal
* @version 1.0.0
*/ 

namespace openutils {
	
	class CSocket;
	class CSocketException;
	class CSocketAddress;
	
	/**
	* class CWinSock
	* Performs winsock initialization and cleanup
	* @since 1.0.0
	*/
	class CWinSock {
	public:
		static void Initialize();/// WSAStartup
		static void Finalize();/// WSACleanup
	};

	/**
	* class CServerSocket.
	* Wraps up win32 calls for creating a server.
	* @since 1.0.0
	*/
	class CServerSocket {
	private:		
		SOCKET m_socket; /// listening socket
		SOCKADDR_IN m_sockAddrIn; /// default socket settings
		int m_nPort; /// server port
		int m_nQueue; /// number of clients that can be in queue waiting for acceptance.
		CSocketAddress *m_sockAddr; /// Address to which this server is attached.
		bool m_bBound; /// true if bound to port.
		bool m_bListening; /// true if listening
	public:
		CServerSocket();  /// default constructor
		CServerSocket(int port); /// overloaded constructor
		CServerSocket(int port,int queue); /// overloaded constructor
		~CServerSocket(); /// default destructor
		void Bind(CSocketAddress *scok_addr);/// Binds the server to the given address.
		CSocket* Accept();// throw (CSocketException);/// Accepts a client connection.
		void Close(); /// Closes the Socket.	
		bool IsListening(); /// returns the listening flag
		
		void SetPort(int port); /// Sets the port
		void SetQueue(int q); /// Sets the queue size
		int GetPort(); /// returns the port
		int GetQueue(); /// returns the queue size
		CSocketAddress* GetSocketAddress(); /// Returns the socket address
	private:
		void Init(); /// default initialization
	};

	/**
	* class CSocket
	* Wraps up a raw SOCKET
	* @since 1.0.0
	*/
	class CSocket {
	private:
		SOCKET m_socket;										/// SOCKET for communication
		CSocketAddress *m_clientAddr;							/// Address details of this socket.
	public:
		CSocket();												/// Default constructor
		void SetSocket(SOCKET sock);							/// Sets the SOCKET
		void SetClientAddr(SOCKADDR_IN addr);					/// Sets address details
		void Connect(std::string &msg);							/// Connects to a server
		int Connect(const char* host_name, 
					int port, 
					std::string &msg);							/// Connects to host
		CSocketAddress* GetAddress() { return m_clientAddr; }	/// Returns the client address
		int Send(const char* data);								/// Writes data to the socket
		int Read(char* buffer,int len);							/// Reads data from the socket
		void Close();											/// Closes the socket
		~CSocket();												/// Destructor
	};
	
	/**
	* class CSocketAddress
	* Wraps up an internet address object.
	* @since 1.0.0
	*/
	class CSocketAddress {
	private:
		SOCKADDR_IN m_sockAddrIn; /// server info
		LPHOSTENT m_lpHostEnt; /// used to obtain address by name
		string m_strHostName; /// host name
		int m_nPort; /// port 
	public:
		CSocketAddress(const char* host,int port); /// default constructor		
		CSocketAddress(SOCKADDR_IN sockAddr);/// Constructor initialized by a SOCKADDR_IN
		const char* GetIP(); /// Returns the IP address
		const char* GetName(); /// Returns the official address
		int GetPort() { return m_nPort; } /// Returns the port
		void GetAliases(vector<string>* ret); /// Returns aliases
		SOCKADDR_IN GetSockAddrIn(); // throw (CSocketException); /// returns the sockaddr_in
		void operator = (CSocketAddress addr); /// Assignment operation
		~CSocketAddress(); /// Destructor
	};
	
	/**
	* class CSocketException
	* Thrown by all sock.h classes.
	* @since 1.0.0
	*/
	class CSocketException {
	private:
		string m_strError; /// error message
		int m_nCode; /// Error code
	public:
	/**
	* Default constructor.
	* @param code Error code.
	* @param msg Error message
	* @since 1.0.0
		*/
		CSocketException(int code,const char* msg) {
			m_nCode = code;
			m_strError = msg;
		}
		/**
		* returns the error code.
		* @return int
		* @since 1.0.0
		*/
		inline int GetCode() { return m_nCode; }
		/**
		* returns the error message.
		* @return const char*
		* @since 1.0.0
		*/
		inline const char* GetMessage() { return m_strError.c_str(); }
	};
}

#endif