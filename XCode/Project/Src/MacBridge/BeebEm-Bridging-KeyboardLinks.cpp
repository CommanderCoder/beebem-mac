//
//  BeebEm-Bridging-KeyboardLinks.cpp
//  BeebEm
//
//  Created by Commander Coder on 13/05/2024.
//

#include "beebemrcids.h"
#include "KeyboardLinksDialog.h"

KeyboardLinksDialog* runningKLDialog = NULL;


extern "C" long beeb_KLHandleCommand(unsigned int cmdID)
{
	return runningKLDialog->WM_COMMAND(ConvID2RC(cmdID));
}
