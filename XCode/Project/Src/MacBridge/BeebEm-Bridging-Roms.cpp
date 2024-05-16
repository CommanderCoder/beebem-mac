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

extern "C" bool RCWindowCommandHandler(int wParam)
{
	if (runningRCDialog != NULL)
		return runningRCDialog->WM_COMMAND(wParam);
	return false;
}


extern "C" const char* beeb_getRCEntry(int row, int column)
{
	return beeb_RCTable[row][column].name.c_str();;
}

