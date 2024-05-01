//
//  BeebEm-Bridging-Roms.cpp
//  BeebEm
//
//  Created by Commander Coder on 21/04/2024.
//

#include "BeebEm-Bridging-Roms.hpp"
#include "Main.h"
#include "beebemrcids.h"
#include "RomConfigDialog.h"

RomConfigDialog* runningRCDialog;

extern "C" void beeb_EditROMConfig()
{
	// IDM_ROMCONFIG pops up the modal and responds to actions
	// however it uses a local RomConfigDialog which gets destroyed before this returns
	//	mainWin->HandleCommand(IDM_ROMCONFIG);
	
	// create the dialog and the SEGUE in the Storyboard will pop up the dialog
	runningRCDialog = new RomConfigDialog(NULL, NULL, RomConfig);
	
	runningRCDialog->WM_INITDIALOG(); // populate the modal
}


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

void RCEndDialog()
{
	mainWin->SetRomMenu();
	
	delete runningRCDialog;
	runningRCDialog = NULL;
}
