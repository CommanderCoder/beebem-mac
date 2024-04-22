//
//  BeebEm-Bridging-Roms.cpp
//  BeebEm
//
//  Created by Commander Coder on 21/04/2024.
//

#include "BeebEm-Bridging-Roms.hpp"



extern "C" void beeb_EditROMConfig()
{
	
}

extern "C" void beeb_FinishROMConfig()
{
	
}

extern "C" bool RCWindowCommandHandler(int wParam)
{
	return false;
}

extern "C" const char* beeb_getRCEntry(int row, int column)
{
	return 0;
}

