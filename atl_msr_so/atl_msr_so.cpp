// atl_msr_so.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"
#include "atl_msr_so.h"
#include "dlldatax.h"

#include "LogFile.h"

static CLogFile myLogFile2;

class Catl_msr_soModule : public CAtlDllModuleT< Catl_msr_soModule >
{
public :
	DECLARE_LIBID(LIBID_atl_msr_soLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATL_MSR_SO, "{AFAD700E-5992-440C-960D-8680032AD539}")
};

Catl_msr_soModule _AtlModule;


#ifdef _MANAGED
#pragma managed(push, off)
#endif

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	myLogFile2.OpenLogFileIfClosed (L"C:\\VirtMsrLog2.log");
	myLogFile2.PrintLog (L" DLLMain called.\r\a");

#ifdef _MERGE_PROXYSTUB
    if (!PrxDllMain(hInstance, dwReason, lpReserved))
        return FALSE;
#endif
	myLogFile2.PrintLog (L" DLLMain 2 called.\r\a");
	hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}

#ifdef _MANAGED
#pragma managed(pop)
#endif




// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
#ifdef _MERGE_PROXYSTUB
    HRESULT hr = PrxDllCanUnloadNow();
    if (hr != S_OK)
        return hr;
#endif
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	HRESULT hrResult = E_OUTOFMEMORY;

    *ppv = NULL; 

	myLogFile2.OpenLogFileIfClosed (L"C:\\VirtMsrLog2.log");

	wchar_t xBuff[128] = {0};
	wsprintf (xBuff, L" DLLGetClassObject called REFCLSID 0x%x 0x%x 0x%x  0x%2.2x 0x%2.2x 0x%2.2x 0x%2.2x 0x%2.2x 0x%2.2x 0x%2.2x 0x%2.2x", rclsid.Data1, rclsid.Data2, rclsid.Data3,
		rclsid.Data4[0], rclsid.Data4[1], rclsid.Data4[2], rclsid.Data4[3], rclsid.Data4[4], rclsid.Data4[5], rclsid.Data4[6], rclsid.Data4[7]);
	myLogFile2.PrintLog (xBuff);
	wsprintf (xBuff, L" DLLGetClassObject called REFIID 0x%x 0x%x 0x%x  0x%2.2x 0x%2.2x 0x%2.2x 0x%2.2x 0x%2.2x 0x%2.2x 0x%2.2x 0x%2.2x", riid.Data1, riid.Data2, riid.Data3,
		riid.Data4[0], riid.Data4[1], riid.Data4[2], riid.Data4[3], riid.Data4[4], riid.Data4[5], riid.Data4[6], riid.Data4[7]);
	myLogFile2.PrintLog (xBuff);

#ifdef _MERGE_PROXYSTUB
    if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
        return S_OK;
#endif
	myLogFile2.PrintLog (L" DLLGetClassObject line 2.\r\a");

    hrResult = _AtlModule.DllGetClassObject(rclsid, riid, ppv);
	if (hrResult != S_OK) {
		myLogFile2.PrintLog (L" DLLGetClassObject line 3.\r\a");
	}

	wsprintf (xBuff, L" DLLGetClassObject line 4 %p  %p.  ", ppv, *ppv);
	myLogFile2.PrintLog (xBuff);

	return hrResult;
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
	myLogFile2.OpenLogFileIfClosed (L"C:\\VirtMsrLog2.log");
	myLogFile2.PrintLog (L" DLLRegisterServer called.\r\a");

	// registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();
#ifdef _MERGE_PROXYSTUB
    if (FAILED(hr))
        return hr;
    hr = PrxDllRegisterServer();
#endif
	return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
#ifdef _MERGE_PROXYSTUB
    if (FAILED(hr))
        return hr;
    hr = PrxDllRegisterServer();
    if (FAILED(hr))
        return hr;
    hr = PrxDllUnregisterServer();
#endif
	return hr;
}

