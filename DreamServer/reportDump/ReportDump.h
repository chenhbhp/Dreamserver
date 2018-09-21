#ifndef _REPORT_DUMP_H__
#define _REPORT_DUMP_H__


#include <windows.h>
#include <ImageHlp.h>

#pragma comment(lib, "dbghelp.lib")

inline BOOL IsDataSectionNeeded(const WCHAR* pModuleName)
{
	if (0 == pModuleName) return FALSE;

	WCHAR szFileName[_MAX_FNAME] = L"";
	_wsplitpath(pModuleName, NULL, NULL, szFileName, NULL);
	if (wcsicmp(szFileName, L"ntdll") == 0) return TRUE;
	return FALSE;
}

inline BOOL CALLBACK MiniDumpCallback(PVOID		pParam,
	const PMINIDUMP_CALLBACK_INPUT	pInput,
	PMINIDUMP_CALLBACK_OUTPUT		pOutput)
{
	if (NULL == pInput || NULL == pOutput) return FALSE;

	switch (pInput->CallbackType)
	{
	case ModuleCallback:
		if (pOutput->ModuleWriteFlags & ModuleWriteCodeSegs)
		if (!IsDataSectionNeeded(pInput->Module.FullPath))
			pOutput->ModuleWriteFlags &= (~ModuleWriteCodeSegs);

	case IncludeModuleCallback:
	case IncludeThreadCallback:
	case ThreadCallback:
	case ThreadExCallback:
		return TRUE;

	}

	return FALSE;
}

inline void CreateMiniDump(EXCEPTION_POINTERS* pep, LPCTSTR szFileName)
{
	HANDLE hFile = CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (NULL != hFile && INVALID_HANDLE_VALUE != hFile)
	{
		MINIDUMP_EXCEPTION_INFORMATION mdei;
		mdei.ThreadId = GetCurrentThreadId();
		mdei.ExceptionPointers = pep;
		mdei.ClientPointers = TRUE;

		MINIDUMP_CALLBACK_INFORMATION mdci;
		mdci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback;
		mdci.CallbackParam = NULL;

		MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)0x0000FFFF;
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile,
			MiniDumpNormal, &mdei, NULL, &mdci);

		CloseHandle(hFile);
	}
}

LPTOP_LEVEL_EXCEPTION_FILTER WINAPI MyDummySetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
	return NULL;
}

BOOL PreventSetUnhandledExceptionFilter()
{
	HMODULE hKernel32 = LoadLibrary(L"kernel32.dll");
	if (NULL == hKernel32) return FALSE;

	void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
	if (NULL == pOrgEntry) return FALSE;

	unsigned char newJump[100];
	DWORD dwOrgEntryAddr = (DWORD)pOrgEntry;
	dwOrgEntryAddr += 5;

	void *pNewFunc = &MyDummySetUnhandledExceptionFilter;
	DWORD dwNewEntryAddr = (DWORD)pNewFunc;
	DWORD dwRelativeAddr = dwNewEntryAddr - dwOrgEntryAddr;

	newJump[0] = 0xE9;
	memcpy(&newJump[1], &dwRelativeAddr, sizeof(pNewFunc));
	SIZE_T bytesWritten;
	BOOL bRet = WriteProcessMemory(GetCurrentProcess(), pOrgEntry, newJump,
		sizeof(pNewFunc)+1, &bytesWritten);

	return bRet;
}

LONG WINAPI UnhandledExceptionFilterEx(struct _EXCEPTION_POINTERS *pException)
{
	TCHAR szMbsFile[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, szMbsFile, MAX_PATH);
	TCHAR *pFind = wcsrchr(szMbsFile, '\\');
	if (pFind)
	{
		*(pFind + 1) = 0;
		wcscat(szMbsFile, L"CreateMiniDump.dmp");
		CreateMiniDump(pException, szMbsFile);
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

void RunCrashHandler()
{
	SetUnhandledExceptionFilter(UnhandledExceptionFilterEx);
	PreventSetUnhandledExceptionFilter();
}



#endif //_REPORT_DUMP_H__