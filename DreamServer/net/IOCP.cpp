#include "IOCP.h"

bool loadSocketLib()
{
	WORD wVersionRequest = MAKEWORD(2, 2);
	WSADATA wsadata;
	DWORD result = WSAStartup(wVersionRequest, &wsadata);

	if (0 != result)
	{
		return false;
	}

	if (2 != LOBYTE(wsadata.wVersion) || 2 != HIBYTE(wsadata.wVersion))
	{
		WSACleanup();
		return false;
	}

	return true;
}

