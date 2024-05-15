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
	char* cmdCHR = (char*)&cmdID;
	
	auto cmdRC = ID2RC.find(cmdID);
	if (cmdRC != ID2RC.end())
	{
		printf("KLHANDLECMD %c%c%c%c", cmdCHR[3], cmdCHR[2], cmdCHR[1], cmdCHR[0]);
		return runningKLDialog->WM_COMMAND(cmdRC->second);
	}

	printf(" NOT FOUND %c%c%c%c", cmdCHR[3], cmdCHR[2], cmdCHR[1], cmdCHR[0]);
	return 0;

}
