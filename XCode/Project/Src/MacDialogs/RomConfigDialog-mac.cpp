//
//  RomConfigDialog-mac.cpp
//  BeebEm
//
//  Created by Commander Coder on 29/04/2024.
//

#include "RomConfigDialog-mac.hpp"

#include <vector>

#define MAXROWS 20
#define MAXCOLUMNS 4
std::string beeb_RCTable[MAXROWS][MAXCOLUMNS];
int lastRow = 0;

const char* GetRCEntry(UINT uRow, UINT uCol)
{
	if (uRow>=MAXROWS || uCol>=MAXCOLUMNS)
		return nullptr;
	return beeb_RCTable[uRow][uCol].c_str();
}

static int LVInsertItem(
	HWND hWnd, UINT uRow, UINT uCol, const LPTSTR pszText, LPARAM lParam)
{
	if (uRow>=MAXROWS || uCol>=MAXCOLUMNS || uRow < 0 || uCol < 0)
		return 1;

	beeb_RCTable[uRow][uCol] = pszText;
	if (lastRow<uRow)
		lastRow = uRow;
	return 0;
}

static void LVSetItemText(
	HWND hWnd, UINT uRow, UINT uCol, const LPTSTR pszText)
{
	if (uRow>=MAXROWS || uCol>=MAXCOLUMNS || uRow < 0 || uCol < 0)
		return;
	
	// change the row/column data which is read by swift dialogue
	beeb_RCTable[uRow][uCol] = pszText;
}

static void LVSetFocus(HWND hWnd)
{
	swift_SetFocus((Modals)*hWnd);
}


void ListView_DeleteAllItems(HWND wnd)
{
	for (int r = 0; r<MAXROWS;r++)
	{
		for (int c = 0; c<MAXCOLUMNS;c++)
		{
			beeb_RCTable[r][c] = "";
		}
	}
	lastRow=0;
}

static void ComboBox_AddString(UINT* nIDDlgItem,
							   std::string b)
{
//	swift_AddString(m_DialogID, nIDDlgItem, b);
}

static void ComboBox_SetCurSel(UINT* nIDDlgItem,
							   INT b)
{
	swift_SetCurSel((Modals)*nIDDlgItem, b);
}


int ListView_GetSelectionMark(HWND hwnd)
{
	return swift_GetSelectionMark((Modals)*hwnd);
}

void ListView_SetSelectionMark(HWND hwnd, int sel)
{
	swift_SetSelectionMark((Modals)*hwnd, sel);
}

/****************************************************************************/
static void EndDialog(HWND h, WPARAM p)
{
	swift_EndModal(p);
}

static int ListView_GetItemCount(HWND a)
{
	return lastRow; // number of items in the listview
}

//static void ListView_SetItemState(HWND a, int c, int x, int y)
//{
	// set state of item in the listview
//}

extern RomConfigDialog* runningRCDialog;

bool RomConfigDialog::DoModal() {
	runningRCDialog = this;
	WM_INITDIALOG();
	bool ret = swift_DoModal((Modals)m_DialogID, this);
	runningRCDialog = NULL;
	return ret;
}



// BeebEm ROM Configuration Dialog

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#include <algorithm>
#include <stdio.h>

#include "RomConfigDialog.h"
#include "BeebMem.h"
#include "BeebWin.h"
#include "FileDialog.h"
#include "ListView.h"
#include "Main.h"
#include "Model.h"
#include "Resource.h"
#include "StringUtils.h"
#include "Rtc.h"


#undef IDD_ROMCONFIG
#define IDD_ROMCONFIG			Modals::romConfig

static char szDefaultROMPath[MAX_PATH] = {0};
static char szDefaultROMConfigPath[MAX_PATH] = {0};

/****************************************************************************/

RomConfigDialog::RomConfigDialog(HINSTANCE hInstance,
                                 HWND hwndParent,
                                 const RomConfigFile& Config) :
	Dialog(hInstance, hwndParent, IDD_ROMCONFIG),
	m_hWndROMList(nullptr),
	m_hWndModel(nullptr),
	m_Model(MachineType),
	m_RomConfig(Config)
{
}

