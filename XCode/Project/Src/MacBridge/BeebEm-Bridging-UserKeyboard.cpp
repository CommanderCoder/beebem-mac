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
	char* cmdCHR = (char*)&cmdID;
	
	auto cmdRC = ID2RC.find(cmdID);
	if (cmdRC != ID2RC.end())
	{
		printf("HANDLECMD %c%c%c%c", cmdCHR[3], cmdCHR[2], cmdCHR[1], cmdCHR[0]);
		return UK_WM_COMMAND((WPARAM)cmdRC->second);//
	}

	printf("NOT FOUND %c%c%c%c", cmdCHR[3], cmdCHR[2], cmdCHR[1], cmdCHR[0]);
	return 0;
}

extern "C" void beeb_ukhandlekeys(long eventkind, unsigned int keycode, char charCode)
{
	
	switch (eventkind)
 {
		 // TYPED A KEY - SO SET THE KEY
		 //
	 case kEventRawKeyDown:
		 
		 fprintf(stderr, "Key pressed: code = %d, '%c'\n", keycode, charCode);
			 
		 selectKeyDialog_Key = keycode;
		 selectKeyDialog_Shift = false;
		 UK_WM_SELECT_KEY_DIALOG_CLOSED((WPARAM)IDOK);
		 break;
 }

}
