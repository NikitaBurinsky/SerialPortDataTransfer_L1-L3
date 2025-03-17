#pragma once
#include <iostream>
#include <string>
#include "L3_CNLayer.h"
#include "UserSender.h"
#include "UserReceiver.h"
//-gc - generate collisions


void main() 
{
	std::string choise;
	std::wstring portId;
	std::cout << "Enter serial port\n -";
	std::wcin >> portId;
	std::cout << "\nChoose con type : sender, receiver\n -";
	std::cin >> choise;
	std::cout << std::endl;

	if (choise.find("sender") != -1)
	{
		UserSender sender(portId, ip4_addr(255,255,255,255));
	if (choise.find("-gc") != -1)
			sender.EnableCollisionGen(true, "-c");
		sender.startSender();
	}
	else if (choise.find("receiver") != -1)
	{
		UserReceiver receiver(portId, ip4_addr(255, 255, 255, 255));
		receiver.startReceiver();
	}
}

