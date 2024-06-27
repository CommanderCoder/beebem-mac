//
//  BeebEm-Bridging-SerialPort.cpp
//  BeebEm
//
//  Created by Commander Coder on 27/06/2024.
//

#include "SerialPortDialog-mac.hpp"
#include "beebemrcids.h"



extern "C" bool beeb_SPHandleCommand(unsigned int wParam)
{
	
	if (serialPortDialog != NULL)
		return serialPortDialog->WM_COMMAND(ConvID2RC(wParam));
	return false;
}

