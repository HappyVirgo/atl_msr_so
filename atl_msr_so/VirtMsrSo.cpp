// VirtMsrSo.cpp : Implementation of CVirtMsrSo

#include "stdafx.h"
#include "VirtMsrSo.h"
#include "LogFile.h"

#include <OposMsr.hi>

#include "ProvDialog.h"
#include "TestDialog.h"

// CVirtMsrSo

// Also need to create the Windows Registry entry for this Service Object.
// The name is the VersionIndependentProgID attribute in the .rgs file
// If you are on a 64bit machine:
//  [HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\OLEforRetail\ServiceOPOS\MSR\MyVirtualDevice]@="atl_msr_so.OPOS.VirtMsrSo"
// If you are on a 32bit machine:
//  [HKEY_LOCAL_MACHINE\SOFTWARE\OLEforRetail\ServiceOPOS\MSR\MyVirtualDevice]@="atl_msr_so.OPOS.VirtMsrSo"

static CLogFile myLogFile;

// --------   Helper methods follow

const wchar_t *CVirtMsrSo::m_SecureMag_rs232 = L"SECUREMAG_RS232";
const wchar_t *CVirtMsrSo::m_SecureMag_usbhid = L"SECUREMAG_USBHID";

// Service Oject calls a Common Object event request method. The
// methods SOData, SODirectIO, SOError, SOOutputComplete, and
// SOStatusUpdate are exposed.
void CVirtMsrSo::FireDataEvent(LONG lData)
{
	if (m_PIDX_Claimed && m_PIDX_DataEventEnabled) {
		VARIANT v; v.vt = VT_I4; v.lVal = lData; // 10;
		// turn this off so that only one event at a time is delivered
		m_PIDX_DataEventEnabled = 0;
		m_pDriver.Invoke1(L"SOData", &v);
	}
}

void CVirtMsrSo::FireStatusUpdateEvent(LONG lStatus)
{
	if (m_PIDX_Claimed && m_PIDX_DataEventEnabled) {
		VARIANT v; v.vt = VT_I4; v.lVal = lStatus;

		m_pDriver.Invoke1(L"SOStatusUpdate", &v);
	}
}

void CVirtMsrSo::FireErrorEvent(LONG lResultCode, LONG lResultCodeExtended, LONG lErrorLocus, LONG *plErrorResponse)
{
	if (m_PIDX_Claimed && m_PIDX_DataEventEnabled) {
		VARIANT vRet;
		VARIANT v[3];
		v[0].vt = VT_I4; v[0].lVal = lResultCode;
		v[1].vt = VT_I4; v[1].lVal = lResultCodeExtended;
		v[2].vt = VT_I4; v[2].lVal = lErrorLocus;

		m_pDriver.InvokeN(L"SOStatusUpdate", v, 3, &vRet);
		if (plErrorResponse && vRet.vt == VT_I4) {
			*plErrorResponse = vRet.lVal;
		}
	}
}
// -------- Standard OPOS Service Object methods follow

