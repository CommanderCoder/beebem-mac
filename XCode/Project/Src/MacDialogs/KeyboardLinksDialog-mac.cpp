//
//  KeyboardLinksDialog-mac.cpp
//  BeebEm
//
//  Created by Commander Coder on 13/05/2024.
//

#include "KeyboardLinksDialog-mac.hpp"


#include <windows.h>

#include "KeyboardLinksDialog.h"
#include "Resource.h"
#include "beebemrcids.h"
#include "Main.h"

#undef IDD_KEYBOARD_LINKS
#define IDD_KEYBOARD_LINKS		Modals::keyboardLinks


/****************************************************************************/

KeyboardLinksDialog::KeyboardLinksDialog(HINSTANCE hInstance,
										 HWND hwndParent,
										 unsigned char Value) :
	Dialog(hInstance, hwndParent, IDD_KEYBOARD_LINKS),
	m_Value(Value)
{
}

/****************************************************************************/

static const UINT nControlIDs[] = {
	IDC_KEYBOARD_BIT0,
	IDC_KEYBOARD_BIT1,
	IDC_KEYBOARD_BIT2,
	IDC_KEYBOARD_BIT3,
	IDC_KEYBOARD_BIT4,
	IDC_KEYBOARD_BIT5,
	IDC_KEYBOARD_BIT6,
	IDC_KEYBOARD_BIT7
};


extern KeyboardLinksDialog* runningKLDialog;

bool KeyboardLinksDialog::DoModal() {
	runningKLDialog = this;
	WM_INITDIALOG();
	bool ret = swift_DoModal((Modals)m_DialogID, this);
	//runningKLDialog// = NULL;
	return ret;
}


/****************************************************************************/
static void EndDialog(HWND h, WPARAM p)
{
	swift_EndModal(p);
}

//INT_PTR KeyboardLinksDialog::DlgProc(UINT   nMessage,
//									 WPARAM wParam,
//									 LPARAM /* lParam */)
//{
//	switch (nMessage)
//	{
//
//		case WM_INITDIALOG: {
bool KeyboardLinksDialog::WM_INITDIALOG()
{
			for (int i = 0; i < 8; i++)
			{
				SetDlgItemChecked(nControlIDs[i], (m_Value & (1 << i)) != 0);
			}
			
			return TRUE;
}

//		case WM_COMMAND:
bool KeyboardLinksDialog::WM_COMMAND(WPARAM wParam)
{
			switch (LOWORD(wParam))
			{
			case IDOK:
				m_Value = 0;

				for (int i = 0; i < 8; i++)
				{
					if (IsDlgItemChecked(nControlIDs[i]))
					{
						m_Value |= 1 << i;
					}
				}

					runningKLDialog = NULL;

				EndDialog(m_hwnd, wParam);
				return TRUE;

			case IDCANCEL:
					runningKLDialog = NULL;
				EndDialog(m_hwnd, wParam);
				return TRUE;
			}
//	}

	return FALSE;
}

/****************************************************************************/

unsigned char KeyboardLinksDialog::GetValue() const
{
	return m_Value;
}