/****************************************************************************/

const RomConfigFile& RomConfigDialog::GetRomConfig() const
{
	return m_RomConfig;
}

/****************************************************************************/

void RomConfigDialog::UpdateROMField(int Row)
{
	bool Unplugged = false;
	int Bank;

	if (m_Model == Model::Master128 || m_Model == Model::MasterET)
	{
		Bank = 16 - Row;

		if (Bank >= 0 && Bank <= 7)
		{
			Unplugged = (RTCGetData(m_Model, 20) & (1 << Bank)) == 0;
		}
		else if (Bank >= 8 && Bank <= 15)
		{
			Unplugged = (RTCGetData(m_Model, 21) & (1 << (Bank - 8))) == 0;
		}
	}

	std::string RomFileName = m_RomConfig.GetFileName(m_Model, Row);

	if (Unplugged)
	{
		RomFileName += " (unplugged)";
	}

	LVSetItemText(m_hWndROMList, Row, 1, (LPTSTR)RomFileName.c_str());
}

/****************************************************************************/

void RomConfigDialog::FillModelList()
{
	HWND hWndModel = GetDlgItem(m_hwnd, IDC_MODEL);
	hWndModel = (HWND)&m_DialogID;

	for (int i = 0; i < MODEL_COUNT; i++)
	{
		ComboBox_AddString(hWndModel, GetModelName(static_cast<Model>(i)));
	}

	ComboBox_SetCurSel(hWndModel, static_cast<int>(m_Model));
}

/****************************************************************************/

void RomConfigDialog::FillROMList()
{
	ListView_DeleteAllItems(m_hWndROMList);

	int Row = 0;
	LVInsertItem(m_hWndROMList, Row, 0, "OS", 16);
	LVSetItemText(m_hWndROMList, Row, 1, (LPTSTR)m_RomConfig.GetFileName(m_Model, 0).c_str());

	for (Row = 1; Row <= 16; ++Row)
	{
		int Bank = 16 - Row;

		char str[20];
		sprintf(str, "%02d (%X)", Bank, Bank);

		LVInsertItem(m_hWndROMList, Row, 0, str, Bank);
		UpdateROMField(Row);
	}
}

/****************************************************************************/

