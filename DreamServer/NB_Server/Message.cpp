#include <iostream>
#include "Message.h"

DWORD WINAPI ServerWorkThread(LPVOID completionPort)
{
	HANDLE CompletionPort = (HANDLE)completionPort;
	DWORD BytesTransferred;
	LPOVERLAPPED IpOverlapped;
	PPER_HANDLER_DATA PerHandleData = nullptr;
	PPER_IO_DATA PerIoData = nullptr;

	DWORD Flags = 0;
	BOOL bResult = false;

	while (true)
	{
		bResult = GetQueuedCompletionStatus(
			CompletionPort,
			&BytesTransferred,
			(PULONG_PTR)&PerHandleData,
			(LPOVERLAPPED*)&IpOverlapped,
			INFINITE
			);

		if (0 == bResult)
		{
			std::cerr << "GetCompletionStatus Error: " << GetLastError() << std::endl;
			return -1;
		}

		PerIoData = CONTAINING_RECORD(IpOverlapped, PER_IO_DATA, overLapped);

		if (0 == BytesTransferred)
		{
			closesocket(PerHandleData->socket);
			GlobalFree(PerHandleData);
			GlobalFree(PerIoData);
			continue;
		}

		//handle data
		WaitForSingleObject(g_Mutex, INFINITE);
		std::cout << "Client sends: " << PerIoData->dataBuf.buf << std::endl;
		ReleaseMutex(g_Mutex);

		memset(&(PerIoData->overLapped), 0, sizeof(OVERLAPPED));
		PerIoData->dataBuf.len = 1024;
		PerIoData->dataBuf.buf = PerIoData->buffer;
		PerIoData->opType = 0;
		/*WSARecv(
			PerHandleData->socket,
			&(PerIoData->dataBuf),
			1,
			&RecvBytes,
			&(PerIoData->overLapped),
			nullptr
			);*/

	}

	return 0;
}

bool startServer()
{
	if (!loadSocketLib())
	{
		return false;
	}

	HANDLE compltetionPort = CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		NULL,
		0,
		0
		);

	//create thread pool
	SYSTEM_INFO tmpSysInfo;
	GetSystemInfo(&tmpSysInfo);

	for (DWORD i = 0; i < tmpSysInfo.dwNumberOfProcessors * 2;++i)
	{
		HANDLE ThreadHandle = ::CreateThread(
			NULL,
			0,
			ServerWorkThread,
			compltetionPort,
			0,
			NULL
			);

		CloseHandle(ThreadHandle);
	}

	//create server socket
	SOCKET srvSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN srvAddr;
	srvAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(DefaultPort);

	int bindRes = bind(srvSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR));
	if (SOCKET_ERROR == bindRes)
	{
		return false;
	}

	int listenRes = listen(srvSocket, 10);
	if (SOCKET_ERROR == listenRes)
	{
		return false;
	}



	return true;
}