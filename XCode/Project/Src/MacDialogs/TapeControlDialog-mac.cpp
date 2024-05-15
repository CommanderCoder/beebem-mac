//
//  TapeControlDialog-mac.cpp
//  BeebEm
//
//  Created by Commander Coder on 03/05/2024.
//

#include "TapeControlDialog-mac.hpp"

#include <string>
#include "TapeControlDialog.h"
#include "Main.h"
#include "Resource.h"
#include "Serial.h"
extern std::vector<TapeMapEntry> beeb_TapeMap;



// Tape control dialog box variables
std::vector<TapeMapEntry> TapeMap;
bool TapeControlEnabled = false;

static HWND hwndTapeControl;
static HWND hwndMap;

static void TapeControlRecord();
static void UpdateState(HWND hwndDlg);

//static INT_PTR CALLBACK TapeControlDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

void TapeControlOpenDialog(HINSTANCE hinst, HWND /* hwndMain */)
{
	TapeControlEnabled = true;

#ifndef __APPLE__
	if (!IsWindow(hwndTapeControl))
	{
		hwndTapeControl = CreateDialog(hinst, MAKEINTRESOURCE(IDD_TAPECONTROL),
									   NULL, TapeControlDlgProc);
		hCurrentDialog = hwndTapeControl;
		ShowWindow(hwndTapeControl, SW_SHOW);

		hwndMap = GetDlgItem(hwndTapeControl, IDC_TAPE_CONTROL_MAP);
		SendMessage(hwndMap, WM_SETFONT, (WPARAM)GetStockObject(ANSI_FIXED_FONT),
					(LPARAM)MAKELPARAM(FALSE,0));

		int Time = SerialGetTapeClock();
		TapeControlAddMapLines();
		TapeControlUpdateCounter(Time);
	}
#else
	swift_OpenDialog(Dialogs::tapeControl, NULL);
	int Time = SerialGetTapeClock();
	TapeControlAddMapLines();
	TapeControlUpdateCounter(Time);
#endif
}

void TapeControlCloseDialog()
{
#ifndef __APPLE__
	DestroyWindow(hwndTapeControl);
#endif
	hwndTapeControl = nullptr;
	hwndMap = nullptr;
	TapeControlEnabled = false;
	hCurrentDialog = nullptr;
}

void TapeControlAddMapLines()
{
#ifndef __APPLE__
	SendMessage(hwndMap, LB_RESETCONTENT, 0, 0);

	for (const TapeMapEntry& line : TapeMap)
	{
		SendMessage(hwndMap, LB_ADDSTRING, 0, (LPARAM)line.desc.c_str());
	}
#else
	beeb_TapeMap = TapeMap;
	swift_TCReload();
#endif
	UpdateState(hwndTapeControl);
}

void TapeControlUpdateCounter(int tape_time)
{
	if (TapeControlEnabled)
	{
		size_t i = 0;

		while (i < TapeMap.size() && TapeMap[i].time <= tape_time)
			i++;

		if (i > 0)
			i--;
#ifndef __APPLE__
		SendMessage(hwndMap, LB_SETCURSEL, (WPARAM)i, 0);
#else
		swift_TCSelectItem(i);
#endif
		
	}

}

static void SetDlgItemText(HWND hDlg, int nID, const char* str)
{
	printf("tapecontrol text %d %s", nID, str);
}

static void EnableDlgItem(HWND hDlg, UINT nIDDlgItem, bool Enable)
{
#ifndef __APPLE__
	EnableWindow(GetDlgItem(hDlg, nIDDlgItem), Enable);
#endif
}

static bool IsDlgItemChecked(HWND hDlg, UINT nIDDlgItem)
{
	return SendDlgItemMessage(hDlg, nIDDlgItem, BM_GETCHECK, 0, 0) == BST_CHECKED;
}

static void SetDlgItemChecked(HWND hDlg, UINT nIDDlgItem, bool Checked)
{
	SendDlgItemMessage(hDlg, nIDDlgItem, BM_SETCHECK, Checked ? BST_CHECKED : BST_UNCHECKED, 0);
}


static void CheckRadioButton(HWND hwndDlg,
					  int a,
					  int b,
					  int c)
{
	
}

static void UpdateState(HWND hwndDlg)
{
	SetFocus(hwndDlg);

	SerialTapeState State = SerialGetTapeState();

	UINT nIDCheckButton;

	switch (State)
	{
		case SerialTapeState::Playing:
			nIDCheckButton = IDC_PLAYING;

			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_PLAY, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_STOP, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_EJECT, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_REWIND, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_LOAD_TAPE, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_RECORD, false);
			break;

		case SerialTapeState::Recording:
			nIDCheckButton = IDC_RECORDING;

			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_PLAY, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_STOP, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_EJECT, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_REWIND, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_LOAD_TAPE, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_RECORD, false);
			break;

		case SerialTapeState::Stopped:
			nIDCheckButton = IDC_STOPPED;

			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_PLAY, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_STOP, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_EJECT, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_REWIND, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_LOAD_TAPE, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_RECORD, true);
			break;

		case SerialTapeState::NoTape:
		default:
			nIDCheckButton = IDC_STOPPED;

			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_PLAY, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_STOP, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_EJECT, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_REWIND, false);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_LOAD_TAPE, true);
			EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_RECORD, true);
			break;
	}

	CheckRadioButton(hwndDlg,
					 IDC_PLAYING,
					 IDC_STOPPED,
					 nIDCheckButton);
}

