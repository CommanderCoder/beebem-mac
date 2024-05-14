//
//  RomConfigDialog-mac.cpp
//  BeebEm
//
//  Created by Commander Coder on 29/04/2024.
//

#include "RomConfigDialog-mac.hpp"


// BeebEm ROM Configuration Dialog

#include <windows.h>
#include <commctrl.h>

#include <stdio.h>

#include "RomConfigDialog.h"
#include "BeebMem.h"
#include "BeebWin.h"
#include "FileDialog.h"
#include "ListView.h"
#include "Main.h"
#include "Resource.h"
#include "SysVia.h"


static const char* szModel[] = { "BBC B", "Integra-B", "B Plus", "Master 128" };
static char szDefaultROMPath[MAX_PATH] = {0};
static char szDefaultROMConfigPath[MAX_PATH] = {0};

static bool WriteROMConfigFile(const char *filename, ROMConfigFile RomConfig);



RCItem beeb_RCTable[16][2];




static int LVInsertItem(
	HWND hWnd, UINT uRow, UINT uCol, const LPTSTR pszText, LPARAM lParam)
{
	beeb_RCTable[uRow][uCol].name = pszText;
	beeb_RCTable[uRow][uCol].bank = (int)lParam;
	return 0;
}

static void LVSetItemText(
	HWND hWnd, UINT uRow, UINT uCol, const LPTSTR pszText)
{
	// change the row/column data which is read by swift dialogue
	beeb_RCTable[uRow][uCol].name = pszText;
}

static void LVSetFocus(HWND hWnd)
{
	swift_RCSetFocus();
}


/****************************************************************************/

RomConfigDialog::RomConfigDialog(HINSTANCE hInstance,
								 HWND hwndParent,
								 ROMConfigFile Config) :
	Dialog(hInstance, hwndParent, IDD_ROMCONFIG),
	m_hWndROMList(nullptr),
	m_hWndModel(nullptr),
	m_Model(MachineType)
{
	memcpy(m_RomConfig, Config, sizeof(ROMConfigFile));

}

/****************************************************************************/

const ROMConfigFile* RomConfigDialog::GetRomConfig() const
{
	return &m_RomConfig;
}

/****************************************************************************/

void RomConfigDialog::UpdateROMField(int Row)
{
	char szROMFile[_MAX_PATH];
	bool Unplugged = false;
	int Bank;

	if (m_Model == Model::Master128)
	{
		Bank = 16 - Row;

		if (Bank >= 0 && Bank <= 7)
		{
			Unplugged = (CMOSRAM[20] & (1 << Bank)) == 0;
		}
		else if (Bank >= 8 && Bank <= 15)
		{
			Unplugged = (CMOSRAM[21] & (1 << (Bank - 8))) == 0;
		}
	}

	strncpy(szROMFile, m_RomConfig[static_cast<int>(m_Model)][Row], _MAX_PATH);

	if (Unplugged)
	{
		strncat(szROMFile, " (unplugged)", _MAX_PATH);
	}

	LVSetItemText(m_hWndROMList, Row, 1, szROMFile);
}

/****************************************************************************/

void ListView_DeleteAllItems(HWND wnd)
{
	for (int r = 0; r<16;r++)
	{
		for (int c = 0; c<2;c++)
		{
			beeb_RCTable[r][c] = RCItem();
		}
	}
}

static void ComboBox_AddString(UINT* nIDDlgItem,
							   std::string b)
{
//	swift_AddStringRC(b);
}


static void ComboBox_SetCurSel(UINT* nIDDlgItem,
							   INT b)
{
	swift_SetCurSelRC(b);
}
/****************************************************************************/

void RomConfigDialog::FillModelList()
{
	HWND hWndModel = GetDlgItem(m_hwnd, IDC_MODEL);

	for (int i = 0; i < static_cast<int>(Model::Last); i++)
	{
		ComboBox_AddString(hWndModel, GetModelName(static_cast<Model>(i)));
	}

	ComboBox_SetCurSel(hWndModel, static_cast<int>(m_Model));
}


