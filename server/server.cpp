//
// Filename:    server.cpp 
// Description: This file contains the chat room server which clients can connect to through TCP
// Author:      Aaron Kelsey
//

#include <iostream>
#include <string>
#include <WinSock2.h>

#include "socket.h"

#define no_init_all

int main(int argc, char* argv[])
{

  if (argc != 2)
  {
    std::cerr << "Port not given" << std::endl;
    exit(0);
  }

  int iPort = std::stoi(argv[1]);

  Socket oListeningSocket;

  if (oListeningSocket.bind(iPort) == SOCKET_ERROR)
  {
    std::cerr << "Binding failed" << std::endl;

  }

  if (oListeningSocket.listen(10) == SOCKET_ERROR)
  {
    std::cerr << "Error enabling socket for listening" << std::endl;
  }

  std::shared_ptr<Socket> oClientSocket = oListeningSocket.accept();

  if (oClientSocket->getSocket() == INVALID_SOCKET)
  {
    std::cerr << "Error accepting connection" << std::endl;
  }

  int bytesReceived = 0;
  int bytesSent = 0;

  // Send and receive from the client
  while (true)
  {
    std::cout << "Awaiting client response..." << std::endl;

    if (oClientSocket == nullptr)
    {
      std::cerr << "client socket null!" << std::endl;
    }

    std::string sBuffer = "";
    bytesReceived += oClientSocket->receive(sBuffer);

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

      std::cout << "Client: " << sBuffer << std::endl;
    }
    
    std::cout << ">:";

    std::string sInput;
    std::getline(std::cin, sInput);

    if (sInput == "exit")
    {
      bytesSent += oClientSocket->send(sInput);

      if (bytesSent == SOCKET_ERROR)
      {
        std::cerr << "Send failed" << WSAGetLastError() << std::endl;
      }

      break;
    }

    bytesSent += oClientSocket->send(sInput);

    if (bytesSent == SOCKET_ERROR)
    {
      std::cerr << "Send failed" << WSAGetLastError() << std::endl;
    }
  }

  std::cout << "Bytes sent: " << bytesSent << std::endl;
  std::cout << "Bytes received: " << bytesReceived << std::endl;

  // Close socket and clean up
  oListeningSocket.close();
  oClientSocket->close();

  std::cout << "Connection closed." << std::endl;
  return 0;
}
