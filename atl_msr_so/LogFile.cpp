#include "StdAfx.h"
#include "LogFile.h"

CLogFile::CLogFile(void) : m_hFile(0), m_dwError(0)
{
}

CLogFile::~CLogFile(void)
{
	if (m_hFile) {
		CloseHandle(m_hFile);
        m_dwError = GetLastError();
		m_hFile = 0;
	}
}

int CLogFile::OpenLogFile (wchar_t *pFilePath)
{
    DWORD dwDesiredAccess = 0;
    DWORD dwShareMode = 0;
    DWORD dwCreationDesposition = 0;
    DWORD dwFlagsAndAttributes = 0;

    dwDesiredAccess |= GENERIC_WRITE;
    dwShareMode     |= FILE_SHARE_READ|FILE_SHARE_WRITE;
	dwCreationDesposition = CREATE_ALWAYS;
	dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH | FILE_FLAG_RANDOM_ACCESS;

	if (m_hFile) {
		CloseHandle(m_hFile);
        m_dwError = GetLastError();
		m_hFile = 0;
	}
	m_hFile = CreateFile (pFilePath, dwDesiredAccess, dwShareMode, NULL, dwCreationDesposition, dwFlagsAndAttributes, 0 );
    m_dwError = GetLastError();

	return 0;
}

int CLogFile::OpenLogFileIfClosed (wchar_t *pFilePath)
{
	if (m_hFile == 0) {
		OpenLogFile (pFilePath);
	}

	return 0;
}

int CLogFile::PrintLog (wchar_t *pLogLine)
{
	DWORD dwBytes = wcslen (pLogLine) * sizeof(wchar_t);
	DWORD dwNumberOfBytesWrite = 0;
	if (m_hFile) {
		WriteFile(m_hFile, pLogLine, dwBytes, &dwNumberOfBytesWrite, NULL);
	}

	return 0;
}