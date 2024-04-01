//
//  beebemrcids.h
//  BeebEm5
//
//  Created by Commander Coder on 30/09/2022.
//  Copyright © 2022 Andrew Hague. All rights reserved.
//

#ifndef beebemrcids_h
#define beebemrcids_h

#include "beebemrc.h"
#include <map>

#define ID IDM_FULLSCREEN

#define ID_FDC_ACORN                      39901
#define ID_FDC_OPUS                       39902
#define ID_FDC_WATFORD                    39903


extern std::map<int,int> ID2RC;
extern std::map<int,int> RC2ID;


#endif /* beebemrcids_h */
