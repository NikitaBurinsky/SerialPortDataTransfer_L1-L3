#pragma once
#include <exception>
#include "COM_Port.h"
#include "L2_CNLayer.h"
#include "L3_CNLayer.h"

class UserComm {
public:
	UserComm(std::wstring& comName, ip4_addr ip);
private:

protected:
	COM_Port* L1;
	L2_CNLayer* L2;
	L3_CNLayer* L3;
};


