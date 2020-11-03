//
// Filename:    client.cpp 
// Description: This file contains the chat room client which connects to the server through TCP
// Author:      Aaron Kelsey
//


#include <iostream>
#include <string>
#include <WinSock2.h>
#include <Ws2tcpip.h>

int cleanupWSA();

int main(int argc, char* argv[])
{

  if (argc != 3)
  {
    std::cerr << "IP Address and Port not given" << std::endl;
    exit(0);
  }

  char* cServerAddress = argv[1];
  int iPort = std::stoi(argv[2]);
  char cMsg[1000];

  WSADATA oWsaData;
  int     oNret;
  SOCKADDR_IN oServerInfo;

  // Initialise WinSock2 with version 2.2
  if (WSAStartup(MAKEWORD(2, 2), &oWsaData) != 0)
  {
    std::cerr << "WSAStartup failed with error: " + WSAGetLastError() << std::endl;
    return 0;
  }
  else
  {
    std::cout << "Winsock successfully found" << std::endl;
  }

  // Create listening socket
  SOCKET oClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (oClientSocket == INVALID_SOCKET)
  {
    std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
    cleanupWSA();
  }

  InetPton(AF_INET, cServerAddress, &oServerInfo.sin_addr.s_addr);

  // Address information
  oServerInfo.sin_family = AF_INET;
  oServerInfo.sin_port   = htons(iPort); // Convert port to network-byte order

  // Connect to server
  oNret = connect(oClientSocket, (sockaddr*)&oServerInfo, sizeof(oServerInfo));

  if (oNret == SOCKET_ERROR)
  {
    std::cerr << "Error connecting to server: " << WSAGetLastError() << std::endl;
    cleanupWSA();
  }

  std::cout << "Connected to server" << std::endl;

  int bytesReceived = 0;
  int bytesSent = 0;

  // Send and receive from the client
  while (true)
  {
    std::cout << ">: ";

    std::string sInput;
    std::getline(std::cin, sInput);
    memset(&cMsg, 0, sizeof(cMsg));
    strcpy_s(cMsg, sInput.c_str());

    if (sInput == "exit")
    {
      bytesSent = send(oClientSocket, (char*)&cMsg, static_cast<int>(strlen(cMsg)), 0);
      if (bytesSent == SOCKET_ERROR)
      {
        std::cerr << "Send failed" << WSAGetLastError() << std::endl;
      }

      break;
    }

    bytesSent += send(oClientSocket, (char*)&cMsg, static_cast<int>(strlen(cMsg)), 0);
    if (bytesSent == SOCKET_ERROR)
    {
      std::cerr << "Send failed" << WSAGetLastError() << std::endl;
    }

    std::cout << "Awaiting server response" << std::endl;

    memset(&cMsg, 0, sizeof(cMsg));
    bytesReceived += recv(oClientSocket, (char*)&cMsg, sizeof(cMsg), 0);

    if (bytesReceived == SOCKET_ERROR)
    {
      std::cerr << "No bytes received by the server" << WSAGetLastError() << std::endl;
    }
    else
    {
      if (!strcmp(cMsg, "exit"))
      {
        std::cout << "Client has disconnected from the session" << std::endl;
        break;
      }

      std::cout << "Server: " << cMsg << std::endl;
    }
  }
  
  std::cout << "Bytes sent: " << bytesSent << std::endl;
  std::cout << "Bytes received: " << bytesReceived << std::endl;

  closesocket(oClientSocket);

  // Shutdown winsock
  cleanupWSA();

  std::cout << "Connection closed" << std::endl;
  return 0;

}

int cleanupWSA()
{
  // Cleanup 
  if (WSACleanup() == SOCKET_ERROR)
  {
    std::cerr << "WSASCleanup failed with error: " + WSAGetLastError() << std::endl;
  }
  else
  {
    std::cout << "Winsock successfully cleaned up" << std::endl;
  }

  return 0;
}