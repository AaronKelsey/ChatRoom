
# ChatRoom

A simple Windows chat server which demonstrates the client-server model and socket programming.

## About

ChatRoom is a console-based chat server written in C++ for Windows environments. The client-server architecture is implemented using sockets  from the WinSock2 library to enable text-based communication using TCP.


## How to Run

Server:

To start the server, open a command prompt, run server.exe and provide the port as an argument.

    server.exe <Port>


Client:

To connect as a client, open a command prompt, run client.exe and provide the port and IP address to connect to the server.

    client.exe <IP> <Port>

**Note:** The server must be running before a client can connect.

## Planned Features

 - Client username
 - Multithreaded server connections
 - Multi-platform support for Windows and Linux

## License
MIT