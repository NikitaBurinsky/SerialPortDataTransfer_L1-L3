#pragma once
#include "COM_Port.h"
#include "DataStructs.h"

#define TIMEOUT 500
#define baudrate CBR_57600

bool COM_Port::Init(const std::wstring& port)
{
	if (isInitialized)
		return true;
	m_Handler = CreateFile(port.c_str(),//open port https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	
	if (m_Handler == (HANDLE)-1)
	{
		m_Handler = nullptr;
		return false;
	}

	SetupComm(m_Handler, 1500, 1500);//https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-setupcomm

	_COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = UINT_MAX;
	CommTimeOuts.ReadTotalTimeoutConstant = TIMEOUT;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = TIMEOUT;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;

	if (!SetCommTimeouts(m_Handler, &CommTimeOuts))
	{
		m_Handler = nullptr;
		return false;
	}

	_DCB ComDCM;

	memset(&ComDCM, 0, sizeof(ComDCM));
	ComDCM.DCBlength = sizeof(_DCB);
	GetCommState(m_Handler, &ComDCM);//Read in ComDCM
	ComDCM.BaudRate = DWORD(baudrate);
	ComDCM.ByteSize = 8;
	ComDCM.Parity = NOPARITY;
	ComDCM.StopBits = ONESTOPBIT;
	ComDCM.fAbortOnError = true;
	ComDCM.fDtrControl = DTR_CONTROL_DISABLE; //сброс бита

	ComDCM.fRtsControl = RTS_CONTROL_TOGGLE; //авто включение
	ComDCM.fBinary = true;
	ComDCM.fParity = false;
	ComDCM.fInX = ComDCM.fOutX = false;//?
	ComDCM.XonChar = false;//?
	ComDCM.XoffChar = 255;//?
	ComDCM.fErrorChar = false;
	ComDCM.fNull = false;
	ComDCM.fOutxCtsFlow = false;
	ComDCM.fOutxDsrFlow = false;
	ComDCM.XonLim = 128;//?
	ComDCM.XoffLim = 128;//?

	if (!SetCommState(m_Handler, &ComDCM))
		return abortHandler();
	isInitialized = true;
	return true;
}

void COM_Port::Disconnect()
{
	abortHandler();
}

bool COM_Port::Write(const byte* data, int datasize)
{
	if (!m_Handler)
		return false;

	DWORD feedback;
	if (!WriteFile(m_Handler, data, datasize, &feedback, 0) ||
		feedback != datasize)

	{ return abortHandler(); }
}

int COM_Port::Read(byte* buf, int bufsize)
{
	DWORD feedback = 1;
	int read = 0;
	
	while (bufsize > read && feedback != 0) 
	{
	if (!ReadFile(m_Handler, buf, bufsize, &feedback, NULL)) //? what is null
		return abortHandler();
	read += feedback;
	}
	return read;
}
