//
//  BeebEm-Bridging-Temp.cpp
//  BeebEm
//
//  Created by Commander Coder on 29/03/2024.
//

#include "BeebEm-Bridging-Temp.hpp"
#include <Carbon/Carbon.h>
#include <string>

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

extern "C" void beeb_video(int height, int width, struct CColour buffer[])
{
}

//tape
extern "C" void beeb_TapeControlOpenDialog()
{
	
}


extern "C" void beeb_TapeControlCloseDialog()
{
	
}

extern "C" long beeb_TCHandleCommand(unsigned int cmdID)
{
	return 0;
}

extern "C" long beeb_ExportDiscFiles( unsigned int driveID)
{
	return 0;
}

extern "C" void beeb_ExportDiscFilesToFolder( )
{
}


extern "C" long beeb_getTableRowsCount(const char* tablename)
{
	return 0;
}


// cannot return value contained in a local variable - so this is global
static std::string  temp;
extern "C" const char* beeb_getTableCellData(UInt32 property, long itemID)
{
	temp="";
	return temp.c_str();
}


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


//main.cpp
extern "C" int mainInit();
extern "C" int mainStep();
extern "C" int mainEnd();


