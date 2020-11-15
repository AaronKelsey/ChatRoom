//
// Filename:    socket.h 
// Description: This file wraps the WinSock2 library 
// Author:      Aaron Kelsey
//

#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <WinSock2.h>

#ifdef SIMPLESOCKET_EXPORT
  #define SIMPLESOCKET_API __declspec(dllexport)
#else
  #define SIMPLESOCKET_API __declspec(dllimport)
#endif

class SIMPLESOCKET_API Socket
{
public:
  /**
    * Initialises WinSock with WSAStartup and creates a socket
    */
  Socket();

  /**
    * Closes socket and cleans up WinSock with WSACleanup
    */
  ~Socket();

  /**
    * Connects socket to target
    * @param Port to connect to
    * @param IP Address to connect to
    * @return 0 if successul or -1 if failed
    */
  int connect(const int iPort, const std::string& sAddress);

  /**
    * Bind the socket to local address
    * @param Assigned port to the socket
    * @return 0 if successul or -1 if failed
    */
  int bind(const int iPort);

  /**
    * Allows socket to listen for connections
    * @param Maximum number of pending connections in queue
    * @return 0 if successul or -1 if failed
    */
  int listen(const int iBacklog);

  /**
    * Connect permits an incoming connection request to the socket
    * @return Socket object with newly created SOCKET if successful
    */
  std::shared_ptr<Socket> accept();

  /**
    * Connects socket to target
    * @param Message to be sent
    * @return Number of bytes sent if successul or -1 if failed
    */
  int send(const std::string& sMessage);

  /**
    * Receives data from connected socket
    * @param Buffer to receive incoming message
    * @return Number of bytes received if successul or -1 if failed
    */
  int receive(std::string& sMessage);

  /**
    * Getter function to return socker descriptor file
    * @return SOCKET object
    */
  SOCKET getSocket() { return oSocket; }

  /**
    * Closes the socket connection
    * @return void
    */
  void close();

private:
  /**
    * Setter function to set socker descriptor file
    * @return void
    */
  void setSocket(SOCKET socket) { oSocket = socket; }

  WSADATA     oWsaData;
  SOCKADDR_IN oServerInfo;
  SOCKET      oSocket;

  bool bConnected;
  bool bClosed;

  static const int iBufferSize = 4000;
};

#endif // SOCKET_H