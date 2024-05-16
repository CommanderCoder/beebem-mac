//
//  BeebEm-Bridging-Keyboard.cpp
//  BeebEm
//
//  Created by Commander Coder on 12/04/2024.
//

#include <windows.h>



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


// SWIFT calls this to
extern "C" void beeb_handlekeys(long message, long wParam, long lParam)
{
//	message :eventkind
//  wParam :keycode
//	lParam :charCode

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
}

