//
//  BeebEm-Debugger.cpp
//  BeebEm
//
//  Created by Commander Coder on 06/05/2024.
//

#include "apples.h"
#include "beebemrcids.h"
extern long DbgWindowCommandHandler(UINT32 cmdID);

extern "C" long beeb_DbgHandleCommand(unsigned int cmdID)
{
	char* cmdCHR = (char*)&cmdID;
	
	auto cmdRC = ID2RC.find(cmdID);
	if (cmdRC != ID2RC.end())
	{
		printf("DBHANDLECMD %c%c%c%c", cmdCHR[3], cmdCHR[2], cmdCHR[1], cmdCHR[0]);
		return DbgWindowCommandHandler(cmdRC->second);
	}

	printf(" NOT FOUND %c%c%c%c", cmdCHR[3], cmdCHR[2], cmdCHR[1], cmdCHR[0]);
	return 0;

}
