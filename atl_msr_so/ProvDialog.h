// ProvDialog.h : Declaration of the CProvDialog

#pragma once

#include <string>

#include "resource.h"       // main symbols

#include <atlhost.h>

// CProvDialog

class CProvDialog : 
	public CAxDialogImpl<CProvDialog>
{
public:
	CProvDialog()
	{
	}

	~CProvDialog()
	{
	}

	enum { IDD = IDD_PROVDIALOG };

BEGIN_MSG_MAP(CProvDialog)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	NOTIFY_HANDLER(IDC_LIST1, LVN_ITEMCHANGED, OnLvnItemchangedList1)
	NOTIFY_HANDLER(IDC_LIST1, LVN_GETDISPINFO, OnLvnGetDispInfoList1)
	COMMAND_HANDLER(IDC_BUTTON_SAVE, BN_CLICKED, OnBnClickedButtonSave)
	NOTIFY_HANDLER(IDC_LIST1, NM_DBLCLK, OnNMDblclkList1)
	CHAIN_MSG_MAP(CAxDialogImpl<CProvDialog>)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
	LRESULT OnLvnItemchangedList1(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnLvnGetDispInfoList1(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);

public:
	CSimpleMap <std::wstring, std::wstring> *m_RegistryData;
	std::wstring m_EditKey;

	LRESULT OnBnClickedButtonSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNMDblclkList1(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
};
