#pragma once
#include <iostream>
typedef unsigned char byte;

struct bufferData
{
	byte* data = nullptr; 
	int size = 0;
	bufferData() {};
	bufferData(byte* d, int s) : data(d), size(s) {};

	void push_back(byte b)
	{
		if (data != nullptr)
			data = (byte*)realloc(data, ++size);
		else
		{
			data = (byte*)malloc(1);
			size = 1;
		}
		data[size - 1] = b;
	}
};

union ip4_addr
{
	struct {
		byte p1;
		byte p2;
		byte p3;
		byte p4;
	};
	void print()
	{
		std::cout << (short)p1 << '.';
		std::cout << (short)p2 << '.';
		std::cout << (short)p3 << '.';
		std::cout << (short)p4;
	}

	ip4_addr() {
		p1 = 0;
		p2 = 0;
		p3 = 0;
		p4 = 0;
	};
	ip4_addr(byte _p1, byte _p2, byte _p3, byte _p4) {
		p1 = _p1;
		p2 = _p2;
		p3 = _p3;
		p4 = _p4;
	}

	uint32_t total;
};

struct packetData
{
	byte* packet;
	int packetSize;
	
	struct header{
		byte version_ihl;
		byte diff_services;
		short total_length;
		short identification;
		short e_df_mf_fragmOffset;
		byte ttl;
		byte protocol;
		short head_csumm;
		ip4_addr source_addr;
		ip4_addr dest_addr;
	};
};

struct frameData {
	struct header {
		byte sflag;
		byte address;
		byte control;
		byte protocol;//1 or 2
	};
	struct trailer {
		short checksum; //2 or 4
		byte eflag;
	};
	byte* frame;
	int frameSize;
};
