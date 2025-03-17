#pragma once
#include "COM_Port.h"
#include "DataStructs.h"

/// <summary>
/// Data link layer
/// </summary>
class L2_CNLayer //PPP
{
	public:
		bool Init(COM_Port* L1_Layer, bool IsLog = true);
		void sendData(byte* payload, int plength);
		bufferData receiveData();

	private:
		bufferData setESCs(byte* data, int datalength);
		frameData formFrame(byte* l3Data, int datalength);
		short calcCheckSumm(byte* data, int datalenght);
		//Components
		COM_Port* L1;
		bool isLog = true;
};

