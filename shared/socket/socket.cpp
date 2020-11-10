//
// Filename:    socket.cpp 
// Description: This file wraps the WinSock2 library 
// Author:      Aaron Kelsey
//

#include "socket.h"

Socket::Socket()
{
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
}

int Socket::connect(const int iPort, const std::string& sAddress)
{
  InetPton(AF_INET, sAddress.c_str(), &oServerInfo.sin_addr.s_addr);

  // Address information
  oServerInfo.sin_family = AF_INET;
  oServerInfo.sin_port = htons(iPort); // Convert port to network-byte order

  // Connect to server
  iReturnValue = ::connect(this->oSocket, (sockaddr*)&oServerInfo, sizeof(oServerInfo));

  if (iReturnValue == SOCKET_ERROR)
  {
    std::cerr << "Error connecting to server: " << WSAGetLastError() << std::endl;
    close();
  }

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
  iReturnValue = ::bind(this->oSocket, (LPSOCKADDR)&oServerInfo, sizeof(struct sockaddr));

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
  iReturnValue = ::listen(this->oSocket, iBacklog); 

  if (iReturnValue == SOCKET_ERROR)
  {
    std::cerr << "Error enabling socket for listening: " << WSAGetLastError() << std::endl;
    close();
  }

  return iReturnValue;
}

SOCKET Socket::accept()
{
  SOCKET oClientSocket;
  oClientSocket = ::accept(this->oSocket, NULL, NULL);

  if (oClientSocket == INVALID_SOCKET)
  {
    std::cerr << "Error waiting for client: " << WSAGetLastError() << std::endl;
    close();
  }

  return oClientSocket;
}

int Socket::send(char* cMessage)
{
  int iLength = sizeof(cMessage);

  int iBytesReceived = ::send(oSocket, (char*)&cMessage, iLength, 0);

  if (iBytesReceived == SOCKET_ERROR)
  {
    std::cerr << "No bytes sent" << WSAGetLastError() << std::endl;
  }

  return iBytesReceived;
}

int Socket::receive(char* cMessage)
{
  memset(cMessage, 0, sizeof(cMessage));
  int iLength = sizeof(cMessage);

  int iBytesReceived = ::recv(oSocket, (char*)&cMessage, iLength, 0);

  if (iBytesReceived == SOCKET_ERROR)
  {
    std::cerr << "No bytes sent" << WSAGetLastError() << std::endl;
  }

  return iBytesReceived;
}

void Socket::close()
{
  if (!bClosed)
  {
    closesocket(oSocket);
  }

  if (WSACleanup() == SOCKET_ERROR)
  {
    std::cerr << "WSASCleanup failed with error: " + WSAGetLastError() << std::endl;
  }
  else
  {
    std::cout << "Winsock successfully cleaned up" << std::endl;
  }
}