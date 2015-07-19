// VirtMsrSo.h : Declaration of the CVirtMsrSo

#pragma once
#include "resource.h"       // main symbols

#include "atl_msr_so.h"

#include "RegistryData.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CVirtMsrSo

class ATL_NO_VTABLE CVirtMsrSo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVirtMsrSo, &CLSID_VirtMsrSo>,
	public IDispatchImpl<IVirtMsrSo, &IID_IVirtMsrSo, &LIBID_atl_msr_soLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVirtMsrSo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VIRTMSRSO)


BEGIN_COM_MAP(CVirtMsrSo)
	COM_INTERFACE_ENTRY(IVirtMsrSo)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

private:
	// persistent data items used by the Service Object

	// Request for the 3 digit Service Object OPOS version, Major, Minor, Build
	// Format is 3 decimal digits per version part returned as a 32 bit signed value.
	// Testing indicates that a version of 1.2.1 works well to exercise basic functionality.
	// Newer revisions of the OPOS standard with higher minor number requires additional
	// properties to be supported by the Service Object.
	static const LONG  lMajor = 1;
	static const LONG  lMinor = 2;
	static const LONG  lBuild = 1;
	static const LONG  lVersionNumber = lMajor * 1000000 + lMinor * 1000 + lBuild;

	// Device name used with the ClaimService() to indicate which interface to use
	static const wchar_t *m_SecureMag_rs232;
	static const wchar_t *m_SecureMag_usbhid;

	CComDispatchDriver m_pDriver;

	// General OPOS Service Object numeric properties used by all Service Objects.
	LONG m_PIDX_ServiceObjectVersion;
	LONG m_PIDX_Claimed;
	LONG m_PIDX_DataEventEnabled;
	LONG m_PIDX_DeviceEnabled;
	LONG m_PIDX_FreezeEvents;
	LONG m_PIDX_OutputID;
	LONG m_PIDX_ResultCode;
	LONG m_PIDX_ResultCodeExtended;
	LONG m_PIDX_State;
	LONG m_PIDX_AutoDisable;
	LONG m_PIDX_BinaryConversion;
	LONG m_PIDX_DataCount;
	LONG m_PIDX_PowerNotify;
	LONG m_PIDX_PowerState;

	// MSR specific OPOS Service Object properties
	LONG m_PIDXMsr_DecodeData;
	LONG m_PIDXMsr_ParseDecodeData;
	LONG m_PIDXMsr_TracksToRead;
	LONG m_PIDXMsr_ErrorReportingType;

	wchar_t  m_PIDXMsr_AccountNumber[96];
	wchar_t  m_PIDXMsr_ExpirationDate[8];
	wchar_t  m_PIDXMsr_FirstName[128];
	wchar_t  m_PIDXMsr_MiddleInitial[8];
	wchar_t  m_PIDXMsr_ServiceCode[8];
	wchar_t  m_PIDXMsr_Suffix[8];
	wchar_t  m_PIDXMsr_Surname[128];
	wchar_t  m_PIDXMsr_Title[128];
	wchar_t  m_PIDXMsr_Track1Data[128];
	wchar_t  m_PIDXMsr_Track1DiscretionaryData[128];
	wchar_t  m_PIDXMsr_Track2Data[128];
	wchar_t  m_PIDXMsr_Track2DiscretionaryData[128];
	wchar_t  m_PIDXMsr_Track3Data[128];

	wchar_t  m_RawDataTrack[1024];

	CRegistryData  m_RegistryData;

private:
	// Helper functions used for various operations.
	void FireDataEvent(LONG  lData);
	void FireStatusUpdateEvent(LONG lStatus);
	void FireErrorEvent(LONG lResultCode, LONG lResultCodeExtended, LONG lErrorLocus, LONG *plErrorResponse);

public:
	// The methods implementing the OPOS Service Object interface
	STDMETHOD(OpenService)(BSTR DeviceClass, BSTR DeviceName, IDispatch* pDispatch, LONG* pRc);
	STDMETHOD(CheckHealth)(LONG level, LONG* pRc);
	STDMETHOD(ClaimDevice)(LONG ClaimTimeOut, LONG* pRc);
	STDMETHOD(ClearInput)(LONG* pRc);
	STDMETHOD(CloseService)(LONG* pRc);
	STDMETHOD(COFreezeEvents)(VARIANT_BOOL Freeze, LONG* pRc);
	STDMETHOD(DirectIO)(LONG Command, LONG* pData, BSTR* pString, LONG* pRc);
	STDMETHOD(ReleaseDevice)(LONG* pRc);
	STDMETHOD(GetPropertyNumber)(LONG PropIndex, LONG* pNumber);
	STDMETHOD(GetPropertyString)(LONG PropIndex, BSTR* pString);
	STDMETHOD(SetPropertyNumber)(LONG PropIndex, LONG Number);
	STDMETHOD(SetPropertyString)(LONG PropIndex, BSTR PropString);
	STDMETHOD(GetOpenResult)(LONG* pRc);
};

OBJECT_ENTRY_AUTO(__uuidof(VirtMsrSo), CVirtMsrSo)
