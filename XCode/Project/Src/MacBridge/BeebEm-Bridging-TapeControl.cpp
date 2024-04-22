//
//  BeebEm-Bridging-TapeControl.cpp
//  BeebEm
//
//  Created by Commander Coder on 21/04/2024.
//

#include "BeebEm-Bridging-TapeControl.hpp"

#include "apples.h"

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
