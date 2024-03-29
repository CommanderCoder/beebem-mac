//
//  BeebEm-Bridging-Temp.cpp
//  BeebEm
//
//  Created by Commander Coder on 29/03/2024.
//

#include "BeebEm-Bridging-Temp.hpp"




// Dummy functions for building the BeebEm shim

extern "C" void beeb_handlekeys(long eventkind, unsigned int keycode, char charCode)
{
}

extern "C" int beeb_main(long argc, char* argv[])
{
	return 1;
}


extern "C" void beeb_MainCpuLoop()
{
}

extern "C" int beeb_end()
{
	return 0;
}

extern "C" void beeb_HandleCommand(unsigned int cmdID)
{
}

extern "C" long beeb_ExportDiscFiles( unsigned int driveID)
{
	return 0;
}


extern "C" void beeb_video(int height, int width, struct CColour buffer[])
{
}
