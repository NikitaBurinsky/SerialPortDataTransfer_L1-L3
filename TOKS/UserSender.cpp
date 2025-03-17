#include "UserSender.h"
#include <string>
#include <iostream>
#include <thread>
#include <atomic>
#include <shared_mutex>
#define mtErrLog(x) mt.lock(); std::cout << "!MTERR - " << x << " -!"; mt.unlock(); 


void UserSender::startSender()
{
	std::mutex mt;
	if (IsCollisionsEnabled)
		collisions.RunCollisionReady();
	while (true)
	{
		mt.lock();
		std::string data;
		std::cout << "\nSend message : ";
		std::cin >> data;
		mt.unlock();
		if (IsCollisionsEnabled && data.find(collisions.colisEscSym) != -1)
			collisions.CreateCollision();
		L3->sendData((byte*)data.c_str(), data.size());
		std::cout << "\nMessage sent\n";
	}
}

void UserSender::EnableCollisionGen(bool isEnable, std::string colisEscSym)
{
	IsCollisionsEnabled = isEnable;
	if (isEnable) {
		collisions.colisEscSym = colisEscSym;
	}
}

//Collision Generator

UserSender::CollisionGenerator::CollisionGenerator(std::wstring& comName, std::atomic_bool* cv)
{
	COM_Port* L1 = new COM_Port();
	if (!L1->Init(comName))
		exit(-2);
	L2_CNLayer* L2 = new L2_CNLayer();
	if (!L2->Init(L1, false))
		exit(-3);
	if(!L3.Init(L2, ip4_addr(0,0,0,0)))
		exit(-4);
	ab_CollisionWait = cv;
}

void UserSender::CollisionGenerator::CreateCollision()
{
	ab_CollisionWait->store(true);
}

void UserSender::CollisionGenerator::RunCollisionReady()
{
	std::thread cThread(&UserSender::CollisionGenerator::StartSendingCollision, this);
	cThread.detach();
}

void UserSender::CollisionGenerator::StartSendingCollision()
{
	std::mutex mt;
	while (true) {
		while (!ab_CollisionWait->load()) {};
		L3.sendData((byte*)L"GENERATED COLLISION", 20);
		ab_CollisionWait->store(false);
	}
	mtErrLog("CollisionGenerator - END");
}
