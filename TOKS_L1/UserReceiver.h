#pragma once
#include "COM_Port.h"
#include "L2_CNLayer.h"
#include "L3_CNLayer.h"
#include "UserCommBase.h"

class UserReceiver : public UserComm
{
public:
	UserReceiver(std::wstring& comName, ip4_addr ip) : UserComm(comName, ip) {};
	void startReceiver();
};
