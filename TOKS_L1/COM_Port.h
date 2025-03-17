#pragma once
#include <string>
#include <exception>
#include <Windows.h>
#include <winbase.h>
#include <fileapi.h>
#include "DataStructs.h"

/// <summary>
/// L1 - Physical layer
/// </summary>

class COM_Port
{
public:
	bool Init(const std::wstring& port);
	void Disconnect();
	bool Write(const byte* data, int datasize);
	int Read(byte* data, int bufsize);
	~COM_Port() { Disconnect(); }

private:
	HANDLE m_Handler = nullptr;
	bool isInitialized = false;
	bool abortHandler()
	{
		CloseHandle(m_Handler);
		m_Handler = nullptr;
		throw std::exception("Serial Port Handler aborted");
	};
};

