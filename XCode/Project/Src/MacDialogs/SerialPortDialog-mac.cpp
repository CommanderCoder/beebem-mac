//
//  SerialPortDialog-mac.cpp
//  BeebEm
//
//  Created by Commander Coder on 12/05/2024.
//

#include "SerialPortDialog-mac.hpp"


#include <windows.h>

#include <string>
#include <vector>

#include "SerialPortDialog.h"
#include "Main.h"
#include "Resource.h"
#include "StringUtils.h"

/****************************************************************************/

SerialPortDialog::SerialPortDialog(HINSTANCE hInstance,
								   HWND hwndParent,
								   SerialType Destination,
								   const char* PortName,
								   const char* IPAddress,
								   int IPPort,
								   bool IP232RawComms,
								   bool IP232Handshake) :
	Dialog(hInstance, hwndParent, IDD_SERIAL),
	m_Destination(Destination),
	m_SerialPortName(PortName),
	m_IPAddress(IPAddress),
	m_IPPort(IPPort),
	m_IP232RawComms(IP232RawComms),
	m_IP232Handshake(IP232Handshake)
{
}


static void SetDlgItemText(HWND hDlg, int nID, const char* str)
{
	printf("tapecontrol text %d %s", nID, str);
}

static void EnableDlgItem(UINT nIDDlgItem, bool Enable)
{
#ifndef __APPLE__
	EnableWindow(GetDlgItem(hDlg, nIDDlgItem), Enable);
#endif
}

static bool IsDlgItemChecked(HWND hDlg, UINT nIDDlgItem)
{
#ifndef __APPLE__
	return SendDlgItemMessage(hDlg, nIDDlgItem, BM_GETCHECK, 0, 0) == BST_CHECKED;
#else
	return FALSE;
#endif
}

static void SetDlgItemText(int nID, const std::string& str)
{
	
}
static std::string GetDlgItemText(int nID)
{
	return "";
}

static void SetDlgItemFocus(int nID)
{
	
}

static bool IsDlgItemChecked(UINT nIDDlgItem)
{
	return false;
}


static void SetDlgItemChecked(UINT nIDDlgItem, bool Checked)
{
#ifndef __APPLE__
	SendDlgItemMessage(hDlg, nIDDlgItem, BM_SETCHECK, Checked ? BST_CHECKED : BST_UNCHECKED, 0);
#endif
}


static void CheckRadioButton(HWND hwndDlg,
					  int a,
					  int b,
					  int c)
{
	
}

/****************************************************************************/
//
//INT_PTR SerialPortDialog::DlgProc(UINT   nMessage,
//								  WPARAM wParam,
//								  LPARAM /* lParam */)
//{
//	switch (nMessage)
//	{
//	case WM_INITDIALOG: {
bool SerialPortDialog::WM_INITDIALOG()
{
		int ID = SerialDestination == SerialType::SerialPort ? IDC_SERIAL_PORT_DESTINATION_SERIAL_PORT :
				 SerialDestination == SerialType::TouchScreen ? IDC_SERIAL_PORT_DESTINATION_TOUCH_SCREEN :
				 IDC_SERIAL_PORT_DESTINATION_IP;

		CheckRadioButton(m_hwnd,
						 IDC_SERIAL_PORT_DESTINATION_SERIAL_PORT,
						 IDC_SERIAL_PORT_DESTINATION_IP,
						 ID);

		SetDlgItemText(IDC_SERIAL_PORT, m_SerialPortName);

		SetDlgItemText(IDC_IP_ADDRESS, m_IPAddress);

		char sz[20];
		sprintf(sz, "%d", m_IPPort);
		SetWindowText(GetDlgItem(m_hwnd, IDC_IP_PORT), sz);

		SetDlgItemChecked(IDC_IP323_RAW_COMMS, m_IP232RawComms);
		SetDlgItemChecked(IDC_IP232_HANDSHAKE, m_IP232Handshake);

		EnableSerialPortControls(SerialDestination == SerialType::SerialPort);
		EnableIP232Controls(SerialDestination == SerialType::IP232);
		return TRUE;
}

//}
//
//	case WM_COMMAND:
bool SerialPortDialog::WM_COMMAND(WPARAM wParam)
{
		switch (LOWORD(wParam))
		{
		case IDOK: {
			if (m_Destination == SerialType::SerialPort)
			{
				m_SerialPortName = GetDlgItemText(IDC_SERIAL_PORT);

				if (m_SerialPortName.empty())
				{
					mainWin->Report(MessageType::Error, "Please enter a valid serial port name");
					SetDlgItemFocus(IDC_SERIAL_PORT);
					return FALSE;
				}
			}
			else if (m_Destination == SerialType::IP232)
			{
				m_IPAddress = GetDlgItemText(IDC_IP_ADDRESS);

				unsigned long Address = inet_addr(m_IPAddress.c_str());

				if (Address == INADDR_NONE)
				{
					mainWin->Report(MessageType::Error, "Please enter a valid IP address");
					SetDlgItemFocus(IDC_IP_ADDRESS);
					return FALSE;
				}

				std::string Port = GetDlgItemText(IDC_IP_PORT);

				bool bSuccess = ParseNumber(Port, &m_IPPort);

				if (!bSuccess || m_IPPort > 65535)
				{
					mainWin->Report(MessageType::Error, "Please enter a valid port number");
					SetDlgItemFocus(IDC_IP_PORT);
					return FALSE;
				}

				m_IP232RawComms = IsDlgItemChecked(IDC_IP323_RAW_COMMS);
				m_IP232Handshake = IsDlgItemChecked(IDC_IP232_HANDSHAKE);
			}

			EndDialog(m_hwnd, wParam);
			break;
		}
		
		case IDCANCEL:
			EndDialog(m_hwnd, wParam);
			return TRUE;

		case IDC_SERIAL_PORT_DESTINATION_SERIAL_PORT:
			m_Destination = SerialType::SerialPort;
			EnableSerialPortControls(true);
			EnableIP232Controls(false);
			break;

		case IDC_SERIAL_PORT_DESTINATION_TOUCH_SCREEN:
			m_Destination = SerialType::TouchScreen;
			EnableSerialPortControls(false);
			EnableIP232Controls(false);
			break;

		case IDC_SERIAL_PORT_DESTINATION_IP:
			m_Destination = SerialType::IP232;
			EnableSerialPortControls(false);
			EnableIP232Controls(true);
			break;
		}
	return FALSE;
}
//	}
//
//	return FALSE;
//}


/****************************************************************************/

void SerialPortDialog::EnableSerialPortControls(bool bEnable)
{
	EnableDlgItem(IDC_SERIAL_PORT, bEnable);
}

/****************************************************************************/

void SerialPortDialog::EnableIP232Controls(bool bEnable)
{
	EnableDlgItem(IDC_IP_ADDRESS, bEnable);
	EnableDlgItem(IDC_IP_PORT, bEnable);
	EnableDlgItem(IDC_IP323_RAW_COMMS, bEnable);
	EnableDlgItem(IDC_IP232_HANDSHAKE, bEnable);
}

/****************************************************************************/

