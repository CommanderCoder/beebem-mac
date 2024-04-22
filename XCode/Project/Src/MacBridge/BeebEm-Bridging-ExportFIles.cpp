//
//  BeebEm-Bridging-ExportFIles.cpp
//  BeebEm
//
//  Created by Commander Coder on 21/04/2024.
//

#include "BeebEm-Bridging-ExportFIles.hpp"
#include "beebemrcids.h"
#include "Main.h"

// Dummy functions for building the BeebEm shim



extern "C" long beeb_ExportDiscFiles( unsigned int driveID)
{
	auto driveRC = ID2RC.find(driveID);
	if (driveRC != ID2RC.end())
	{
		mainWin->ExportDiscFiles(driveRC->second);
		return 1;
	}
   
	return 0;
}
	
extern "C" void beeb_ExportDiscFilesToFolder( )
{
//	mainWin->ExportDiscFilesToFolder();
}
