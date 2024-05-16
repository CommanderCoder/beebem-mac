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
			userPortBreakoutDialog->KeyDown(charCode);
			break;
		case kEventRawKeyUp:
			
			fprintf(stderr, "Key released: code = %d, '%c'\n", keycode, charCode);
			userPortBreakoutDialog->KeyUp(charCode);
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
//	BreakoutBoxCloseDialog();
}

extern "C" long beeb_BBHandleCommand(unsigned int cmdID)
{
	char* cmdCHR = (char*)&cmdID;
	
	auto cmdRC = ID2RC.find(cmdID);
	if (cmdRC != ID2RC.end())
	{
		printf("TCHANDLECMD %c%c%c%c", cmdCHR[3], cmdCHR[2], cmdCHR[1], cmdCHR[0]);
		return userPortBreakoutDialog->WM_COMMAND(cmdRC->second);
	}

	printf(" NOT FOUND %c%c%c%c", cmdCHR[3], cmdCHR[2], cmdCHR[1], cmdCHR[0]);
	return 0;


	
	
}

