
#pragma once 


#ifndef _IOCP_H__
#define _IOCP_H__

#include <WinSock2.h>
#include <Windows.h>
#include <vector>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Kernel32.lib")

const int DataBuffSize = 2 * 1024;

enum POST_TYPE
{
	PT_NULL = 0,
	PT_ACCEPT,
	PT_SEND,
	PT_RCV
};

typedef struct
{
	OVERLAPPED overLapped;
	WSABUF dataBuf;
	char buffer[DataBuffSize];
	int bufferLen;
	int opType;
}PER_IO_OP_DATA, *PPER_IO_OP_DATA, PER_IO_DATA, *PPER_IO_DATA;


typedef struct
{
	SOCKET socket;
	SOCKADDR_IN clientAddr;
}PER_HANDLE_DATA, *PPER_HANDLER_DATA;

const int DefaultPort = 6800;
extern std::vector<PPER_HANDLER_DATA> g_ClientGroup;
extern HANDLE g_Mutex;


//some functions
bool loadSocketLib();




#endif