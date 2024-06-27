//
//  BeebEm-Bridging-TapeControl.cpp
//  BeebEm
//
//  Created by Commander Coder on 21/04/2024.
//

#include "apples.h"
#include "beebemrcids.h"
#include "TapeControlDialog-mac.hpp"

#include <vector>

#include "TapeMap.h"
std::vector<TapeMapEntry> beeb_TapeMap;

#include "Serial.h"


extern int csw_bit;
extern int csw_pulselen;
extern int csw_ptr;
extern int csw_pulsecount;
extern bool CSWFileOpen;
extern int CSWPollCycles;


extern "C" long beeb_getTableRowsCount(const char* tablename)
{
//    printf("TD size %d\n", beeb_TapeMap.size());

	return beeb_TapeMap.size();
}


// cannot return value contained in a local variable - so this is global
static std::string  temp;
extern "C" const char* beeb_getTableCellData(UINT32 property, long itemID)
{
	temp = "";
	TapeMapEntry e = beeb_TapeMap [itemID];

	//    char* propertyCHR = (char*)&property;

//    printf("%c%c%c%c data %ld", propertyCHR[3], propertyCHR[2], propertyCHR[1], propertyCHR[0], itemID);
//    printf("TD %s\n", e.desc.c_str());
	
	if (e.desc.length()==0)
		return "---";
	switch(property)
	{
		case 'NAME' :
			temp = e.desc.substr(0,12);
			break;

		case 'BLCK' :
			temp = e.desc.substr(13,2);
			break;

		case 'LENG' :
			temp = e.desc.substr(16,std::string::npos);
			break;
			
		case 3 :
				if (itemID >= 0 && itemID < beeb_TapeMap.size())
				{
					SetTapePosition(e.time);
				}
			break;
	}

	return temp.c_str();
}

extern "C" long beeb_TCHandleCommand(unsigned int cmdID)
{
	return TC_WM_COMMAND(ConvID2RC(cmdID));
}

