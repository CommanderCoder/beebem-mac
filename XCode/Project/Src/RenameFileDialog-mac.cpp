//
//  RenameFileDialog-mac.cpp
//  BeebEm
//
//  Created by Commander Coder on 26/04/2024.
//

#include "RenameFileDialog-mac.hpp"


#include <windows.h>

#include <string>
#include <vector>

#include "RenameFileDialog-mac.hpp"
#include "Resource.h"

/****************************************************************************/

RenameFileDialog::RenameFileDialog(HINSTANCE hInstance,
								   HWND hwndParent,
								   const char* pszBeebFileName,
								   const char* pszHostFileName) :
//	Dialog(hInstance, hwndParent, IDD_RENAME_FILE),
	m_BeebFileName(pszBeebFileName),
	m_HostFileName(pszHostFileName)
{
}

/****************************************************************************/
//
//INT_PTR RenameFileDialog::DlgProc(
//	UINT   nMessage,
//	WPARAM wParam,
//	LPARAM /* lParam */)
//{
//	switch (nMessage)
//	{
//	case WM_INITDIALOG:
//		SetDlgItemText(IDC_BEEB_FILE_NAME, m_BeebFileName.c_str());
//		SetDlgItemText(IDC_HOST_FILE_NAME, m_HostFileName.c_str());
//
//		return TRUE;
//
//	case WM_COMMAND:
//		switch (LOWORD(wParam))
//		{
//		case IDOK:
//			GetDlgItemText(IDC_HOST_FILE_NAME);
//
//			m_HostFileName = GetDlgItemText(IDC_HOST_FILE_NAME);
//			EndDialog(m_hwnd, wParam);
//			break;
//
//		case IDCANCEL:
//			EndDialog(m_hwnd, wParam);
//			return TRUE;
//		}
//	}
//
//	return FALSE;
//}

/****************************************************************************/

const std::string& RenameFileDialog::GetHostFileName() const
{
	return m_HostFileName;
}