void RomConfigDialog::FillROMList()
{
	ListView_DeleteAllItems(m_hWndROMList);

	int Row = 0;
	LVInsertItem(m_hWndROMList, Row, 0, "OS", 16);
	LVSetItemText(m_hWndROMList, Row, 1, m_RomConfig[static_cast<int>(m_Model)][0]);

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

extern RomConfigDialog* runningRCDialog;


bool RomConfigDialog::DoModal() {
	runningRCDialog = this;
	WM_INITDIALOG();
	bool ret = swift_DoModalRC(this);
	runningRCDialog = NULL;
	return ret;
}


bool RomConfigDialog::WM_INITDIALOG()
{
	FillModelList();
	FillROMList();
	return TRUE;
}


int ListView_GetSelectionMark(HWND hwnd)
{
	return swift_RCGetSelectionMark();
}


bool RomConfigDialog::WM_COMMAND(WPARAM wParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_MODEL:{
		// model is the currently selected Model
		// from the model combo box
		int model = HIWORD(wParam);
		m_Model = static_cast<Model>(model);
		FillROMList();
			return TRUE;
	}
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

				strcpy(m_RomConfig[static_cast<int>(m_Model)][Row], szROMFile);
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
			char *cfg = m_RomConfig[static_cast<int>(m_Model)][Row];

			if (strcmp(cfg, BANK_EMPTY) != 0 && strcmp(cfg, BANK_RAM) != 0)
			{
				if (strlen(cfg) > 4 && strcmp(cfg + strlen(cfg) - 4, ROM_WRITABLE) == 0)
					cfg[strlen(cfg) - 4] = 0;
				else
					strcat(cfg, ROM_WRITABLE);

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
			strcpy(m_RomConfig[static_cast<int>(m_Model)][Row], BANK_RAM);
			UpdateROMField(Row);
		}

		LVSetFocus(m_hWndROMList);
		break;
	}

	case IDC_EMPTY: {
		int Row = ListView_GetSelectionMark(m_hWndROMList);

		if (Row >= 1 && Row <= 16)
		{
			strcpy(m_RomConfig[static_cast<int>(m_Model)][Row], BANK_EMPTY);
			UpdateROMField(Row);
		}

		LVSetFocus(m_hWndROMList);
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

	FileDialog fileDialog(m_hwnd, szROMConfigPath, MAX_PATH, DefaultPath, filter);

	if (fileDialog.Open())
	{
		// Save directory as default for next time
		unsigned int PathLength = (unsigned int)(strrchr(szROMConfigPath, '\\') - szROMConfigPath);
		strncpy(szDefaultROMConfigPath, szROMConfigPath, PathLength);
		szDefaultROMConfigPath[PathLength] = 0;

		// Read the file
		ROMConfigFile LoadedROMCfg;
		if (ReadROMConfigFile(szROMConfigPath, LoadedROMCfg))
		{
			// Copy in loaded config
			memcpy(&m_RomConfig, &LoadedROMCfg, sizeof(ROMConfigFile));
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

	FileDialog fileDialog(m_hwnd, szROMConfigPath, MAX_PATH, DefaultPath, filter);

	if (fileDialog.Save())
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
		if (WriteROMConfigFile(szROMConfigPath, m_RomConfig))
		{
			success = true;
		}
	}

	return success;
}

/****************************************************************************/

static bool WriteROMConfigFile(const char *filename, ROMConfigFile ROMConfig)
{
	FILE *fd = fopen(filename, "w");
	if (!fd)
	{
		mainWin->Report(MessageType::Error,
						"Failed to write ROM configuration file:\n  %s", filename);

		return false;
	}

	for (int Model = 0; Model < 4; ++Model)
	{
		for (int Bank = 0; Bank < 17; ++Bank)
		{
			fprintf(fd, "%s\n", ROMConfig[Model][Bank]);
		}
	}

	fclose(fd);

	return true;
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

	FileDialog fileDialog(m_hwnd, pszFileName, MAX_PATH, DefaultPath, filter);

	if (fileDialog.Open())
	{
		// Save directory as default for next time
//		unsigned int PathLength = (unsigned int)(strrchr(pszFileName, '\\') - pszFileName);
		unsigned int PathLength = (unsigned int)(strrchr(pszFileName, '/') - pszFileName);
		strncpy(szDefaultROMPath, pszFileName, PathLength);
		szDefaultROMPath[PathLength] = 0;

		success = true;
	}

	return success;
}
