//
//  BeebEm-Bridging-Keyboard.cpp
//  BeebEm
//
//  Created by Commander Coder on 12/04/2024.
//

#include <windows.h>


enum {
	aEventJoystick1Axis              = 0x0000,
	aEventJoystick1Button            = 0x0001,
	aEventJoystick2Axis              = 0x1000,
	aEventJoystick2Button            = 0x1001,
};


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


// map from APPLE input: https://eastmanreference.com/complete-list-of-applescript-key-codes
// to WINDOWS input: https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

// which is then translated to beeb input:
/* from b-Em
 
 *       0x00      0x01  0x02  0x03 0x04 0x05 0x06 0x07 0x08 0x09    0x0a   0x0b   0x0c
 * 0x00  Shift     Ctrl  <------- starup up DIP swicthes ------->
 * 0x10  Q         3     4     5    f4   8    f7   =-   ~^   Left    KP 6   KP 7
 * 0x20  f0        W     E     T    7    I    9    0    £    Down    KP 8   KP 9
 * 0x30  1         2     D     R    6    U    O    P    [{   Up      KP +   KP -   KP Return
 * 0x40  CapsLck   A     X     F    Y    J    K    @    :*   Return  KP /   KP Del KP .
 * 0x50  ShiftLck  S     C     G    H    N    L    ;+   ]}   Delete  KP #   KP *   KP ,
 * 0x60  Tab       Z     SPC   V    B    M    <,   >.   /?   Copy    KP 0   KP 1   KP 3
 * 0x70  ESC       f1    f2    f3   f5   f6   f8   f9   \    Right   KP 4   KP 5   KP 2
 */

int kmap[] =
{
VK_KEY_A, //[0]
VK_KEY_S,
VK_KEY_D,
VK_KEY_F,
VK_KEY_H,
VK_KEY_G,
VK_KEY_Z,
VK_KEY_X,
VK_KEY_C,
VK_KEY_V,
VK_OEM_MINUS, //VK_KEY_§ (next to 1)
VK_KEY_B, //[11]
VK_KEY_Q,
VK_KEY_W,
VK_KEY_E,
VK_KEY_R,
VK_KEY_Y,
VK_KEY_T,
VK_KEY_1,
VK_KEY_2,
VK_KEY_3,
VK_KEY_4,
VK_KEY_6,
VK_KEY_5,
VK_OEM_PLUS,
VK_KEY_9, //[25]
VK_KEY_7,
VK_OEM_MINUS,
VK_KEY_8,
VK_KEY_0,
VK_OEM_6, //]
VK_KEY_O,
VK_KEY_U,
VK_OEM_4, //[
VK_KEY_I,
VK_KEY_P,
VK_RETURN,
VK_KEY_L,
VK_KEY_J, //[38]
VK_OEM_3, //'
VK_KEY_K,
VK_OEM_1, //;
VK_OEM_5, //\    .
VK_OEM_COMMA,
VK_OEM_2, // /
VK_KEY_N,
VK_KEY_M,
VK_OEM_PERIOD, //.
VK_TAB,
VK_SPACE,
VK_OEM_8, // ~` [50]
VK_BACK,
0,
VK_ESCAPE,
0,
0, // [55] COMMAND
0, // [56] /// left shift
0, // [57] caps lock
0, // [58] left option
0, // [59] left control
0, // [60] // right shift
0, // [61] right option
0, // [62] right control
0, // [63] fn key
0, // [64]
0,
0,
0,
0, // [68]
0,
0,
0,
0, // [72]
0,
0,
0,
0, // [76]
0,
0,
0,
0, // [80]
0,
0,
0,
0, // [84]
0,
0,
0,
0, // [88]
0,
0,
0,
0, // [92]
0,
0,
0,
VK_F5, // F5 [96]
VK_F6, // F6
VK_F3, // F3
VK_F7, // F7
VK_F8, // F8
VK_F9, // F9
0,
VK_END, // f11
0,
VK_F13, //[BREAK - F13] 105
0,
0,
0,
VK_F10, // F10
0,
VK_F12, //[BREAK - F12]
0, // [112]
0,
0,
0,
0, // pageup [116]
0,
VK_F4, // F4
VK_END, // <end>
VK_F2, // F2 [120]
0, //pagedown
VK_F1, // F1
VK_LEFT, // <left>
VK_RIGHT,  // <right> [124]
VK_DOWN,  // <down>
VK_UP,  // <up>
0,
VK_SHIFT, // <SHIFT> //[128]
0,
0,
0,
0, // [132]
0,
0,
0,
0, // [136]
0,
0,
0,
0, // [140]
0,
0,
0,
0, // [144]
0,
0,
0,
0, // [148]
0,
0,
0,
0, // [152]
0,
0,
0,
0, // [156]
0,
0,
0,
0, // [160]
0,
0,
0,
0, // [164]
0,
0,
0,
0, // [168]
0,
0,
0,
0, // [172]
0,
0,
0,
0, // [176]
0,
0,
0,
0, // [180]
0,
0,
0,
0, // [184]
0,
0,
0,
0, // [188]
0,
0,
0,
0, // [192]
0,
0,
0,
0, // [196]
0,
0,
0,
0, // [200]
VK_CONTROL, // 201 is CTRL
VK_CAPITAL, // 202 is ALT
0,
0, // [204]
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	0,
//	6 10 0 6 10 1 // NUMPAD0 [355]
//	6 11 0 6 11 1
//	7 12 0 7 12 1
//	6 12 0 6 12 1
//	7 10 0 7 10 1
//	7 11 0 7 11 1
//	1 10 0 1 10 1 // NUMPAD6
//	1 11 0 1 11 1
//	2 10 0 2 10 1
//	2 11 0 2 11 1
//	5 11 0 5 11 1
//	3 10 0 3 10 1
//	3 11 0 3 11 1 // VK_SUBTRACT
//	4 12 0 4 12 1
//	4 10 0 4 10 1
//	-3 -3 0 -3 -3 1
//	-3 -4 0 -3 -4 1 // <SHORTCUT>


};

