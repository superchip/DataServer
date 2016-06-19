#include "TCPDataServer.h"

#include <iostream>
 
TCPDataServer::TCPDataServer(int port, CallbackInterface* callBack) : TcpServer(port)
{
	TCPDataServer::m_cb = callBack;

	TcpServer::start_listening();

	/* accept connections thread */
	m_processThread  = std::thread(&TCPDataServer::acceptConnections, this);
}

TCPDataServer::~TCPDataServer()
{
	while(m_messageQueue.size() != 0)
	{
		delete [] m_messageQueue.front();
		m_messageQueue.pop();
	}
}

void TCPDataServer::acceptConnections()
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

		// Handle Data
		processData(temp);

		closesocket(m_cSock);

    }
    closesocket(m_sSock);
    WSACleanup();
    return;
}

char* TCPDataServer::dequeueIncomingData()
{
	// Keep data intact
	std::lock_guard<std::mutex> guard(m_mutex);
	if(m_messageQueue.size() == 0)
		return NULL;

	// Get first data input and dequeue
	char* temp = m_messageQueue.front();
	m_messageQueue.pop();
	return temp;
	 
}

void TCPDataServer::processData(char* data)
{
	// Find ~ and replace with null terminated
	for(int i = 0 ; i < BUFFERSIZE ; i++)
	{
		if(data[i] == '~')
			data[i] = '\0';
	}

	// Enqueue data
	m_messageQueue.push(data);

	// Call call back function with new data
	m_cb->OnNewData(data);
	
}
