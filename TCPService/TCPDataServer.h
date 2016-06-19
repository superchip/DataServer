#include "TcpServer.h"

class TCPDataServer : public TcpServer
{
public:
	/* Callback interface to handle server incoming data */
	class CallbackInterface
	{
	public:
		virtual void OnNewData(char* message) = 0;
	};

	/* Calling constructor will start the server */
	TCPDataServer(int,CallbackInterface*);

	virtual ~TCPDataServer();

	char* dequeueIncomingData();

private:
	// Process the incoming data
	void processData(char* data);

	virtual void acceptConnections();

	// Callback for each new data received
	CallbackInterface* m_cb;

	// All incoming messages will be saved in this queue
	std::queue<char*>  m_messageQueue;
};