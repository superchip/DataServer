#include "TCPDataServer.h"
#include <iostream>
	

using namespace std;

class MyCallbackHandler : public TCPDataServer::CallbackInterface
{
public:
	// The callback function that Caller will call.
	void OnNewData(char* message)
	{ 
		printf("Callback: Received new data: [%s]\n",message);
	}

};


int main()
{
	MyCallbackHandler* cb = new MyCallbackHandler();

	TCPDataServer server = TCPDataServer(11222,cb);
	
	Sleep(5000);

	char* m = server.dequeueIncomingData();
	printf("Dequeue data result: [%s]\n",m);

	char* m1 = server.dequeueIncomingData();
	printf("Dequeue new data result: [%s]\n",m1);

	// Wait here
	getchar();

	delete cb;
	return 0;
}