#ifndef __APPLE__

INT_PTR CALLBACK TapeControlDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM /* lParam */)
{
	switch (message)
	{
		case WM_INITDIALOG:
			SetDlgItemText(hwndDlg, IDC_TAPE_FILENAME, TapeFileName);
			UpdateState(hwndDlg);
			return TRUE;

		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				hCurrentDialog = nullptr;
			}
			else
			{
				hCurrentDialog = hwndTapeControl;
			}
			return FALSE;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_TAPE_CONTROL_MAP:
					if (HIWORD(wParam) == LBN_SELCHANGE)
					{
						LRESULT s = SendMessage(hwndMap, LB_GETCURSEL, 0, 0);

						if (s != LB_ERR && s >= 0 && s < (int)TapeMap.size())
						{
							SetTapePosition(TapeMap[s].time);
						}
					}
					return FALSE;

				case IDC_TAPE_CONTROL_PLAY:
					SerialStopTapeRecording(true);
					SerialPlayTape();
					UpdateState(hwndDlg);
					return TRUE;

				case IDC_TAPE_CONTROL_STOP:
					SerialStopTapeRecording(true);
					SerialStopTape();
					UpdateState(hwndDlg);
					return TRUE;

				case IDC_TAPE_CONTROL_EJECT:
					SerialStopTapeRecording(false);
					SerialEjectTape();
					SetDlgItemText(hwndDlg, IDC_TAPE_FILENAME, "");
					UpdateState(hwndDlg);
					return TRUE;

				case IDC_TAPE_CONTROL_REWIND:
					RewindTape();
					UpdateState(hwndDlg);
					return TRUE;

				case IDC_TAPE_CONTROL_LOAD_TAPE:
					mainWin->LoadTape();
					return TRUE;

				case IDC_TAPE_CONTROL_RECORD:
					TapeControlRecord();
					UpdateState(hwndDlg);
					return TRUE;

				case IDC_TAPE_CONTROL_UNLOCK: {
					bool Unlock = IsDlgItemChecked(hwndDlg, IDC_TAPE_CONTROL_UNLOCK);
					mainWin->SetUnlockTape(Unlock);
					return TRUE;
				}

				case IDCANCEL:
					TapeControlCloseDialog();
					return TRUE;
			}
	}

	return FALSE;
}
#else


long TCWindowCommandHandler(UINT32 cmdID)
{
	HWND hwndDlg = NULL;
	
	switch(cmdID)
	{
		case IDC_TAPE_CONTROL_MAP:
// Select a row on the tape map
// Handled by tableViewSelectionDidChange()
// and in turn: beeb_getTableCellData
		{
			unsigned int s = swift_TCGetSelected();
			if (s >= 0 && s < (int)TapeMap.size())
			{
				SetTapePosition(TapeMap[s].time);
			}
			return FALSE;
		}
		case IDC_TAPE_CONTROL_PLAY:
			SerialStopTapeRecording(true);
			SerialPlayTape();
			UpdateState(hwndDlg);
			return TRUE;

		case IDC_TAPE_CONTROL_STOP:
			SerialStopTapeRecording(true);
			SerialStopTape();
			UpdateState(hwndDlg);
			return TRUE;

		case IDC_TAPE_CONTROL_EJECT:
			SerialStopTapeRecording(false);
			SerialEjectTape();
			SetDlgItemText(hwndDlg, IDC_TAPE_FILENAME, "");
			UpdateState(hwndDlg);
			return TRUE;

		case IDC_TAPE_CONTROL_REWIND:
			RewindTape();
			UpdateState(hwndDlg);
			return TRUE;

		case IDC_TAPE_CONTROL_LOAD_TAPE:
			mainWin->LoadTape();
			return TRUE;

		case IDC_TAPE_CONTROL_RECORD:
			TapeControlRecord();
			UpdateState(hwndDlg);
			return TRUE;

		case IDC_TAPE_CONTROL_UNLOCK: {
			bool Unlock = IsDlgItemChecked(hwndDlg, IDC_TAPE_CONTROL_UNLOCK);
			mainWin->SetUnlockTape(Unlock);
			return TRUE;
		}

		case IDCANCEL:
			TapeControlCloseDialog();
			return TRUE;
	}

	return false;
}
#endif

void TapeControlRecord()
{
	if (!TapeState.Recording)
	{
		// Query for new file name
		char FileName[_MAX_PATH];
		FileName[0] = '\0';

		if (mainWin->NewTapeImage(FileName, sizeof(FileName)))
		{
			CloseTape();

			// Create file
			if (!SerialRecordTape(FileName))
			{
				mainWin->Report(MessageType::Error,
								"Error creating tape file:\n  %s", FileName);
			}
		}
	}
	UpdateState(hwndTapeControl);
}

void TapeControlCloseTape()
{
#ifndef __APPLE__
	SendMessage(hwndMap, LB_RESETCONTENT, 0, 0);
#else
	beeb_TapeMap.clear();
	swift_TCReload();
#endif
	UpdateState(hwndTapeControl);
}

void TapeControlSetFileName(const char *FileName)
{
	SetDlgItemText(hwndTapeControl, IDC_TAPE_FILENAME, FileName);
}

void TapeControlSetUnlock(bool Unlock)
{
	SetDlgItemChecked(hwndTapeControl, IDC_TAPE_CONTROL_UNLOCK, Unlock);
}
