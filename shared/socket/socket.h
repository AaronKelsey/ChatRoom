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
  SOCKET accept();
  int send(char* cMessage);
  int receive(char* cMessage);

  void close();

private:
  WSADATA oWsaData;

  SOCKADDR_IN oServerInfo;

  SOCKET oSocket;

  int iReturnValue;

  int iPort;

  bool bConnected;
  bool bClosed;
};

#endif // SOCKET_H

