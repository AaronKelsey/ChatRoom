//
// Filename:    socket.cpp 
// Description: This file wraps the WinSock2 library 
// Author:      Aaron Kelsey
//

#include "socket.h"

#include <Ws2tcpip.h>
#include <iostream>

Socket::Socket()
{
  bClosed = false; 
  bConnected = true; 

  if (WSAStartup(MAKEWORD(2, 2), &oWsaData) != 0)
  {
    std::cerr << "WSAStartup failed with error: " + WSAGetLastError() << std::endl;
  }
  else
  {
    std::cout << "Winsock successfully found" << std::endl;
  }

  // Create listening socket
  oSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (oSocket == INVALID_SOCKET)
  {
    std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
    close();
  }
}

Socket::~Socket()
{
  close();

  if (WSACleanup() == SOCKET_ERROR)
  {
    std::cerr << "WSASCleanup failed with error: " + WSAGetLastError() << std::endl;
  }
  else
  {
    std::cout << "Winsock successfully cleaned up" << std::endl;
  }
}

int Socket::connect(const int iPort, const std::string& sAddress)
{
  InetPton(AF_INET, sAddress.c_str(), &oServerInfo.sin_addr.s_addr);

  // Address information
  oServerInfo.sin_family = AF_INET;
  oServerInfo.sin_port = htons(iPort); // Convert port to network-byte order

  // Connect to server
  int iReturnValue = ::connect(this->oSocket, (sockaddr*)&oServerInfo, sizeof(oServerInfo));

  if (iReturnValue == SOCKET_ERROR)
  {
    std::cerr << "Error connecting to server: " << WSAGetLastError() << std::endl;
    close();
  }

  bConnected = true;

  return iReturnValue;
}

int Socket::bind(const int iPort)
{
  // Address information
  SOCKADDR_IN oServerInfo;
  oServerInfo.sin_family = AF_INET;
  oServerInfo.sin_addr.s_addr = INADDR_ANY; // Listen for any connection
  oServerInfo.sin_port = htons(iPort); // Convert port to network-byte order

  // Bind the socket to local server address
  int iReturnValue = ::bind(this->oSocket, (LPSOCKADDR)&oServerInfo, sizeof(struct sockaddr));

  if (iReturnValue == SOCKET_ERROR)
  {
    std::cerr << "Error binding socket: " << WSAGetLastError() << std::endl;
    close();
  }

  return iReturnValue;
}

int Socket::listen(const int iBacklog)
{
  // Allow socket to listen
  int iReturnValue = ::listen(this->oSocket, iBacklog);

  if (iReturnValue == SOCKET_ERROR)
  {
    std::cerr << "Error enabling socket for listening: " << WSAGetLastError() << std::endl;
    close();
  }

  return iReturnValue;
}

std::shared_ptr<Socket> Socket::accept()
{
  SOCKET oClientSocket;
  oClientSocket = ::accept(this->oSocket, NULL, NULL);

  if (oClientSocket == INVALID_SOCKET)
  {
    std::cerr << "Error waiting for client: " << WSAGetLastError() << std::endl;
    close();
  }

  std::shared_ptr<Socket> newSocket(new Socket());
  newSocket->setSocket(oClientSocket);

  return newSocket;
}

int Socket::send(const std::string& sMessage)
{
  int iBytesReceived = ::send(oSocket, sMessage.c_str(), static_cast<int>(sMessage.length()), 0);

  if (iBytesReceived == SOCKET_ERROR)
  {
    std::cerr << "No bytes sent - error:" << WSAGetLastError() << std::endl;
  }

  return iBytesReceived;
}

int Socket::receive(std::string& sMessage)
{
  // Create buffer which received messages will be written to
  char cBuffer[iBufferSize];
  memset(&cBuffer, 0, sizeof(cBuffer));

  int iBytesReceived = ::recv(oSocket, (char*)&cBuffer, sizeof(cBuffer), 0);

  if (iBytesReceived == SOCKET_ERROR)
  {
    std::cerr << "No bytes recveived - error: " << WSAGetLastError() << std::endl;
  }

  sMessage = std::string(cBuffer);
  return iBytesReceived;
}

void Socket::close()
{
  if (!bClosed)
  {
    closesocket(oSocket);
    bClosed = false;
  }
}