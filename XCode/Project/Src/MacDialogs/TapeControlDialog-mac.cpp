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
//#include "WindowUtils.h"


// Tape control dialog box variables
std::vector<TapeMapEntry> TapeMap;
bool TapeControlEnabled = false;
static HWND hwndTapeControl;
static HWND hwndMap;

static INT_PTR CALLBACK TapeControlDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

static void TapeControlNewTape();
static void TapeControlEjectTape();
static void UpdateState(HWND hwndDlg);

/****************************************************************************/

#include "beebemrcids.h" // convRC2ID
extern std::vector<TapeMapEntry> beeb_TapeMap;
static HWND hwndtc = (HWND)Dialogs::tapeControl;

#define MS_CTS_ON 0x0010

void TapeControlOpenDialog(HINSTANCE hinst, HWND /* hwndMain */)
{
	TapeControlEnabled = true;
    
    if (!hwndTapeControl)
    {
        hwndTapeControl = (HWND)&hwndtc;

        /* dialogue created and shown */
        swift_OpenDialog(Dialogs::tapeControl, NULL);

        
        int Time = SerialGetTapeClock();
        TapeControlAddMapLines();
        TapeControlUpdateCounter(Time);
    }
}

void TapeControlCloseDialog()
{
    DestroyWindow(hwndTapeControl);
	hwndTapeControl = nullptr;
	hwndMap = nullptr;
	TapeControlEnabled = false;
	hCurrentDialog = nullptr;
}

void TapeControlAddMapLines()
{
    beeb_TapeMap.clear();
    beeb_TapeMap = TapeMap;
    swift_TCReload(/*TapeMap*/);

    SendMessage(hwndMap, LB_RESETCONTENT, 0, 0);
    for (const TapeMapEntry& line : TapeMap)
    {
        SendMessage(hwndMap, LB_ADDSTRING, 0, (LPARAM)line.desc.c_str());
    }
    
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
        
        SendMessage(hwndMap, LB_SETCURSEL, (WPARAM)i, 0);
        
		swift_TCSelectItem(i);
	}

}

static void EnableDlgItem(HWND hDlg, UINT nIDDlgItem, bool Enable)
{
    printf("SetDlgItem %d %d", nIDDlgItem, Enable);
    swift_SetDlgItem((Dialogs)*hDlg, ConvRC2ID(nIDDlgItem), Enable);

    EnableWindow(GetDlgItem(hDlg, nIDDlgItem), Enable);
}

static bool IsDlgItemChecked(HWND hDlg, UINT nIDDlgItem)
{
	return SendDlgItemMessage(hDlg, nIDDlgItem, BM_GETCHECK, 0, 0) == BST_CHECKED;
}

static void SetDlgItemChecked(HWND hDlg, UINT nIDDlgItem, bool Checked)
{
	SendDlgItemMessage(hDlg, nIDDlgItem, BM_SETCHECK, Checked ? BST_CHECKED : BST_UNCHECKED, 0);
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
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_NEW_TAPE, true);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_RECORD, false);
            break;

        case SerialTapeState::Recording:
            nIDCheckButton = IDC_RECORDING;

            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_PLAY, false);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_STOP, true);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_EJECT, false);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_REWIND, true);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_LOAD_TAPE, false);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_NEW_TAPE, false);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_RECORD, false);
            break;

        case SerialTapeState::Stopped:
            nIDCheckButton = IDC_STOPPED;

            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_PLAY, true);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_STOP, false);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_EJECT, true);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_REWIND, true);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_LOAD_TAPE, true);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_NEW_TAPE, true);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_RECORD, SerialTapeIsUef());
            break;

        case SerialTapeState::NoTape:
        default:
            nIDCheckButton = IDC_STOPPED;

            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_PLAY, false);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_STOP, false);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_EJECT, false);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_REWIND, false);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_LOAD_TAPE, true);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_NEW_TAPE, true);
            EnableDlgItem(hwndDlg, IDC_TAPE_CONTROL_RECORD, false);
            break;
    }

    mainWin->EnableSaveState(State != SerialTapeState::Recording);

	CheckRadioButton(hwndDlg,
					 IDC_PLAYING,
					 IDC_STOPPED,
					 nIDCheckButton);
}

long TC_WM_COMMAND(UINT32 cmdID)
{
	HWND hwndDlg = (HWND)&hwndtc;

    
    
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
            }
            return FALSE;


        case IDC_TAPE_CONTROL_PLAY:
            SerialStopTapeRecording();
            SerialPlayTape();
            UpdateState(hwndDlg);
            return TRUE;

        case IDC_TAPE_CONTROL_STOP:
            if (TapeState.Recording)
            {
                SerialStopTapeRecording();
                SerialUpdateTapeClock();

                if (SerialTapeIsModified())
                {
                    UEFResult Result = UEFFile.Save(TapeFileName);

                    if (Result != UEFResult::Success)
                    {
                        mainWin->Report(MessageType::Error,
                                        "Failed to write to tape file:\n %s", TapeFileName);
                    }

                    UEFFile.CreateTapeMap(TapeMap);
                    TapeControlAddMapLines();
                }
            }

            SerialStopTape();
            UpdateState(hwndDlg);
            return TRUE;

        case IDC_TAPE_CONTROL_EJECT:
            TapeControlEjectTape();
            UpdateState(hwndDlg);
            return TRUE;

        case IDC_TAPE_CONTROL_REWIND:
            RewindTape();
            UpdateState(hwndDlg);
            return TRUE;

        case IDC_TAPE_CONTROL_LOAD_TAPE:
            mainWin->LoadTape();
            return TRUE;

        case IDC_TAPE_CONTROL_NEW_TAPE:
            TapeControlNewTape();
            TapeControlSetFileName("(Untitled)");
            UEFFile.CreateTapeMap(TapeMap);
            TapeControlAddMapLines();
            UpdateState(hwndDlg);
            return TRUE;

        case IDC_TAPE_CONTROL_RECORD:
            SerialRecordTape();
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

/****************************************************************************/

static void TapeControlEjectTape()
{
    SerialEjectTape();
    TapeControlSetFileName("");
}

/****************************************************************************/

static void TapeControlNewTape()
{
    mainWin->NewTape(TapeFileName, sizeof(TapeFileName));
}

/****************************************************************************/

void TapeControlCloseTape()
{
    beeb_TapeMap.clear();
	swift_TCReload(/*TapeMap*/);

    SendMessage(hwndMap, LB_RESETCONTENT, 0, 0);
    UpdateState(hwndTapeControl);
}

/****************************************************************************/

void TapeControlSetFileName(const char *FileName)
{
    SetDlgItemText(hwndTapeControl, IDC_TAPE_FILENAME, FileName);
}

/****************************************************************************/

void TapeControlSetUnlock(bool Unlock)
{
    SetDlgItemChecked(hwndTapeControl, IDC_TAPE_CONTROL_UNLOCK, Unlock);
}

/****************************************************************************/
