#include "TcpServer.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <thread> 


TcpServer::TcpServer(int port)
{
    TcpServer::m_port = port;
}

TcpServer::~TcpServer()
{
	// Terminate incoming connection thread
	m_processThread.detach();
}

int TcpServer::start_listening()
{
    /* WSAStartup() initializes the program to call winsock2.dll */
    WORD wVersionRequested;
    WSADATA wsaData;
    int wsaerr;
    wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaerr != 0)
    {
        printf("Server: The Winsock dll not found!\n");
        WSACleanup();
        return 0;
    }

    /* SOCKET is simply a UINT, created because
	   on Unix sockets are file descriptors(UINT) but not in windows
	   so new type SOCKET was created 
	*/
    m_sSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(m_sSock == INVALID_SOCKET)
    {
        printf("Server: Error initializing socket!n");
        WSACleanup();
        return 0;
    }

    /* The SOCKADDR_IN structure is used by
	   Win Sockets to specify an endpoint address
       to which the socket connects 
	*/
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_port = htons(m_port);
    service.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* bind just links the socket
	   with the sockaddr_in struct we initialized 
	*/
    if(bind(m_sSock,(SOCKADDR*)&service,sizeof(service))==SOCKET_ERROR)
    {
        printf("Server: Error binding socket to port\n");
        WSACleanup();
        return 0;
    }

    /* wait for incoming connections */
    if(listen(m_sSock,10)==SOCKET_ERROR)
        printf("listen(): Error listening on socket %d.\n", WSAGetLastError());
    else
    {
        printf("Server: Waiting for connections on port [%d]\n" , m_port);
    }
    printf("Server: Waiting for a client to connect...\n\n");
    
    return 0;
}

void TcpServer::acceptConnections()
{
	sockaddr_in from;
	int fromlen=sizeof(from);

	while (true)
	{

		char* temp = new char[BUFFERSIZE];
		const char* sendMessege = "OK";

		m_cSock = accept(m_sSock,(SOCKADDR*)&from,&fromlen);

		recv(m_cSock, temp, BUFFERSIZE, 0);

		// Send OK Message
		send(m_cSock,sendMessege,strlen(sendMessege),0);

		std::cout << "Connection from " << inet_ntoa(from.sin_addr) <<" [OK Sent] \n";

		closesocket(m_cSock);

	}
	closesocket(m_sSock);
	WSACleanup();
	return;
}




