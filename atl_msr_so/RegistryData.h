#pragma once

#include <string>

#include <OposMsr.hi>

class CRegistryData
{
public:
	CRegistryData(void);
	CRegistryData(LPCWSTR DeviceClass, LPCWSTR DeviceName);
	~CRegistryData(void);
	void SetTheRegistryKey (LPCWSTR DeviceClass, LPCWSTR DeviceName);
	// Read the data from the registry filling in the data areas.
	int ReadRegistry(void);
	int WriteRegistry(void);

public:
	CSimpleMap <std::wstring, std::wstring> m_RegistryData;

private:
	wchar_t m_OposRootKey[256];

};
