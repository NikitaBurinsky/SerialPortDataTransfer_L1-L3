#pragma once
#include <Windows.h>
#include "L3_CNLayer.h"
#include "L2_CNLayer.h"

bool L3_CNLayer::Init(L2_CNLayer* L2_Layer, ip4_addr ip)
{
	L2 = L2_Layer;
	local_addr = ip;
    return true;
}


void L3_CNLayer::sendData(byte* payload, int plength)
{
	packetData packet = formPacket(payload, plength);
	L2->sendData(packet.packet, packet.packetSize);
}

bufferData L3_CNLayer::receiveData()
{
	bufferData receivedPacket = L2->receiveData();
	bufferData purData = decapsPacket(receivedPacket);
	return purData;
}

packetData L3_CNLayer::formPacket(byte* payload, int datalength)//options = 0
{
	bufferData bpayload = coder.HEncodePayload(bufferData(payload, datalength));
	payload = bpayload.data;
	datalength = bpayload.size;

	packetData packet;
	packet.packetSize = datalength + sizeof(packetData::header); //без options
	packet.packet = (byte*)malloc(packet.packetSize);
	memcpy(packet.packet + sizeof(packetData::header), payload, datalength);
	auto header = (packetData::header*)packet.packet;
	
	header->version_ihl = 0b01000101;//? код ip4
	header->diff_services = 0b00000000;//!? потом поискать стандарт для ds
	header->total_length = packet.packetSize;
	header->identification = 0;
	header->e_df_mf_fragmOffset = 0;
	header->ttl = 0;
	header->protocol = 6;//tcp
	header->head_csumm = calcHeaderCheckSumm(header);

	header->source_addr = local_addr;

	ip4_addr destin_addr(255, 255, 255, 255);
	header->dest_addr = destin_addr;
	return packet;
}

short L3_CNLayer::calcHeaderCheckSumm(packetData::header* head)
{
	head->head_csumm = 0;
	short summ = 0;
	byte* head_bytes = (byte*)head;
	for (int i = 0; i < head->total_length; ++i)
		summ += head_bytes[i];
	return summ;
}

bufferData L3_CNLayer::decapsPacket(bufferData packet)
{
	packetData::header* head = (packetData::header*)packet.data;
	int datalength = head->total_length - sizeof(packetData::header);
	bufferData result;
	result.data = (byte*)malloc(datalength);
	memcpy(result.data, packet.data + sizeof(packetData::header), datalength);
	result.size = datalength;
	free(packet.data);

	result = coder.HDecodePayload(result);
	return result;
}

