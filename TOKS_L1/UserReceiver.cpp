#include "UserReceiver.h"

void UserReceiver::startReceiver()
{
	while (true)
	{
		bufferData receivedData;
		receivedData = L3->receiveData();
		if (receivedData.size)
		{
			std::cout << "\nReceived message : ";
			for (int i = 0; i < receivedData.size; ++i)
				std::cout << receivedData.data[i];
		}
	}
}