#ifndef __APPLE__
INT_PTR RomConfigDialog::DlgProc(UINT   nMessage,
                                 WPARAM wParam,
                                 LPARAM /* lParam */)
{
	switch (nMessage)
	{
		case WM_INITDIALOG:
			m_hWndROMList = GetDlgItem(m_hwnd, IDC_ROMLIST);

			ListView_SetExtendedListViewStyle(m_hWndROMList, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
			LVInsertColumn(m_hWndROMList, 0, "Bank", LVCFMT_LEFT, 45);
			LVInsertColumn(m_hWndROMList, 1, "ROM File", LVCFMT_LEFT, 200);
			ListView_SetColumnWidth(m_hWndROMList, 1, LVSCW_AUTOSIZE_USEHEADER);
#else
bool RomConfigDialog::WM_INITDIALOG()
{
			m_hWndROMList = (uint*)&m_DialogID;

#endif
			FillModelList();
			FillROMList();
			return TRUE;
#ifdef __APPLE__
}

#define CBN_SELCHANGE 1
#define ComboBox_GetCurSel(_x_) model
			
bool RomConfigDialog::WM_COMMAND(WPARAM wParam)
{
	int nDlgItemID = GET_WM_COMMAND_ID(wParam, lParam);
	int Notification = CBN_SELCHANGE;
	int model = HIWORD(wParam);	// model is the currently selected Model from the model combo box
#else
		case WM_COMMAND: {
			int nDlgItemID = GET_WM_COMMAND_ID(wParam, lParam);
			int Notification = GET_WM_COMMAND_CMD(wParam, lParam);
#endif

			switch (nDlgItemID)
			{
			case IDC_MODEL:
				if (Notification == CBN_SELCHANGE)
				{
					HWND hWndModelCombo = GetDlgItem(m_hwnd, IDC_MODEL);
					m_Model = static_cast<Model>(ComboBox_GetCurSel(hWndModelCombo));
					FillROMList();
				}
				break;

			case IDC_SELECTROM: {
				int Row = ListView_GetSelectionMark(m_hWndROMList);

				if (Row >= 0 && Row <= 16)
				{
					char szROMFile[MAX_PATH];
					szROMFile[0] = '\0';

					if (GetROMFile(szROMFile))
					{
						// Strip user data path
						char szROMPath[MAX_PATH];
						strcpy(szROMPath, "BeebFile");
						mainWin->GetDataPath(mainWin->GetUserDataPath(), szROMPath);

						int nROMPathLen = (int)strlen(szROMPath);

						if (strncmp(szROMFile, szROMPath, nROMPathLen) == 0)
						{
							strcpy(szROMFile, szROMFile + nROMPathLen + 1);
						}

						m_RomConfig.SetFileName(m_Model, Row, szROMFile);
						UpdateROMField(Row);
					}
				}

				LVSetFocus(m_hWndROMList);
				break;
			}

			case IDC_MARKWRITABLE: {
				int Row = ListView_GetSelectionMark(m_hWndROMList);

				if (Row >= 1 && Row <= 16)
				{
					std::string RomFileName = m_RomConfig.GetFileName(m_Model, Row);

					if (RomFileName != BANK_EMPTY && RomFileName != BANK_RAM)
					{
						if (StringEndsWith(RomFileName, ROM_WRITABLE))
						{
							RomFileName = RomFileName.substr(0, RomFileName.size() - strlen(ROM_WRITABLE));
						}
						else
						{
							RomFileName += ROM_WRITABLE;
						}

						m_RomConfig.SetFileName(m_Model, Row, RomFileName);

						UpdateROMField(Row);
					}
				}

				LVSetFocus(m_hWndROMList);
				break;
			}

			case IDC_RAM: {
				int Row = ListView_GetSelectionMark(m_hWndROMList);

				if (Row >= 1 && Row <= 16)
				{
					m_RomConfig.SetFileName(m_Model, Row, BANK_RAM);
					UpdateROMField(Row);
				}

				LVSetFocus(m_hWndROMList);
				break;
			}

			case IDC_EMPTY: {
				int Row = ListView_GetSelectionMark(m_hWndROMList);

				if (Row >= 1 && Row <= 16)
				{
					m_RomConfig.SetFileName(m_Model, Row, BANK_EMPTY);
					UpdateROMField(Row);
				}

				LVSetFocus(m_hWndROMList);
				break;
			}

			case IDC_UP: {
				int Row = ListView_GetSelectionMark(m_hWndROMList);

				if (Row >= 2)
				{
					int PreviousRow = Row - 1;

					m_RomConfig.Swap(m_Model, PreviousRow, Row);

					UpdateROMField(PreviousRow);
					UpdateROMField(Row);

					ListView_SetSelectionMark(m_hWndROMList, PreviousRow);
				}

				LVSetFocus(m_hWndROMList);
				break;
			}

			case IDC_DOWN: {
				int Row = ListView_GetSelectionMark(m_hWndROMList);

				if (Row > 0)
				{
					const int NextRow = Row + 1;
					const int Count = ListView_GetItemCount(m_hWndROMList);

					if (NextRow < Count)
					{
						m_RomConfig.Swap(m_Model, Row, NextRow);

						UpdateROMField(Row);
						UpdateROMField(NextRow);

						ListView_SetSelectionMark(m_hWndROMList, NextRow);
					}

					LVSetFocus(m_hWndROMList);
				}
				break;
			}

			case IDC_SAVE:
				SaveROMConfigFile();
				LVSetFocus(m_hWndROMList);
				break;

			case IDC_LOAD:
				LoadROMConfigFile();
				LVSetFocus(m_hWndROMList);
				break;

			case IDOK:
				EndDialog(m_hwnd, TRUE);
				return TRUE;

			case IDCANCEL:
				EndDialog(m_hwnd, FALSE);
				return TRUE;
			}
#ifndef __APPLE__
			break;
		}
	}
#endif

	return FALSE;
}

/****************************************************************************/

bool RomConfigDialog::LoadROMConfigFile()
{
	char DefaultPath[MAX_PATH];
	char szROMConfigPath[MAX_PATH];
	szROMConfigPath[0] = '\0';
	bool success = false;
	const char* filter = "ROM Config File (*.cfg)\0*.cfg\0";

	if (szDefaultROMConfigPath[0] != '\0')
	{
		strcpy(DefaultPath, szDefaultROMConfigPath);
	}
	else
	{
		strcpy(DefaultPath, mainWin->GetUserDataPath());
	}

	FileDialog Dialog(m_hwnd, szROMConfigPath, MAX_PATH, DefaultPath, filter);

	if (Dialog.Open())
	{
		// Save directory as default for next time
		unsigned int PathLength = (unsigned int)(strrchr(szROMConfigPath, '\\') - szROMConfigPath);
		strncpy(szDefaultROMConfigPath, szROMConfigPath, PathLength);
		szDefaultROMConfigPath[PathLength] = 0;

		// Read the file
		RomConfigFile LoadedRomConfig;
		if (LoadedRomConfig.Load(szROMConfigPath))
		{
			// Copy in loaded config
			m_RomConfig = LoadedRomConfig;
			FillROMList();
			success = true;
		}
	}

	return success;
}

/****************************************************************************/

bool RomConfigDialog::SaveROMConfigFile()
{
	char DefaultPath[MAX_PATH];
	char szROMConfigPath[MAX_PATH];
	szROMConfigPath[0] = '\0';
	bool success = false;
	const char* filter = "ROM Config File (*.cfg)\0*.cfg\0";

	if (szDefaultROMConfigPath[0] != '\0')
	{
		strcpy(DefaultPath, szDefaultROMConfigPath);
	}
	else
	{
		strcpy(DefaultPath, mainWin->GetUserDataPath());
	}

	FileDialog Dialog(m_hwnd, szROMConfigPath, MAX_PATH, DefaultPath, filter);

	if (Dialog.Save())
	{
		// Save directory as default for next time
		unsigned int PathLength = (unsigned int)(strrchr(szROMConfigPath, '\\') - szROMConfigPath);
		strncpy(szDefaultROMConfigPath, szROMConfigPath, PathLength);
		szDefaultROMConfigPath[PathLength] = 0;

		// Add a file extension if required
		if (strchr(szROMConfigPath, '.') == NULL)
		{
			strcat(szROMConfigPath, ".cfg");
		}

		// Save the file
		if (m_RomConfig.Save(szROMConfigPath))
		{
			success = true;
		}
	}

	return success;
}

/****************************************************************************/

bool RomConfigDialog::GetROMFile(char *pszFileName)
{
	char DefaultPath[MAX_PATH];
	char szROMPath[MAX_PATH];
	bool success = false;
	const char* filter = "ROM File (*.rom)\0*.rom\0";

	strcpy(szROMPath, "BeebFile");
	mainWin->GetDataPath(mainWin->GetUserDataPath(), szROMPath);

	if (szDefaultROMPath[0])
		strcpy(DefaultPath, szDefaultROMPath);
	else
		strcpy(DefaultPath, szROMPath);

	FileDialog Dialog(m_hwnd, pszFileName, MAX_PATH, DefaultPath, filter);

	if (Dialog.Open())
	{
		// Save directory as default for next time
#ifndef __APPLE__
		unsigned int PathLength = (unsigned int)(strrchr(pszFileName, '\\') - pszFileName);
#else
		unsigned int PathLength = (unsigned int)(strrchr(pszFileName, '/') - pszFileName);
#endif
		strncpy(szDefaultROMPath, pszFileName, PathLength);
		szDefaultROMPath[PathLength] = 0;

		success = true;
	}

	return success;
}
