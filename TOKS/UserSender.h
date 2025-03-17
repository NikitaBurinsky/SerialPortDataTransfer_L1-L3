#pragma once
#include "COM_Port.h"
#include "L2_CNLayer.h"
#include "L3_CNLayer.h"
#include <condition_variable>
#include "UserCommBase.h"
class CollisionGenerator;

class UserSender : public UserComm
{
public:
	UserSender(std::wstring& port, ip4_addr ip) : UserComm(port, ip), collisions(comName, &ab_CollisionWait) {};
	void startSender();
	void EnableCollisionGen(bool isEnable, std::string colisEscSym = "");
private:
	/// <summary>
	/// Creating collision of data packets. Creates a new thread with a new sender in it, to 
	/// send any packets in one time with User
	/// </summary>
	class CollisionGenerator {
	public:
		CollisionGenerator(std::wstring& comName, std::atomic_bool* cv);
		void CreateCollision();
		void RunCollisionReady();
		std::string colisEscSym;
	private:
		void StartSendingCollision();
		std::atomic_bool* ab_CollisionWait;
		L3_CNLayer L3 = L3_CNLayer(false);
	};
	bool IsCollisionsEnabled = false;
	std::wstring comName = L"COM1";
	CollisionGenerator collisions;
	std::atomic_bool ab_CollisionWait;
};
