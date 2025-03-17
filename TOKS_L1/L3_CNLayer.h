#pragma once
#include "L2_CNLayer.h"
#include <bitset>
#define HLOG
#define NOISE

class L3_CNLayer //IPv4
{
	class HammingCoder;
public:
	L3_CNLayer(bool IsLog = true) : coder(IsLog) {};
	bool Init(L2_CNLayer* L2_Layer, ip4_addr ip4);
	void sendData(byte* payload, int plength);
	bufferData receiveData();
	ip4_addr to_ip4_addr(std::string ip);

private:
	packetData formPacket(byte* l3Data, int datalength);
	short calcHeaderCheckSumm(packetData::header* header);
	bufferData decapsPacket(bufferData packet);

	//Components
	L2_CNLayer* L2;
	ip4_addr local_addr;

private:
	class HammingCoder {
	public:
		HammingCoder(bool IsLog) : isLog(IsLog) {};
		bufferData HEncodePayload(bufferData payload);
		bufferData HDecodePayload(bufferData payload);

		static void invertRandomBit(bufferData data);
	private:
		bool isLog;
		static void printHCodedSymbol(byte data);

		byte encodeWord(byte inf);

		byte decodeWord(byte word);//-iiikikk


	};
	HammingCoder coder;
};