// The OPenService() method is called by the Open() method of the OPOS Common Control object.
//
// WARNING: When using the Class View of Visual Studio to add this method to the Service
//          Object interface you may run into an error from Visual Studio.  It appears that
//          OpenService() is also part of the Windows API and the Visual Studio Intellisense
//          may emit errors when attempting to add this method.
//          You may need to close Visual Studio and delete the .ncb Intellisense data file
//          in order to recover.
//          You may also need to create this method with a different name such as CheckService
//          and then once created, do a Find in order to find all usages of CheckService and
//          change them to OpenService.
STDMETHODIMP CVirtMsrSo::OpenService(BSTR DeviceClass, BSTR DeviceName, IDispatch* pDispatch, LONG* pRc)
{
	m_pDriver = pDispatch;
	*pRc = OPOS_SUCCESS;

	myLogFile.OpenLogFileIfClosed (L"C:\\VirtMsrLog.log");
	wchar_t  xBuff[128];
	wsprintf (xBuff, L"OpenService called: %s %s  ", DeviceClass, DeviceName);
	myLogFile.PrintLog (xBuff);

	m_RegistryData.SetTheRegistryKey (DeviceClass, DeviceName);
	m_RegistryData.ReadRegistry ();

	for (int iLoop = 0; iLoop < m_RegistryData.m_RegistryData.GetSize (); iLoop++) {
		std::wstring j1 = m_RegistryData.m_RegistryData.GetKeyAt(iLoop);
		std::wstring j2 = m_RegistryData.m_RegistryData.GetValueAt(iLoop);
		wsprintf (xBuff, L"  -Reg: %d %s %s  ", iLoop, j1.c_str(), j2.c_str());
		myLogFile.PrintLog (xBuff);
	}

	m_PIDX_ServiceObjectVersion = lVersionNumber;
	m_PIDX_Claimed = 0;
	m_PIDX_DataEventEnabled = 0;
	m_PIDX_DeviceEnabled = 0;
	m_PIDX_FreezeEvents = 0;
	m_PIDX_OutputID = 0;
	m_PIDX_ResultCode = 0;
	m_PIDX_ResultCodeExtended = 0;
	m_PIDX_State = 0;
	m_PIDX_AutoDisable = 0;
	m_PIDX_BinaryConversion = 0;
	m_PIDX_DataCount = 0;
	m_PIDX_PowerNotify = 0;
	m_PIDX_PowerState = 0;
	m_PIDXMsr_DecodeData = 0;
	m_PIDXMsr_ParseDecodeData = 0;
	m_PIDXMsr_TracksToRead = MSR_TR_1_2;
	m_PIDXMsr_ErrorReportingType = MSR_ERT_CARD;

	memset (m_RawDataTrack, 0, sizeof(m_RawDataTrack));

	wcscpy (m_PIDXMsr_Track1Data, L"track1 1234567890");
	wcscpy (m_PIDXMsr_Track2Data, L"track2 1234567890");
	wcscpy (m_PIDXMsr_Track3Data, L"track3 1234567890");

	if (wcscmp (DeviceClass, L"MSR") != 0) {
		return E_INVALIDARG;
	} else {
		return S_OK;
	}
}

STDMETHODIMP CVirtMsrSo::CheckHealth(LONG level, LONG* pRc)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\VirtMsrLog.log");
	wchar_t  xBuff[128];
	wsprintf (xBuff, L"CheckHealth called %d  ", level);
	myLogFile.PrintLog (xBuff);

	if (level > 2) {
		CProvDialog myDialog;
		myDialog.m_RegistryData = &m_RegistryData.m_RegistryData;
		myDialog.DoModal ();
	}
	m_PIDX_ResultCode = 0;
	m_PIDX_ResultCodeExtended = 0;
	*pRc = OPOS_SUCCESS;
	return S_OK;
}

STDMETHODIMP CVirtMsrSo::ClaimDevice(LONG ClaimTimeOut, LONG* pRc)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\VirtMsrLog.log");
	myLogFile.PrintLog (L"ClaimDevice called. \n");

	m_PIDX_Claimed = 1;
	*pRc = OPOS_SUCCESS;
	return S_OK;
}

STDMETHODIMP CVirtMsrSo::ClearInput(LONG* pRc)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\VirtMsrLog.log");
	myLogFile.PrintLog (L"ClearInput called. \n");

	memset (m_RawDataTrack, 0, sizeof(m_RawDataTrack));

	*pRc = OPOS_SUCCESS;
	return S_OK;
}

