#pragma once
#include "L3_CNLayer.h"
#include "DataStructs.h"

bufferData L3_CNLayer::HammingCoder::HEncodePayload(bufferData payload)
{
	//info = 4, coded = 7, coding = 3
	bufferData codedData;
	codedData.size = payload.size * 2;
	codedData.data = (byte*)malloc(codedData.size);
	int codedInd = 0;
#ifdef HLOG
	if (isLog) {
		std::cout << "\n\nHamming encoding : \n\tsource data:\n\t\t|";
		for (int i = 0; i < payload.size; ++i)
			std::cout << "    " << std::bitset<8>(payload.data[i]) << "      |";
	}
#endif
	for (int i = 0; i < payload.size; ++i)
	{
		codedData.data[codedInd] = encodeWord(payload.data[i] >> 4);
		++codedInd;
		codedData.data[codedInd] = encodeWord(payload.data[i]);
		++codedInd;
	}
#ifdef HLOG
	if (isLog) {
		std::cout << "\n\tencoded data:\n\t\t|";
		for (int i = 0; i < codedData.size; i += 2)
			std::cout << std::bitset<8>(codedData.data[i]) << " " << std::bitset<8>(codedData.data[i + 1]) << " |";
	}
#endif
#ifdef NOISE
	std::cout << "\n\tnoise generator:\n\t\t";
	HammingCoder::invertRandomBit(codedData);
#endif

	return codedData;
};

bufferData L3_CNLayer::HammingCoder::HDecodePayload(bufferData payload)
{
	bufferData decodedData;
	decodedData.size = payload.size / 2;
	decodedData.data = (byte*)malloc(decodedData.size);
	int decodingWord = 0;
	for (int i = 0; i < decodedData.size; ++i)
	{
		decodedData.data[i] = (decodeWord(payload.data[decodingWord]) << 4) & 0b11110000;
		++decodingWord;
		decodedData.data[i] |= decodeWord(payload.data[decodingWord]) & 0b00001111;
		++decodingWord;
	}

	free(payload.data);
	return decodedData;
};
void L3_CNLayer::HammingCoder::invertRandomBit(bufferData data)
{
	srand(time(NULL));
	int randomByteIndex = rand() % data.size;
	int randomBitIndex = rand() % 7;
	data.data[randomByteIndex] ^= (1 << randomBitIndex);
	std::cout << "Inverted bit " << randomBitIndex + 1 << " in byte " << randomByteIndex + 1;
}

void L3_CNLayer::HammingCoder::printHCodedSymbol(byte data)
{
	std::cout << (int)((data & 0b00000001)) << ". ";
	std::cout << (int)((data & 0b00000001) >> 1) << ". ";
	std::cout << (int)((data & 0b00000001) >> 2) << " ";
	std::cout << (int)((data & 0b00000001) >> 3) << ". ";
	std::cout << (int)((data & 0b00000001) >> 4) << " ";
	std::cout << (int)((data & 0b00000001) >> 5) << " ";
	std::cout << (int)((data & 0b00000001) >> 6) << " ";
	std::cout << (int)((data & 0b00000001) >> 7) << " ";
}

byte L3_CNLayer::HammingCoder::encodeWord(byte inf)
{
	byte coded = 0;
	coded |= (inf & 0b0000001) << 2;
	coded |= (inf & 0b0001110) << 3;

	//calc k1
	byte xorsum = (inf) ^ (inf >> 1) ^ (inf >> 3);
	coded |= xorsum & 0b00000001;
	//calc k2
	xorsum = (inf) ^ (inf >> 2) ^ (inf >> 3);
	coded |= (xorsum << 1) & 0b00000010;
	//calc k3
	xorsum = (inf >> 1) ^ (inf >> 2) ^ (inf >> 3);
	coded |= (xorsum << 3) & 0b00001000;
	return coded;
}

byte L3_CNLayer::HammingCoder::decodeWord(byte word)
{
	uint8_t errInd = 0;
	byte xorsum = (word >> 2) ^ (word >> 4) ^ (word >> 6);
	if ((word & 1) != (xorsum & 1))
		errInd += 1;

	xorsum = (word >> 2) ^ (word >> 5) ^ (word >> 6);
	if (((word >> 1) & 1) != (xorsum & 1))
		errInd += 2;

	xorsum = (word >> 4) ^ (word >> 5) ^ (word >> 6);
	if (((word >> 3) & 1) != (xorsum & 1))
		errInd += 4;

	if (errInd) {
#ifdef NOISE
		if (isLog)
			std::cout << "\n\thamming code fixed bit : " << (int)errInd - 1 << " | in coded word : " << std::bitset<8>(word);
#endif
		word ^= (1 << --errInd);
	}


	byte result = 0;
	result |= (word & 0b00000100) >> 2;
	result |= (word & 0b01110000) >> 3;
	return result;
}