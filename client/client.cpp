//
// Filename:    client.cpp 
// Description: This file contains the chat room client which connects to the server through TCP
// Author:      Aaron Kelsey
//

#include <iostream>
#include <string>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#include "socket.h"

#define no_init_all

int main(int argc, char* argv[])
{

  if (argc != 3)
  {
    std::cerr << "IP Address and Port not given" << std::endl;
    exit(0);
  }

  char* cServerAddress = argv[1];
  int iPort = std::stoi(argv[2]);

  Socket oClientSocket;

  if (oClientSocket.connect(iPort, cServerAddress) == SOCKET_ERROR)
  {
    std::cerr << "Error enabling socket for listening" << std::endl;
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

    if (sInput == "exit")
    {
      bytesSent += oClientSocket.send(sInput);
      if (bytesSent == SOCKET_ERROR)
      {
        std::cerr << "Send failed" << WSAGetLastError() << std::endl;
      }

      break;
    }

    bytesSent += oClientSocket.send(sInput);
    if (bytesSent == SOCKET_ERROR)
    {
      std::cerr << "Send failed" << WSAGetLastError() << std::endl;
    }

    std::cout << "Awaiting server response" << std::endl;

    std::string sBuffer = "";
    bytesReceived += oClientSocket.receive(sBuffer);

    if (bytesReceived == SOCKET_ERROR)
    {
      std::cerr << "No bytes received by the server" << WSAGetLastError() << std::endl;
    }
    else
    {
      if (sBuffer == "exit")
      {
        std::cout << "Client has disconnected from the session" << std::endl;
        break;
      }

      std::cout << "Server: " << sBuffer << std::endl;
    }
  }
  
  std::cout << "Bytes sent: " << bytesSent << std::endl;
  std::cout << "Bytes received: " << bytesReceived << std::endl;

  // CLose socket and clean up
  oClientSocket.close();

  std::cout << "Connection closed" << std::endl;
  return 0;

}