STDMETHODIMP CVirtMsrSo::CloseService(LONG* pRc)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\VirtMsrLog.log");
	myLogFile.PrintLog (L"CloseService called. \n");

	m_PIDX_Claimed = 0;
	m_PIDX_DataEventEnabled = 0;
	m_PIDX_DeviceEnabled = 0;
	m_PIDX_FreezeEvents = 0;
	m_PIDX_OutputID = 0;
	m_PIDX_ResultCode = 0;
	m_PIDX_ResultCodeExtended = 0;
	m_PIDX_State = 0;
	m_PIDX_AutoDisable = 0;
	m_PIDX_BinaryConversion = 0;
	m_PIDX_DataCount = 0;
	m_PIDX_PowerNotify = 0;
	m_PIDX_PowerState = 0;

	memset (m_RawDataTrack, 0, sizeof(m_RawDataTrack));

	*pRc = OPOS_SUCCESS;
	return S_OK;
}

STDMETHODIMP CVirtMsrSo::COFreezeEvents(VARIANT_BOOL Freeze, LONG* pRc)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\VirtMsrLog.log");
	myLogFile.PrintLog (L"COFreezeEvents called. \n");

	*pRc = OPOS_SUCCESS;
	return S_OK;
}

STDMETHODIMP CVirtMsrSo::DirectIO(LONG Command, LONG* pData, BSTR* pString, LONG* pRc)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\VirtMsrLog.log");
	myLogFile.PrintLog (L"DirectIO called. \n");

	*pRc = OPOS_SUCCESS;
	return S_OK;
}

STDMETHODIMP CVirtMsrSo::ReleaseDevice(LONG* pRc)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\VirtMsrLog.log");
	myLogFile.PrintLog (L"ReleaseDevice called. \n");

	m_PIDX_Claimed = 0;

	memset (m_RawDataTrack, 0, sizeof(m_RawDataTrack));

	*pRc = OPOS_SUCCESS;
	return S_OK;
}

STDMETHODIMP CVirtMsrSo::GetPropertyNumber(LONG PropIndex, LONG* pNumber)
{
	HRESULT  hrStatus = S_OK;

	myLogFile.OpenLogFileIfClosed (L"C:\\VirtMsrLog.log");

	switch (PropIndex) {
		case PIDX_ServiceObjectVersion:
			*pNumber = m_PIDX_ServiceObjectVersion;
			break;
		case PIDX_Claimed:
			*pNumber = m_PIDX_Claimed;
			break;
		case PIDX_DataEventEnabled:
			*pNumber = m_PIDX_DataEventEnabled;
			break;
		case PIDX_DeviceEnabled:
			*pNumber = m_PIDX_DeviceEnabled;
			break;
		case PIDX_FreezeEvents:
			*pNumber = m_PIDX_FreezeEvents;
			break;
		case PIDX_OutputID:
			*pNumber = m_PIDX_OutputID;
			break;
		case PIDX_ResultCode:
			*pNumber = m_PIDX_ResultCode;
			break;
		case PIDX_ResultCodeExtended:
			*pNumber = m_PIDX_ResultCodeExtended;
			break;
		case PIDX_State:
			*pNumber = m_PIDX_State;
			break;
		case PIDX_AutoDisable:
			*pNumber = m_PIDX_AutoDisable;
			break;
		case PIDX_BinaryConversion:
			*pNumber = m_PIDX_BinaryConversion;
			break;
		case PIDX_DataCount:
			*pNumber = m_PIDX_DataCount;
			break;
		case PIDX_PowerNotify:
			*pNumber = m_PIDX_PowerNotify;
			break;
		case PIDX_PowerState:
			*pNumber = m_PIDX_PowerState;
			break;
		case PIDXMsr_DecodeData:
			*pNumber = m_PIDXMsr_DecodeData;
			break;
		case PIDXMsr_ParseDecodeData:
			*pNumber = m_PIDXMsr_ParseDecodeData;
			break;
		case PIDXMsr_TracksToRead:
			*pNumber = m_PIDXMsr_TracksToRead;
			break;
		case PIDXMsr_ErrorReportingType:
			*pNumber = m_PIDXMsr_ErrorReportingType;
			break;
		default:
			hrStatus = E_INVALIDARG;
			break;
	}

	wchar_t  xBuff[128];
	wsprintf (xBuff, L"GetPropertyNumber called %d %d  ", PropIndex, *pNumber);
	myLogFile.PrintLog (xBuff);
	return hrStatus;
}

