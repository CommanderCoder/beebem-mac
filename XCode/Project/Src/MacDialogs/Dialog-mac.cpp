//
//  Dialog-mac.cpp
//  BeebEm
//
//  Created by Commander Coder on 26/04/2024.
//

#include <windows.h>

#include <vector>

#include "Dialog-mac.hpp"

/****************************************************************************/

Dialog::Dialog(HINSTANCE hInstance,
			   HWND hwndParent,
			   int DialogID) :
	m_hInstance(hInstance),
	m_hwndParent(hwndParent),
	m_DialogID(DialogID),
	m_hwnd(nullptr)
{
}

/****************************************************************************/


/****************************************************************************/
//
//INT_PTR CALLBACK Dialog::sDlgProc(HWND   hwnd,
//								  UINT   nMessage,
//								  WPARAM wParam,
//								  LPARAM lParam)
//{
//	Dialog* dialog;
//
//	if (nMessage == WM_INITDIALOG)
//	{
//		SetWindowLongPtr(hwnd, DWLP_USER, lParam);
//		dialog = reinterpret_cast<Dialog*>(lParam);
//		dialog->m_hwnd = hwnd;
//
//		dialog->CenterDialog();
//	}
//	else
//	{
//		dialog = reinterpret_cast<Dialog*>(
//			GetWindowLongPtr(hwnd, DWLP_USER)
//		);
//	}
//
//	if (dialog)
//	{
//		return dialog->DlgProc(nMessage, wParam, lParam);
//	}
//	else
//	{
//		return FALSE;
//	}
//}
//
///****************************************************************************/
//
//std::string Dialog::GetDlgItemText(int nID)
//{
//	int Length = GetWindowTextLength(GetDlgItem(m_hwnd, nID));
//
//	std::vector<char> Text;
//	Text.resize(Length + 1);
//
//	::GetDlgItemText(m_hwnd, nID, &Text[0], (int)Text.size());
//
//	return std::string(&Text[0]);
//}
//
///****************************************************************************/
//
//void Dialog::SetDlgItemText(int nID, const std::string& str)
//{
//	SetWindowText(GetDlgItem(m_hwnd, nID), str.c_str());
//}
//
///****************************************************************************/
//
//bool Dialog::IsDlgItemChecked(int nID)
//{
//	return SendDlgItemMessage(m_hwnd, nID, BM_GETCHECK, 0, 0) == BST_CHECKED;
//}
//
///****************************************************************************/
//
//void Dialog::SetDlgItemChecked(int nID, bool bChecked)
//{
//	SendDlgItemMessage(m_hwnd, nID, BM_SETCHECK, bChecked ? BST_CHECKED : BST_UNCHECKED, 0);
//}
//
///****************************************************************************/
//
//void Dialog::SetDlgItemFocus(int nID)
//{
//	SetFocus(GetDlgItem(m_hwnd, nID));
//}
//
///****************************************************************************/
//
//void Dialog::CenterDialog()
//{
//	RECT rcOwner;
//	GetWindowRect(m_hwndParent, &rcOwner);
//
//	RECT rcDialog;
//	GetWindowRect(m_hwnd, &rcDialog);
//
//	RECT rc;
//	CopyRect(&rc, &rcOwner);
//
//	// Offset the owner and dialog box rectangles so that right and bottom
//	// values represent the width and height, and then offset the owner again
//	// to discard space taken up by the dialog box.
//
//	OffsetRect(&rcDialog, -rcDialog.left, -rcDialog.top);
//	OffsetRect(&rc, -rc.left, -rc.top);
//	OffsetRect(&rc, -rcDialog.right, -rcDialog.bottom);
//
//	// The new position is the sum of half the remaining space and the owner's
//	// original position.
//
//	SetWindowPos(m_hwnd,
//				 HWND_TOP,
//				 rcOwner.left + (rc.right / 2),
//				 rcOwner.top + (rc.bottom / 2),
//				 0, 0, // Ignores size arguments.
//				 SWP_NOSIZE);
//}
