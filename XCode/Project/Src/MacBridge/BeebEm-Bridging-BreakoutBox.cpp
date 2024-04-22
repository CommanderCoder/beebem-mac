//
//  BeebEm-Bridging-BreakoutBox.cpp
//  BeebEm
//
//  Created by Commander Coder on 21/04/2024.
//

#include "BeebEm-Bridging-BreakoutBox.hpp"


extern "C" void beeb_bbhandlekeys(long eventkind, unsigned int keycode, char charCode)
{
}



extern "C" void beeb_BreakoutBoxOpenDialog()
{
}

extern "C" void beeb_BreakoutBoxCloseDialog()
{
}

extern "C" long beeb_BBHandleCommand(unsigned int cmdID)
{
	return 0;
}