STDMETHODIMP CVirtMsrSo::GetPropertyString(LONG PropIndex, BSTR* pString)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\VirtMsrLog.log");
	wchar_t  xBuff[128];
	wsprintf (xBuff, L"GetPropertyString called %d  ", PropIndex);
	myLogFile.PrintLog (xBuff);

	HRESULT  hrStatus = S_OK;

	pString = NULL;

	switch (PropIndex) {
		case PIDXMsr_AccountNumber:
			break;
		case PIDXMsr_ExpirationDate:
			break;
		case PIDXMsr_FirstName:
			break;
		case PIDXMsr_MiddleInitial:
			break;
		case PIDXMsr_ServiceCode:
			break;
		case PIDXMsr_Suffix:
			break;
		case PIDXMsr_Surname:
			break;
		case PIDXMsr_Title:
			break;
		case PIDXMsr_Track1Data:
			pString = new BSTR(m_PIDXMsr_Track1Data);
			break;
		case PIDXMsr_Track1DiscretionaryData:
			break;
		case PIDXMsr_Track2Data:
			pString = new BSTR(m_PIDXMsr_Track2Data);
			break;
		case PIDXMsr_Track2DiscretionaryData:
			break;
		case PIDXMsr_Track3Data:
			pString = new BSTR(m_PIDXMsr_Track3Data);
			break;
		default:
			hrStatus = E_INVALIDARG;
			break;
	}

	return S_OK;
}

STDMETHODIMP CVirtMsrSo::SetPropertyNumber(LONG PropIndex, LONG Number)
{
	HRESULT  hrStatus = S_OK;

	myLogFile.OpenLogFileIfClosed (L"C:\\VirtMsrLog.log");
	wchar_t  xBuff[128];
	wsprintf (xBuff, L"SetPropertyNumber called %d %d  ", PropIndex, Number);
	myLogFile.PrintLog (xBuff);

	switch (PropIndex) {
		case PIDX_DataEventEnabled:
			m_PIDX_DataEventEnabled = Number;
			break;
		case PIDX_DeviceEnabled:
			m_PIDX_DeviceEnabled = Number;
			break;
		case PIDX_FreezeEvents:
			m_PIDX_FreezeEvents = Number;
			break;
		case PIDX_OutputID:
			m_PIDX_OutputID = Number;
			break;
		case PIDX_ResultCode:
			m_PIDX_ResultCode = Number;
			break;
		case PIDX_ResultCodeExtended:
			m_PIDX_ResultCodeExtended = Number;
			break;
		case PIDX_AutoDisable:
			m_PIDX_AutoDisable = Number;
			break;
		case PIDX_BinaryConversion:
			m_PIDX_BinaryConversion = Number;
			break;
		case PIDXMsr_DecodeData:
			m_PIDXMsr_DecodeData = Number;
			break;
		case PIDXMsr_ParseDecodeData:
			m_PIDXMsr_ParseDecodeData = Number;
			break;
		case PIDXMsr_TracksToRead:
			m_PIDXMsr_TracksToRead = Number;
			break;
		case PIDXMsr_ErrorReportingType:
			m_PIDXMsr_ErrorReportingType = Number;
			break;
		default:
			hrStatus = E_INVALIDARG;
			break;
	}

	return hrStatus;
}

STDMETHODIMP CVirtMsrSo::SetPropertyString(LONG PropIndex, BSTR PropString)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\VirtMsrLog.log");
	wchar_t  xBuff[128];
	wsprintf (xBuff, L"SetPropertyString called %d  ", PropIndex);
	myLogFile.PrintLog (xBuff);
	return S_OK;
}

STDMETHODIMP CVirtMsrSo::GetOpenResult(LONG* pRc)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\VirtMsrLog.log");
	myLogFile.PrintLog (L"GetOpenResult called \n");

	*pRc = OPOS_SUCCESS;
	return S_OK;
}
