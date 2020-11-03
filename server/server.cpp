//
// Filename:    server.cpp 
// Description: This file contains the chat room server which clients can connect to through TCP
// Author:      Aaron Kelsey
//


#include <iostream>
#include <string>
#include <WinSock2.h>

int cleanupWSA();

int main(int argc, char* argv[])
{

  if (argc != 2)
  {
    std::cerr << "Port not given" << std::endl;
    exit(0);
  }

  int iPort = std::stoi(argv[1]);
  char cMsg[1000];

  WSADATA oWsaData;
  int     oNret;

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
  SOCKET oListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (oListeningSocket == INVALID_SOCKET)
  {
	  std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
    cleanupWSA();
  }

  // Address information
  SOCKADDR_IN oServerInfo;
  oServerInfo.sin_family        = AF_INET;
  oServerInfo.sin_addr.s_addr   = INADDR_ANY; // Listen for any connection
  oServerInfo.sin_port          = htons(iPort); // Convert port to network-byte order

  // Bind the socket to local server address
  oNret = bind(oListeningSocket, (LPSOCKADDR)&oServerInfo, sizeof(struct sockaddr));

  if (oNret == SOCKET_ERROR)
  {
    std::cerr << "Error binding socket: " << WSAGetLastError() << std::endl;
    cleanupWSA();
  }

  // Allow socket to listen
  oNret = listen(oListeningSocket, 10); // Up to 10 waiting connections to be accepted

  if (oNret == SOCKET_ERROR)
  {
    std::cerr << "Error enabling socket for listening: " << WSAGetLastError() << std::endl;
    cleanupWSA();
  }

  SOCKET oClientSocket;
  oClientSocket = accept(oListeningSocket, NULL, NULL);

  if (oClientSocket == INVALID_SOCKET)
  {
    std::cerr << "Error waiting for client: " << WSAGetLastError() << std::endl;
    cleanupWSA();
  }

  int bytesReceived = 0;
  int bytesSent = 0;

  // Send and receive from the client
  while (true)
  {
    std::cout << "Awaiting client response..." << std::endl;

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

      std::cout << "Client: " << cMsg << std::endl;
    }
    
    std::cout << ">:";

    std::string sInput;
    std::getline(std::cin, sInput);
    memset(&cMsg, 0, sizeof(cMsg));
    strcpy_s(cMsg, sInput.c_str());

    if (sInput == "exit")
    {
      bytesSent += send(oClientSocket, (char*)&cMsg, static_cast<int>(strlen(cMsg)), 0);
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
  }

  std::cout << "Bytes sent: " << bytesSent << std::endl;
  std::cout << "Bytes received: " << bytesReceived << std::endl;

  closesocket(oListeningSocket);
  closesocket(oClientSocket);

  // Shutdown winsock
  cleanupWSA();

  std::cout << "Connection closed." << std::endl;
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