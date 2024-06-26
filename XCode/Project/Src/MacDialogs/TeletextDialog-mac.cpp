//
//  TeletextDialog-mac.cpp
//  BeebEm
//
//  Created by Commander Coder on 12/05/2024.
//


#include <windows.h>
#include <windowsx.h>

#include <string>
#include <vector>

#include "TeletextDialog.h"
#include "FileDialog.h"
#include "Main.h"
#include "Resource.h"
#include "StringUtils.h"

/****************************************************************************/

static const int FileControl[] =
{
	IDC_FILE1,
	IDC_FILE2,
	IDC_FILE3,
	IDC_FILE4
};

static const int IPAddressControl[] =
{
	IDC_IP_ADDRESS1,
	IDC_IP_ADDRESS2,
	IDC_IP_ADDRESS3,
	IDC_IP_ADDRESS4
};

static const int IPPortControl[] =
{
	IDC_IP_PORT1,
	IDC_IP_PORT2,
	IDC_IP_PORT3,
	IDC_IP_PORT4
};

static const int EnableFileControl[] =
{
	IDC_FILE1,
	IDC_FILE2,
	IDC_FILE3,
	IDC_FILE4,
	IDC_SELECT_FILE1,
	IDC_SELECT_FILE2,
	IDC_SELECT_FILE3,
	IDC_SELECT_FILE4
};

static const int EnableIPAddressControl[] =
{
	IDC_IP_ADDRESS1,
	IDC_IP_ADDRESS2,
	IDC_IP_ADDRESS3,
	IDC_IP_ADDRESS4,
	IDC_IP_ADDRESS1_STATIC,
	IDC_IP_ADDRESS2_STATIC,
	IDC_IP_ADDRESS3_STATIC,
	IDC_IP_ADDRESS4_STATIC,
	IDC_IP_PORT1,
	IDC_IP_PORT2,
	IDC_IP_PORT3,
	IDC_IP_PORT4,
	IDC_PORT1_STATIC,
	IDC_PORT2_STATIC,
	IDC_PORT3_STATIC,
	IDC_PORT4_STATIC
};

#undef IDD_TELETEXT
#define IDD_TELETEXT			Dialogs::teletextSelect

/****************************************************************************/

TeletextDialog::TeletextDialog(HINSTANCE hInstance,
							   HWND hwndParent,
							   TeletextSourceType Source,
							   const std::string& DiscsPath,
							   const std::string* FileName,
							   const std::string* IPAddress,
							   const u_short* IPPort) :
	Dialog(hInstance, hwndParent, IDD_TELETEXT),
	m_TeletextSource(Source),
	m_DiscsPath(DiscsPath)
{
	for (int i = 0; i < TELETEXT_CHANNEL_COUNT; i++)
	{
		m_FileName[i] = FileName[i];
		m_IPAddress[i] = IPAddress[i];
		m_IPPort[i] = IPPort[i];
	}
}



static void SetDlgItemFocus(int nID)
{
	
}


static void ComboBox_AddString(UINT* nIDDlgItem,
							   std::string b)
{
	
}


static void ComboBox_SetCurSel(UINT* nIDDlgItem,
							   INT b)
{
	
}


bool TeletextDialog::DoModal() {
//	runningTXDialog = this;
	WM_INITDIALOG();
	bool ret = swift_OpenDialog(Dialogs::teletextSelect, this);
//	runningTXDialog = NULL;
	return ret;
}


/****************************************************************************/
bool TeletextDialog::WM_INITDIALOG()
{
//INT_PTR TeletextDialog::DlgProc(UINT   nMessage,
//								WPARAM wParam,
//								LPARAM /* lParam */)
//{
//	switch (nMessage)
//	{
//		case WM_INITDIALOG: {
			ComboBox_AddString(GetDlgItem(m_hwnd, IDC_TELETEXT_SOURCE), "TCP/IP");
			ComboBox_AddString(GetDlgItem(m_hwnd, IDC_TELETEXT_SOURCE), "Capture Files");

			int Index = static_cast<int>(m_TeletextSource);

			ComboBox_SetCurSel(GetDlgItem(m_hwnd, IDC_TELETEXT_SOURCE), Index);

			EnableFileControls(m_TeletextSource == TeletextSourceType::File);
			EnableIPControls(m_TeletextSource == TeletextSourceType::IP);

			for (int i = 0; i < TELETEXT_CHANNEL_COUNT; i++)
			{
				SetDlgItemText(FileControl[i], m_FileName[i]);
				SetChannelIPControls(i);
			}

			return TRUE;
		}

bool TeletextDialog::WM_COMMAND(WPARAM wParam){
//		case WM_COMMAND:
			return OnCommand(HIWORD(wParam), LOWORD(wParam));
//	}
}

//	return FALSE;
//}

/****************************************************************************/
static void EndDialog(HWND h, WPARAM p)
{
	swift_EndModal(p);
}

