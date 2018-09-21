
#pragma once

#ifndef _MESSAGE_H__
#define _MESSAGE_H__

#include "../net/IOCP.h"

DWORD WINAPI ServerWorkThread(LPVOID completionPort);
bool startServer();


#endif