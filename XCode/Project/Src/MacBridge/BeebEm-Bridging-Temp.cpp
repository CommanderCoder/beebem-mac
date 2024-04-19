//
//  BeebEm-Bridging-Temp.cpp
//  BeebEm
//
//  Created by Commander Coder on 29/03/2024.
//

#include "BeebEm-Bridging-Temp.hpp"
#include <string>

// Dummy functions for building the BeebEm shim



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
extern "C" const char* beeb_getTableCellData(unsigned int property, long itemID)
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



