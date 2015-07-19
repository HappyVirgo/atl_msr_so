#include "StdAfx.h"
#include "RegistryData.h"

CRegistryData::CRegistryData(void)
{
}

CRegistryData::CRegistryData(LPCWSTR DeviceClass, LPCWSTR DeviceName)
{
	SetTheRegistryKey (DeviceClass, DeviceName);
}

void CRegistryData::SetTheRegistryKey (LPCWSTR DeviceClass, LPCWSTR DeviceName)
{
	wsprintf (m_OposRootKey, L"%S\\%s\\%s", OPOS_ROOTKEY, DeviceClass, DeviceName);
}

CRegistryData::~CRegistryData(void)
{
}

// Read the data from the registry filling in the data areas.
int CRegistryData::ReadRegistry(void)
{
	HKEY  hKey = 0;

	LONG lRetVal = RegOpenKeyEx (HKEY_LOCAL_MACHINE, m_OposRootKey, 0, KEY_READ, &hKey);
	if (lRetVal == ERROR_SUCCESS) {
		DWORD  dwIndex = 0;
		DWORD  dwType;

		do {
			wchar_t  wsValueName[128] = {0};
			BYTE     wsValueValue[256] = {0};

			DWORD  dwValueNameSize = 124, dwValueValueSize = sizeof(wsValueValue);

			lRetVal = RegEnumValue (hKey, dwIndex, wsValueName, &dwValueNameSize, NULL, &dwType, wsValueValue, &dwValueValueSize);
			if (dwValueNameSize > 0 && lRetVal == ERROR_SUCCESS) {
				wsValueName[dwValueNameSize] = 0;
				wsValueValue[dwValueValueSize] = wsValueValue[dwValueValueSize+1] = 0;
				m_RegistryData.Add(std::wstring(wsValueName), std::wstring((wchar_t *)wsValueValue));
			}
			dwIndex++;
		} while (lRetVal == ERROR_SUCCESS);

		lRetVal = RegCloseKey (hKey);
	}

	return 0;
}

// Write the data to the registry from the data areas.
int CRegistryData::WriteRegistry(void)
{
	HKEY  hKey = 0;

	LONG lRetVal = RegOpenKeyEx (HKEY_LOCAL_MACHINE, m_OposRootKey, 0, KEY_WRITE, &hKey);
	if (lRetVal == ERROR_SUCCESS) {
		DWORD  dwIndex = 0;
		DWORD  dwType = REG_SZ;

		for (int iLoop = 0; iLoop < m_RegistryData.GetSize (); iLoop++) {
			std::wstring j1 = m_RegistryData.GetKeyAt(iLoop);
			std::wstring j2 = m_RegistryData.GetValueAt(iLoop);

			DWORD  dwValueValueSize = (j2.length() + 1) * sizeof(wchar_t);

			lRetVal = RegSetValueEx (hKey, j1.c_str(), NULL, dwType, (BYTE *)j2.c_str(), dwValueValueSize);
		}

		lRetVal = RegCloseKey (hKey);
	}

	return 0;
}