//INT_PTR TeletextDialog::OnCommand(int Notification, int nCommandID)
BOOL TeletextDialog::OnCommand(int Notification, int nCommandID)
{
	switch (nCommandID)
	{
		case IDOK: {
			if (m_TeletextSource == TeletextSourceType::File)
			{
				for (int i = 0; i < TELETEXT_CHANNEL_COUNT; i++)
				{
					m_FileName[i] = GetDlgItemText(FileControl[i]);
				}
			}
			else if (m_TeletextSource == TeletextSourceType::IP)
			{
				for (int i = 0; i < TELETEXT_CHANNEL_COUNT; i++)
				{
					if (!GetChannelIPControls(i))
					{
						return FALSE;
					}
				}
			}

			EndDialog(m_hwnd, IDOK);
			return FALSE;
		}

		case IDCANCEL:
			EndDialog(m_hwnd, IDCANCEL);
			return FALSE;

		case IDC_TELETEXT_SOURCE:
#ifdef APPLE__FUTURE
			if (Notification == CBN_SELCHANGE)
			{
				m_TeletextSource = static_cast<TeletextSourceType>(
					ComboBox_GetCurSel(GetDlgItem(m_hwnd, IDC_TELETEXT_SOURCE))
				);

				EnableFileControls(m_TeletextSource == TeletextSourceType::File);
				EnableIPControls(m_TeletextSource == TeletextSourceType::IP);				
			}
#endif
			break;

		case IDC_SELECT_FILE1:
			SelectFile(0);
			break;

		case IDC_SELECT_FILE2:
			SelectFile(1);
			break;

		case IDC_SELECT_FILE3:
			SelectFile(2);
			break;

		case IDC_SELECT_FILE4:
			SelectFile(3);
			break;
	}

	return TRUE;
}

/****************************************************************************/

void TeletextDialog::SelectFile(int Channel)
{
	char DefaultPath[_MAX_PATH];
	char FileName[_MAX_PATH];
	FileName[0] = '\0';

	strcpy(DefaultPath, m_DiscsPath.c_str());
	mainWin->GetDataPath(mainWin->GetUserDataPath(), DefaultPath);

	const char* Filter = "Teletext Capture File (*.dat)\0*.dat\0";

	FileDialog Dialog(m_hwnd, FileName, sizeof(FileName), DefaultPath, Filter);

	if (Dialog.Open())
	{
		m_FileName[Channel] = FileName;
		SetDlgItemText(FileControl[Channel], m_FileName[Channel]);
	}
}

/****************************************************************************/

BOOL TeletextDialog::GetChannelIPControls(int Channel)
{
	m_IPAddress[Channel] = GetDlgItemText(IPAddressControl[Channel]);

	unsigned long Address = inet_addr(m_IPAddress[Channel].c_str());

	if (Address == INADDR_NONE)
	{
		mainWin->Report(MessageType::Error, "Please enter a valid IP address");
		SetDlgItemFocus(IPAddressControl[Channel]);
		return FALSE;
	}

	std::string strPort = GetDlgItemText(IPPortControl[Channel]);

	int Port;

	bool bSuccess = ParseNumber(strPort, &Port);

	if (bSuccess && Port >= 0 && Port <= 65535)
	{
		m_IPPort[Channel] = static_cast<u_short>(Port);
	}
	else
	{
		mainWin->Report(MessageType::Error, "Please enter a valid port number");
		SetDlgItemFocus(IPPortControl[Channel]);
		return FALSE;
	}

	return TRUE;
}

/****************************************************************************/

void TeletextDialog::SetChannelIPControls(int Channel)
{
	SetDlgItemText(IPAddressControl[Channel], m_IPAddress[Channel]);

	char sz[20];
	sprintf(sz, "%d", m_IPPort[Channel]);
	SetDlgItemText(IPPortControl[Channel], sz);
}

/****************************************************************************/

TeletextSourceType TeletextDialog::GetSource() const
{
	return m_TeletextSource;
}

/****************************************************************************/

const std::string& TeletextDialog::GetFileName(int Index) const
{
	return m_FileName[Index];
}

/****************************************************************************/

const std::string& TeletextDialog::GetIPAddress(int Index) const
{
	return m_IPAddress[Index];
}

/****************************************************************************/

u_short TeletextDialog::GetPort(int Index) const
{
	return m_IPPort[Index];
}

/****************************************************************************/

void TeletextDialog::EnableFileControls(bool bEnable)
{
	for (int i = 0; i < _countof(EnableFileControl); i++)
	{
#ifndef __APPLE__
		HWND hwndCtrl = GetDlgItem(m_hwnd, EnableFileControl[i]);
		EnableWindow(hwndCtrl, bEnable);
		ShowWindow(hwndCtrl, bEnable ? SW_SHOW : SW_HIDE);
#else
#endif
		
	}
}

/****************************************************************************/

void TeletextDialog::EnableIPControls(bool bEnable)
{
	for (int i = 0; i < _countof(EnableIPAddressControl); i++)
	{
#ifndef __APPLE__
		HWND hwndCtrl = GetDlgItem(m_hwnd, EnableIPAddressControl[i]);
		EnableWindow(hwndCtrl, bEnable);
		ShowWindow(hwndCtrl, bEnable ? SW_SHOW : SW_HIDE);
#endif
		
	}
}

/****************************************************************************/
