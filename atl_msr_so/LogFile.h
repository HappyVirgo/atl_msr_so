#pragma once

#include <Windows.h>

class CLogFile
{
public:
	CLogFile(void);
	~CLogFile(void);
	int OpenLogFile (wchar_t *pFilePath);
	int OpenLogFileIfClosed (wchar_t *pFilePath);
	int PrintLog (wchar_t *pLogLine);

private:
	HANDLE  m_hFile;
	DWORD   m_dwError;
};
