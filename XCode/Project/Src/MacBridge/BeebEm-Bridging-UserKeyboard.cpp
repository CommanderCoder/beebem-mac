//
//  BeebEm-Bridging-UserKeyboard.cpp
//  BeebEm
//
//  Created by Commander Coder on 21/04/2024.
//

#include "BeebEm-Bridging-UserKeyboard.hpp"



extern "C" void beeb_UserKeyboardOpen()
{
	
}

extern "C" long beeb_UKHandleCommand(unsigned int cmdID)
{
	return 0;
}

extern "C" void beeb_ukhandlekeys(long eventkind, unsigned int keycode, char charCode)
{
	
}
