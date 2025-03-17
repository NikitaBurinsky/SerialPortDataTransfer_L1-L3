#include "L2_CNLayer.h"
#include <bitset>
#include <iostream>

void printPacketFrame(bufferData frame) {
  std::cout << "\n\n received frame length : " << frame.size;
  auto frameHead = (frameData::header*)frame.data;
  auto packetHead =
      (packetData::header*)(frame.data + sizeof(frameData::header));
  auto frameTrailer = (frameData::trailer*)(frame.data + frame.size - 3);
  std::cout << "\n ~ frame header";
  std::cout << "\n\t flag    - " << std::bitset<8>(frameHead->sflag);
  std::cout << "\n\t address - " << std::bitset<8>(frameHead->address);
  std::cout << "\n\t control - " << std::bitset<8>(frameHead->control);
  std::cout << "\n\t protocol- " << std::bitset<8>(frameHead->protocol);

  std::cout << "\n\n ~ ~ packet header";
  std::cout << "\n\t\t version|ihl - "
            << std::bitset<8>(packetHead->version_ihl);
  std::cout << "\n\t\t typeOfServ  - "
            << std::bitset<8>(packetHead->diff_services);
  std::cout << "\n\t\t totalLengtn - "
            << std::bitset<16>(packetHead->total_length);
  std::cout << "\n\t\t identificat - "
            << std::bitset<16>(packetHead->identification);
  std::cout << "\n\t\t e_df_mf_fOfs- "
            << std::bitset<16>(packetHead->e_df_mf_fragmOffset);
  std::cout << "\n\t\t timeToLive  - " << std::bitset<8>(packetHead->ttl);
  std::cout << "\n\t\t protocol    - " << std::bitset<8>(packetHead->protocol);
  std::cout << "\n\t\t head checsum- "
            << std::bitset<16>(packetHead->head_csumm);
  std::cout << "\n\t\t source addr - ";
  packetHead->source_addr.print();
  std::cout << "\n\t\t destin addr - ";
  packetHead->dest_addr.print();

  std::cout << "\n\n ~ frame trailer";
  std::cout << "\n\t checsum - " << std::bitset<16>(frameTrailer->checksum);
  std::cout << "\n\t flag    - " << std::bitset<8>(frameTrailer->eflag);
  std::cout << "\n";
}

bool L2_CNLayer::Init(COM_Port* Inited_L1_Layer, bool IsLog) {
  isLog = IsLog;
  L1 = Inited_L1_Layer;
  return true;
}

void L2_CNLayer::sendData(byte* payload, int plength) {
  frameData fdata = formFrame(payload, plength);
  if (!L1->Write(fdata.frame, fdata.frameSize)) {
    std::cout << "\nl2 - send data - write error";
    exit(0);
  }
}

bufferData L2_CNLayer::receiveData()  // receive packer
{
  static const byte flag = 0b01111110;
  static const byte esc = 0b01111101;

  byte bbuf;
  while (!L1->Read(&bbuf, 1)) {}
  if (bbuf != flag) {
    std::cout << "\nl2 - receiveData - start frame not found";
    exit(0);
  }
  if (!L1->Read(&bbuf, 1)) {
    std::cout << "\nl2 - receiveData - read error";
    exit(0);
  }

  bufferData buf;
  buf.push_back(flag);
  while (bbuf != flag)  //+-
  {
    if (bbuf == esc) {
      L1->Read(&bbuf, 1);
    }
    buf.push_back(bbuf);
    L1->Read(&bbuf, 1);
  }
  buf.push_back(bbuf);
  if (isLog)
  printPacketFrame(buf);

  int packetsize =
      buf.size - sizeof(frameData::header) - sizeof(frameData::trailer) + 1;

  byte* payload = (byte*)malloc(packetsize);
  memcpy(payload, buf.data + sizeof(frameData::header), packetsize);

  // free(buf.data);
  buf.data = payload;
  buf.size = packetsize;

  return buf;
}

bufferData L2_CNLayer::setESCs(byte* data, int datalength) {
  bufferData newData;
  for (int i = 0; i < datalength; ++i) {
    if (data[i] == 0b01111110 || data[i] == 0b01111101)
      newData.push_back(0b01111101);
    newData.push_back(data[i]);
  }
  free(data);
  return newData;
}

frameData L2_CNLayer::formFrame(byte* l3Data, int datalengt) {
  bufferData bd = setESCs(l3Data, datalengt);
  l3Data = bd.data;
  datalengt = bd.size;
  frameData frame;
  frame.frameSize =
      datalengt - 1 + sizeof(frameData::header) + sizeof(frameData::trailer);
  frame.frame = (byte*)malloc(frame.frameSize);
  memcpy(frame.frame + sizeof(frameData::header), l3Data, datalengt);
  frameData::header* head = (frameData::header*)frame.frame;
  frameData::trailer* trailer =
      (frameData::trailer*)(frame.frame + datalengt +
                            sizeof(frameData::header));

  // Init frame head and trailer
  head->sflag = 0b01111110;
  head->address = 0b11111111;   // for all stations
  head->control = 0b00000011;   // non numerated frame
  head->protocol = 0b00000000;  // starts with 0 => ipv4 or ipv6

  trailer->checksum = calcCheckSumm(frame.frame, frame.frameSize);
  trailer->eflag = 0b01111110;
  return frame;
}

short L2_CNLayer::calcCheckSumm(byte* data, int datalenght) {
  short cs = 0;
  for (int i = 0; i < datalenght; ++i) {
    cs += (short)data[i];
  }
  return cs;
}
