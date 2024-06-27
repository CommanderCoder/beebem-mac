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
	return DbgWindowCommandHandler(ConvID2RC(cmdID));
}