int remapKeys(int k)
{
	if (k > sizeof(kmap))
		return k;
	
	return kmap[k];
}


/*
*/


// SWIFT calls this to
extern "C" void beeb_handlekeys(long message, long wParam, long lParam)
{
//	message :eventkind
//  wParam :keycode
//	lParam :charCode

	int row, col;
	int key;

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
			
			key = remapKeys((int)wParam);
			mainWin->TranslateKey(key, false, row, col);
			break;
		case kEventRawKeyUp:
//          fprintf(stderr, "Key released: code = %d, '%c'\n", wParam, lParam);
				key = remapKeys((int)wParam);

				if (mainWin->TranslateKey(key, true, row, col) < 0)
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
				mainWin->TranslateKey(VK_SHIFT, (wParam & SHIFTMASK)==0, row, col);
			}
			// APPLE CTRL KEY
			if ((diff_wParam & CTRLMASK)!=0) // left and right ctrl key
			{
				// UP when mask is 0, DOWN if mask is 1
				mainWin->TranslateKey(VK_CONTROL, (wParam & CTRLMASK)==0, row, col);
			}

			// APPLE ALT KEY
			if ((diff_wParam & ALTMASK)!=0) // left and right caps key
			{
				// UP when mask is 0, DOWN if mask is 1
				mainWin->TranslateKey(VK_CAPITAL, (wParam & ALTMASK)==0, row, col);
			}
			
			last_wParam = wParam;
			break;
	}
}

extern "C" void beeb_handlemouse(long message, long wParam, long lParam)
{
	UINT msg = WM_LBUTTONUP;
	switch (message)
	{
		case kEventMouseUp:
//			fprintf(stderr, "mouseup : code = %ld\n", wParam);
			switch (wParam)
			{
				case MK_RBUTTON: msg = WM_RBUTTONUP; break;
				case MK_MBUTTON: msg = WM_MBUTTONUP; break;
				default: msg = WM_LBUTTONUP; break;
			}
			mainWin->AppProc(msg, wParam, lParam);
			break;
		case kEventMouseDown:
//			fprintf(stderr, "mousedown : code = %ld\n", wParam);
			switch (wParam)
			{
				case MK_RBUTTON: msg = WM_RBUTTONDOWN; break;
				case MK_MBUTTON: msg = WM_MBUTTONDOWN; break;
				default: msg = WM_LBUTTONDOWN; break;
			}
			mainWin->AppProc(msg, wParam, lParam);
			break;
		case kEventMouseMoved:
		case kEventMouseDragged:
			int x = mainWin->m_XWinSize*GET_X_LPARAM(lParam)/32768.0f;
			int y = mainWin->m_YWinSize*GET_Y_LPARAM(lParam)/32768.0f;
			
//			fprintf(stderr, "mousemove : %ld, x = %d, y = %d\n", message, x, y);

			bool useMovementDelta = true;
			if (useMovementDelta)
				mainWin->AppProc(WM_INPUT, wParam, MAKELPARAM(x,y));
			else
				mainWin->AppProc(WM_MOUSEMOVE, wParam, MAKELPARAM(x,y));
			break;
	}
}

static long joy1[4]; // 4 axes
static long buttons1; // button bitflags

extern "C" void beeb_handlejoystick(long message, long wParam, long lParam)
{
	// axis or buttons message (joystick1, joystick2)
	// axes:
	//  wParam is the axis number (axis on a PS3 controller)
	//  lParam is axis values
	// buttons:
	//  wParam is the button number
	//  lParam is value (0 up, 1 down)
	
	switch (message)
	{
		case aEventJoystick1Axis:
			// wParam is 0,1,2 or 3 for x and y axes (on joystick)
			if (wParam >= 0 && wParam < 4)
			{
				joy1[wParam] = 32768.0+lParam;
			}

			//	MM_JOY1MOVE
			//	lParam x,y from wXmax to wXmin..
			mainWin->AppProc(MM_JOY1MOVE, 0, MAKELPARAM(joy1[0],joy1[1]));
			break;
		case aEventJoystick1Button:
			//	 JOY_BUTTON1 , JOY_BUTTON2
			if (wParam == 0) break;
			
			int bitpos = 0;
			if (wParam == 1) bitpos = JOY_BUTTON1;
			if (wParam == 2) bitpos = JOY_BUTTON2;
			if (wParam == 3) bitpos = JOY_BUTTON3;
			if (wParam == 4) bitpos = JOY_BUTTON4;

			long oldpattern = buttons1;

			if (lParam==0)
			{
				buttons1 &= ~bitpos;
				if (oldpattern != buttons1)
					mainWin->AppProc(MM_JOY1BUTTONUP, buttons1, 0);
			}
			else
			{
				buttons1 |= bitpos;
				if (oldpattern != buttons1)
					mainWin->AppProc(MM_JOY1BUTTONDOWN, buttons1, 0);
			}
			break;
			break;
	}
	
}

