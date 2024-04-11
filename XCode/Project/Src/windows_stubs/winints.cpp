//
//  winints.cpp
//  BeebEm
//
//  Created by Commander Coder on 09/04/2024.
//

#include <stdio.h>
#include <windows.h>
#include "beebemrcids.h"

int __argc;
char** __argv;

DWORD GetTickCount() // milliseconds
{
	auto since_epoch = std::chrono::steady_clock::now().time_since_epoch();
	auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);
	auto millis = milli.count();
	return (DWORD) millis;
}



void beebwin_ModifyMenu(
						UINT position,
						UINT newitem,
						CHAR* newtext)
{
	auto cmdID = RC2ID.find(position);
	auto newID = RC2ID.find(newitem);
	if (cmdID != RC2ID.end())
	{
		// check the selected item
		swift_ModifyMenu(cmdID->second, newID->second, newtext);
	}

}



// set the tick on the menu with a 4 character identifier
void beebwin_SetMenuCheck(UINT cmd, bool check)
{
	auto cmdID = RC2ID.find(cmd);
	if (cmdID != RC2ID.end())
	{
		// check the selected item
		swift_SetMenuCheck(cmdID->second, check);
	}
	
}

// set the tick on the menu with a 4 character identifier
void beebwin_CheckMenuRadioItem(UINT first, UINT last, UINT cmd)
{
	for (UINT v = first; v <= last; v++)
	{
		// uncheck all the items in the 'radio'
		beebwin_SetMenuCheck(v, false);
	}
	// check the selected item
	beebwin_SetMenuCheck(cmd, true);
}



int _vscprintf (const char * format, va_list pargs) {
	int retval;
	va_list argcopy;
	va_copy(argcopy, pargs);
	retval = vsnprintf(NULL, 0, format, argcopy);
	va_end(argcopy);
	return retval;
 }

#include "FileUtils.h"

void _splitpath(const char *path,
				char *drive,
				char *dir,
				char *fname,
				char *ext)
{
	char d[300];
	char n[300];
	char e[300];
	sscanf(path, "%s/%s.%s", d,n,e);
	if (dir)
		strcpy(dir, d);
	if (fname)
		strcpy(fname, n);
	if (ext)
		strcpy(ext, e);
}

void _makepath(char *path,
			   const char *drive,
	  const char *dir,
	  const char *fname,
	  const char *ext)
{
	if (fname && ext)
		sprintf(path, "%s/%s.%s", dir,fname,ext);
	else
		sprintf(path, "%s/", dir);
}

bool SHGetFolderPath(const char* path)
{
	// look for path
	bool folderFound = FolderExists(path);
	
	if (!folderFound)
	{
		// replace the path with the path to the users DOCUMENTS
		// folder and return 'true'
	}

	return false;
}



#include <Carbon/Carbon.h>
#include "BeebWin.h"
#include "6502core.h"
#include "Disc8271.h"
#include "Disc1770.h"
#include "Econet.h"
#include "Scsi.h"
#include "Sasi.h"
#include "IDE.h"
#include "Sound.h"
#include "Music5000.h"
#include "Tube.h"
#include "Master512CoPro.h"
#include "Z80mem.h"
#include "Z80.h"
#include "SprowCoPro.h"
#include "Teletext.h"
#include "SysVia.h"
extern BeebWin* mainWin;


int beebwin_KeyUpDown(long message, long wParam, long lParam)
{
	int row, col;

	switch (message)
	{
		case kEventRawKeyDown:
//          fprintf(stderr, "Key pressed: code = %d, '%c'\n", wParam, lParam);
			// Reset shift state if it was set by Run Disc
			if (mainWin->m_ShiftBooted)
			{
				mainWin->m_ShiftBooted = false;
				BeebKeyUp(0, 0);
			}

			mainWin->TranslateKey((int)wParam, false, row, col);
			break;
		case kEventRawKeyUp:
//          fprintf(stderr, "Key released: code = %d, '%c'\n", wParam, lParam);
				if (mainWin->TranslateKey((int)wParam, true, row, col) < 0)
				{
					if (row == -2)
					{
						// Must do a reset!
						Init6502core();

						if (TubeType == Tube::Acorn65C02)
						{
							Init65C02core();
						}
						else if (TubeType == Tube::Master512CoPro)
						{
							master512CoPro.Reset();
						}
						else if (TubeType == Tube::AcornZ80 || TubeType == Tube::TorchZ80)
						{
							R1Status = 0;
							ResetTube();
							init_z80();
						}
						else if (TubeType == Tube::AcornArm)
						{
							R1Status = 0;
							ResetTube();
							mainWin->DestroyArmCoPro();
							mainWin->CreateArmCoPro();
						}
						else if (TubeType == Tube::SprowArm)
						{
							R1Status = 0;
							ResetTube();
							// We don't want to throw the contents of memory away
							// just tell the co-pro to reset itself.
							sprow->Reset();
						}

						Disc8271Reset();
						Reset1770();
						if (EconetEnabled) EconetReset();//Rob
						if (SCSIDriveEnabled) SCSIReset();
						if (SCSIDriveEnabled) SASIReset();
						if (IDEDriveEnabled)  IDEReset();
						TeletextInit();
						//SoundChipReset();
						Music5000Reset();
						if (Music5000Enabled)
							Music5000Init();
					}
					else if(row==-3)
					{
						if (col==-3) SoundTuning+=0.1; // Page Up
						if (col==-4) SoundTuning-=0.1; // Page Down
					}
			}
			break;
		case kEventRawKeyModifiersChanged:
//            fprintf(stderr, "Key modifier : code = %016x\n", wParam);
			
			static long last_wParam = 0;
			
			long diff_wParam = wParam ^ last_wParam; // XOR - to find what changed
			
			// bitpatterns
			// 0000 0010 0000 0000 0000 0010 - L SHIFT
			// 0000 0010 0000 0000 0000 0100 - R SHIFT
			// 0000 0100 0000 0000 0000 0001 - CTRL
			// 0000 1000 0000 0000 0010 0000 - L ALT
			// 0000 1000 0000 0000 0100 0000 - R ALT
			// 0000 0001 0000 0000 0000 0000 - CAPS
			// 1000 0000 0000 0000 0000 0000 - FN
			// 0001 0000 0000 0000 0000 1000 - L CMD
			// 0001 0000 0000 0000 0001 0000 - R CMD

#define SHIFTMASK 0x20000
#define ALTMASK 0x80000
#define CTRLMASK 0x40000
#define CMDMASK 0x100000
#define CAPSMASK 0x10000
#define FNMASK 0x800000
			
			if ((diff_wParam & SHIFTMASK)!=0) // left and right shift key
			{
				// UP when mask is 0, DOWN if mask is 1
				mainWin->TranslateKey((int)128, (wParam & SHIFTMASK)==0, row, col);
			}
			// APPLE CTRL KEY
			if ((diff_wParam & CTRLMASK)!=0) // left and right ctrl key
			{
				// UP when mask is 0, DOWN if mask is 1
				mainWin->TranslateKey((int)201, (wParam & CTRLMASK)==0, row, col);
			}

			// APPLE ALT KEY
			if ((diff_wParam & ALTMASK)!=0) // left and right caps key
			{
				// UP when mask is 0, DOWN if mask is 1
				mainWin->TranslateKey((int)202, (wParam & ALTMASK)==0, row, col);
			}
			
			last_wParam = wParam;
			break;
	}
	return 0;
}

