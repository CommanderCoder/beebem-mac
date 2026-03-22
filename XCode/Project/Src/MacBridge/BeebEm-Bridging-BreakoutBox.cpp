//
//  BeebEm-Bridging-BreakoutBox.cpp
//  BeebEm
//
//  Created by Commander Coder on 21/04/2024.
//

#include <Carbon/Carbon.h>
#include "UserPortBreakoutBox-mac.hpp"
#include "beebemrcids.h"


extern "C" void beeb_bbhandlekeys(long eventkind, unsigned int keycode, char charCode)
{

	switch (eventkind)
	{
			// TYPED A KEY - SO SET THE KEY
			//
		case kEventRawKeyDown:
			
			fprintf(stderr, "Key pressed: code = %d, '%c'\n", keycode, charCode);
			userPortBreakoutDialog->KeyDown(keycode);
			break;
		case kEventRawKeyUp:
			
			fprintf(stderr, "Key released: code = %d, '%c'\n", keycode, charCode);
			userPortBreakoutDialog->KeyUp(keycode);
			break;
	}

}


extern "C" void beeb_BreakoutBoxOpenDialog()
{
	if (userPortBreakoutDialog != NULL)
		userPortBreakoutDialog->WM_INITDIALOG();
}

extern "C" void beeb_BreakoutBoxCloseDialog()
{
    userPortBreakoutDialog->Close();
    delete userPortBreakoutDialog;
    userPortBreakoutDialog = nullptr;
}

extern "C" long beeb_BBHandleCommand(unsigned int cmdID)
{
	return userPortBreakoutDialog->WM_COMMAND(ConvID2RC(cmdID));
}

extern "C" void beeb_BBClearMapping()
{
    userPortBreakoutDialog->WM_CLEAR_KEY_MAPPING();
}

