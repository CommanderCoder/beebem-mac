//
//  BeebEm-Bridging-UserKeyboard.cpp
//  BeebEm
//
//  Created by Commander Coder on 21/04/2024.
//

#include "UserKeyboardDialog-mac.hpp"
#include "beebemrcids.h"
#include "Main.h"
#include <Carbon/Carbon.h>


extern "C" void beeb_UserKeyboardClosed()
{
//	WM_USER_KEYBOARD_DIALOG_CLOSED
	mainWin->UserKeyboardDialogClosed();
}

// user keyboard
extern "C" long beeb_UKHandleCommand(unsigned int cmdID)
{
	return UK_WM_COMMAND((WPARAM)ConvID2RC(cmdID));
}

extern "C" void beeb_ukhandlekeys(long eventkind, unsigned int keycode, char charCode)
{
	
	switch (eventkind)
 {
		 // TYPED A KEY - SO SET THE KEY
		 //
	 case kEventRawKeyDown:
		 
		 fprintf(stderr, "Key pressed: code = %d, '%c'\n", keycode, charCode);
			 
         // this is the MAC keycode
         int PCkey = remapKeys((int)keycode);
         
		 selectKeyDialog_Key = PCkey;
		 selectKeyDialog_Shift = false;
		 UK_WM_SELECT_KEY_DIALOG_CLOSED((WPARAM)IDOK);
 		 break;
 }

}

extern "C" void beeb_UKReset()
{
	UK_WM_CLEAR_KEY_MAPPING(0);
}
