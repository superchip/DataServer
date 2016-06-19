#include <string.h>
#include <winsock2.h>
#include <thread>
#include <mutex>
#include <queue>

#pragma comment (lib, "ws2_32")

class TcpServer
{

public:
	
	/* Calling constructor will start the server */
	TcpServer(int);

	virtual ~TcpServer();
	
protected:

	static const int BUFFERSIZE = 512;
	// Listen to incoming connections
	int  start_listening();
	// Infinite loop for accepting connections
	virtual void acceptConnections();

	int    m_port;
	SOCKET m_sSock; // Server Socket
	SOCKET m_cSock; // Client Socket

	// Thread Handling
	std::thread		   m_processThread;
	std::mutex         m_mutex;




};