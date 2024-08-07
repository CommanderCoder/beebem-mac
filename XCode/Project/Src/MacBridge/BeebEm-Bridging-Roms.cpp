//
//  BeebEm-Bridging-Roms.cpp
//  BeebEm
//
//  Created by Commander Coder on 21/04/2024.
//

#include "Main.h"
#include "beebemrcids.h"
#include "RomConfigDialog.h"

RomConfigDialog* runningRCDialog;

extern "C" bool beeb_RCHandleCommand(int wParam)
{
	if (runningRCDialog != NULL)
		return runningRCDialog->WM_COMMAND(wParam);
	return false;
}


extern "C" const char* beeb_getRCEntry(unsigned int row, unsigned int column)
{
	return GetRCEntry(row,column);
}

