//
// Filename:    socket.h 
// Description: This file wraps the WinSock2 library 
// Author:      Aaron Kelsey
//

#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <string>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#ifdef SIMPLESOCKET_EXPORT
  #define SIMPLESOCKET_API __declspec(dllexport)
#else
  #define SIMPLESOCKET_API __declspec(dllimport)
#endif

class SIMPLESOCKET_API Socket
{
public:
  Socket();
  ~Socket();

  int connect(const int iPort, const std::string& sAddress);
  int bind(const int iPort);
  int listen(const int iBacklog);
  std::shared_ptr<Socket> accept();
  int send(const std::string& sMessage);
  int receive(std::string& sMessage);

  SOCKET getSocket() { return oSocket; }

  void close();

private:
  void setSocket(SOCKET socket) { oSocket = socket; }

  WSADATA     oWsaData;
  SOCKADDR_IN oServerInfo;
  SOCKET      oSocket;

  bool bConnected;
  bool bClosed;

  static const int iBufferSize = 4000;
};

#endif // SOCKET_H

