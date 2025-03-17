#include "UserCommBase.h"

UserComm::UserComm(std::wstring& comName, ip4_addr ip)
{
	try {
		L1 = new COM_Port();
		L1->Init(comName);
		L2 = new L2_CNLayer();
		L2->Init(L1);
		L3 = new L3_CNLayer();
		L3->Init(L2, ip);
	}
	catch (std::exception& ex) {
		std::cout << ex.what();
		exit(-1);
	}
}

