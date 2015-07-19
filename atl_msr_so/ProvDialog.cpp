// ProvDialog.cpp : Implementation of CProvDialog

#include "stdafx.h"
#include "ProvDialog.h"


// CProvDialog
LRESULT CProvDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CAxDialogImpl<CProvDialog>::OnInitDialog(uMsg, wParam, lParam, bHandled);

	bHandled = TRUE;

	CWindow pListView = GetDlgItem(IDC_LIST1);  // Get the listview control window handle

	// First of all lets make sure that the listview is in Details or Report style.
	// If the control is not in Details or Report style then even though we add columns
	// only the first column will be displayed.  Multiple columns only available in Details view.
	ULONG  ulWinStyle = pListView.GetWindowLong (GWL_STYLE);
	ulWinStyle |= LVS_REPORT;
	pListView.SetWindowLong (GWL_STYLE, ulWinStyle);

//	ListView_SetView (pListView.m_hWnd, LV_VIEW_DETAILS);

	// now we begin our initialization of the listview control.
	// we will be using a listview control with the following columns:
	//  - key of the value
	//  - value associated with the key

	// we will turn off redraw until we complete the listview changes.
	pListView.SendMessage (WM_SETREDRAW, (WPARAM)FALSE, (LPARAM)0);

	// Add the columns.
	// Initialize the LVCOLUMN structure.
	// The mask specifies that the format, width, text,
	// and subitem members of the structure are valid.
	wchar_t  *pszHeadings[] = {
		L"Header 1",
		L"Header 2"
	};

	
	pListView.SendMessage (LVM_SETITEMCOUNT, m_RegistryData->GetSize (), (LPARAM)0);

	// create the columns using the column headings.
	LVCOLUMN lvc = {0};
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt = LVCFMT_LEFT;      // Left-aligned column.
	lvc.cx = 80;                // Width of column in pixels.
	// Insert the columns and their headings into the list view.
	for (int iCol = 0; iCol < 2; iCol++)
	{
		lvc.pszText = pszHeadings[iCol];  // the text for the column heading
		ListView_InsertColumn (pListView.m_hWnd, iCol, &lvc);
	}


	// Initialize LVITEM members that are common to all items.
	LVITEM lvI = {0};
	lvI.pszText   = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
	lvI.mask      = LVIF_TEXT | LVIF_STATE | LVCF_SUBITEM;
	lvI.stateMask = 0;
	lvI.state     = 0;

	// Initialize LVITEM members that are different for each item.
	int iCount = m_RegistryData->GetSize();
	for (int index = 0; index < iCount; index++)
	{
		// Insert the item row with the first column into the list.
		lvI.iItem  = index;
		lvI.iSubItem  = 0;
		if (ListView_InsertItem (pListView.m_hWnd, &lvI) != -1) {
			// insert the second column into the listview.
			lvI.iSubItem  = 1;
			ListView_SetItem (pListView.m_hWnd, &lvI);
		}
	}

	// turn redraw back on for the listview and then refresh it.
	pListView.SendMessage (WM_SETREDRAW, (WPARAM)TRUE, (LPARAM)0);
	pListView.RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);

	return 1;  // Let the system set the focus
}

LRESULT CProvDialog::OnLvnItemchangedList1(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL & bHandled)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	return 0;
}

LRESULT CProvDialog::OnLvnGetDispInfoList1(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL & bHandled)
{
	static  wchar_t szText[128];

	LPNMLVDISPINFOW plvdi  = reinterpret_cast<LPNMLVDISPINFOW>(pNMHDR);
	switch (plvdi->item.iSubItem)
	{
		case 0:
			// First column for the listview
			if (plvdi->item.iItem < m_RegistryData->GetSize()) {
				wsprintf (szText, L"%-5.120s", m_RegistryData->GetKeyAt(plvdi->item.iItem).c_str());
				plvdi->item.pszText = szText; // rgPetInfo[plvdi->item.iItem].szKind;
			}
			break;

		case 1:
			// Second column for the listview
			if (plvdi->item.iItem < m_RegistryData->GetSize()) {
				wsprintf (szText, L"%-5.120s", m_RegistryData->GetValueAt(plvdi->item.iItem).c_str());
				plvdi->item.pszText = szText; // rgPetInfo[plvdi->item.iItem].szKind;
			}
			break;

		default:
			break;
	}

	bHandled = TRUE;

	return 1;
}

LRESULT CProvDialog::OnBnClickedButtonSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	CWindow pEdit = GetDlgItem (IDC_EDIT_PROP);

	int iTextSize = pEdit.GetWindowTextLength ();
	if (iTextSize > 0 && m_EditKey.length () > 0) {
		wchar_t myText[128];
		pEdit.GetWindowText(myText, 127);
		m_RegistryData->SetAt (m_EditKey, myText);
		
		m_EditKey.clear();            // clear the key area since we have done an update
		pEdit.SetWindowText (L"");    // clear the edit box before returning for user feedback.

		CWindow pListView = GetDlgItem(IDC_LIST1);  // Get the listview control window handle
		pListView.RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
	}
	bHandled = TRUE;
	return 1;
}

LRESULT CProvDialog::OnNMDblclkList1(int idCtrl, LPNMHDR pNMHDR, BOOL & bHandled)
{
	// Handle a double click in the listview control.
	//   "The iItem, iSubItem, and ptAction members of this
	//    structure [NMITEMACTIVATE] contain information about the item."
	LPNMITEMACTIVATE plvdi  = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (plvdi->iItem >= 0) {
		// we have a valid listview row number so lets update our edit box
		CWindow pEdit = GetDlgItem (IDC_EDIT_PROP);

		pEdit.SetWindowText (m_RegistryData->GetValueAt(plvdi->iItem).c_str());
		m_EditKey = m_RegistryData->GetKeyAt(plvdi->iItem);
	}

	bHandled = TRUE;
	return 1;
}
