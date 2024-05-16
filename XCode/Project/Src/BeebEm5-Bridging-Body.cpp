//
//  BeebEm5-Bridging-Body.cpp
//  BeebEm5
//
//  Created by Commander Coder on 04/01/2021.
//  Copyright © 2021 Andrew Hague. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Carbon/Carbon.h>

#include "main.h"
#include "beebwin.h"
#include "6502core.h"
#include "beebsound.h"
#include "serial.h"
#include "speech.h"
#include "tube.h"
#include "uef.h"
#include "csw.h"

#include "beebemrcids.h"
#include "userportbreakoutbox.h"

int done = 0;
extern OSStatus TCWindowCommandHandler(UInt32 cmdID);
extern FILE *tlog;

struct CColour{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

extern int __argc;
extern char** __argv;

extern OSStatus UKWindowCommandHandler(UInt32 cmdID);

extern BeebWin *mainWin;

extern void UserKeyboardDialog();
extern void SetBBCKeyForVKEY(int Key, bool shift);
extern void UKWindowKeyDown(UINT keycode);

extern RCItem beeb_RCTable[16][2];

std::vector<TapeMapEntry> beeb_TapeMap;
extern int beeb_TAPECYCLES();

#ifdef BEEBWIN
extern char AutoRunPath[];
extern void AutoRunFromPath();
#endif

extern "C" void beeb_autorun(char* path)
{
#ifdef BEEBWIN
    strcpy(AutoRunPath, path);
#endif
}

extern "C" void beeb_handlekeys(long eventkind, unsigned int keycode, char charCode)
{
    BeebMainKeyUpDown(eventkind, keycode, charCode);
}




int y = 0;
const int width = 640;
const int height = 512;


const int bpp = 4; // 4 bytes or 32 bits
unsigned char videobuffer[bpp * width * height]; // 200x200 by 4 bytes

extern "C" void beeb_video(int height, int width, struct CColour buffer[])
{
    for (int i = 0; i < width*height; i++)
    {
        buffer[i].b = videobuffer[0+(i*bpp)];
        buffer[i].g = videobuffer[1+(i*bpp)];
        buffer[i].r = videobuffer[2+(i*bpp)];
        buffer[i].a = videobuffer[3+(i*bpp)];
    }
    // move a green line everytime this is updated (50fps)
    y += 1;
    y %= height-2;
}

// MAIN EQUIVALENT IS
// beeb_main
// beeb_MainCpuLoop
// beeb_end
//


extern "C" int beeb_main(long argc, char* argv[])
{
    // NEED TO TURN OFF SANDBOXING IN ENTITLEMENTS FILE TO GET LOCAL FOLDERS TO WORK
    // DON'T TURN OFF SANDBOXING - THIS IS UNSAFE - ALLOW USER FOLDERS

    __argc = argc;
    __argv = argv;
    
    
// show the path
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }
    
    return (  mainInit() );
}



extern "C" void beeb_MainCpuLoop()
{
	mainStep();
}




extern "C" int beeb_end()
{
  fprintf(stderr, "Shutting Down ...\n");
  
    mainEnd();
  return(0);
} /* main */



extern "C" void beeb_BreakoutBoxOpenDialog()
{
    BreakoutBoxOpenDialog();
}

extern "C" void beeb_BreakoutBoxCloseDialog()
{
    BreakoutBoxCloseDialog();
}

extern "C" long beeb_BBHandleCommand(unsigned int cmdID)
{
    char* cmdCHR = (char*)&cmdID;
    printf("%c%c%c%c", cmdCHR[3], cmdCHR[2], cmdCHR[1], cmdCHR[0]);
#ifdef BEEBWIN
    return BBWindowCommandHandler(cmdID);
#else
    return 0;
#endif

}

extern "C" void beeb_TapeControlOpenDialog()
{
    TapeControlOpenDialog();
}


extern "C" void beeb_TapeControlCloseDialog()
{
    TapeControlCloseDialog();
}


extern "C" long beeb_TCHandleCommand(unsigned int cmdID)
{    
    char* cmdCHR = (char*)&cmdID;
    
    auto cmdRC = ID2RC.find(cmdID);
    if (cmdRC != ID2RC.end())
    {
        printf("TCHANDLECMD %c%c%c%c", cmdCHR[3], cmdCHR[2], cmdCHR[1], cmdCHR[0]);
        return TCWindowCommandHandler(cmdRC->second);
    }

    printf("NOT FOUND %c%c%c%c", cmdCHR[3], cmdCHR[2], cmdCHR[1], cmdCHR[0]);
    return 0;

}


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
    mainWin->ExportDiscFilesToFolder();
}
    
// user keyboard
extern "C" long beeb_UKHandleCommand(unsigned int cmdID)
{
    char* cmdCHR = (char*)&cmdID;
    
    auto cmdRC = ID2RC.find(cmdID);
    if (cmdRC != ID2RC.end())
    {
        printf("HANDLECMD %c%c%c%c", cmdCHR[3], cmdCHR[2], cmdCHR[1], cmdCHR[0]);
        return UKWindowCommandHandler(cmdRC->second);
    }

    printf("NOT FOUND %c%c%c%c", cmdCHR[3], cmdCHR[2], cmdCHR[1], cmdCHR[0]);
    return 0;
}

extern "C" void beeb_EditROMConfig()
{
    mainWin->HandleCommand(IDM_ROMCONFIG);
}

extern "C" void beeb_FinishROMConfig()
{
    mainWin->FinishROMConfig();
}
extern "C" const char* beeb_getRCEntry(int row, int column)
{
    return beeb_RCTable[row][column].name.c_str();
}

extern "C" long beeb_getTableRowsCount(const char* tablename)
{
//    printf("TD size %d\n", beeb_TapeMap.size());

    return beeb_TapeMap.size();
}


// cannot return value contained in a local variable - so this is global
static std::string  temp;
extern "C" const char* beeb_getTableCellData(UInt32 property, long itemID)
{
    temp = "";
//    char* propertyCHR = (char*)&property;

//    printf("%c%c%c%c data %ld", propertyCHR[3], propertyCHR[2], propertyCHR[1], propertyCHR[0], itemID);
    TapeMapEntry e = beeb_TapeMap [itemID];
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
                    if (CSWFileOpen)
                    {
                        csw_ptr = e.time;
                    }
                    else
                    {
                        TapeClock=e.time;
                    }

                    OldClock = 0;

                    SetTrigger(beeb_TAPECYCLES(), TapeTrigger);
                }
            break;
    }

    return temp.c_str();
}



extern "C" void beeb_HandleCommand(unsigned int cmdID)
{
    char* cmdCHR = (char*)&cmdID;
    
    auto cmdRC = ID2RC.find(cmdID);
    if (cmdRC != ID2RC.end())
    {
        printf("HANDLECMD %c%c%c%c", cmdCHR[3], cmdCHR[2], cmdCHR[1], cmdCHR[0]);
        mainWin->HandleCommand(cmdRC->second);
    }
    else
        printf("NOT FOUND %c%c%c%c", cmdCHR[3], cmdCHR[2], cmdCHR[1], cmdCHR[0]);

}


extern "C" void WriteLog(const char *fmt, ...)
{
}

extern "C" void beeb_UserKeyboardOpen()
{
    UserKeyboardDialog();
}

extern "C" void beeb_ukhandlekeys(long eventkind, unsigned int keycode, char charCode)
{

    switch (eventkind)
    {
            // TYPED A KEY - SO SET THE KEY
            //
        case kEventRawKeyDown:
            
            fprintf(stderr, "Key pressed: code = %d, '%c'\n", keycode, charCode);
                
            UKWindowKeyDown(keycode);
    }

}



extern "C" void beeb_bbhandlekeys(long eventkind, unsigned int keycode, char charCode)
{

    switch (eventkind)
    {
            // TYPED A KEY - SO SET THE KEY
            //
        case kEventRawKeyDown:
            
            fprintf(stderr, "Key pressed: code = %d, '%c'\n", keycode, charCode);
            userPortBreakoutDialog->KeyDown(charCode);
            break;
        case kEventRawKeyUp:
            
            fprintf(stderr, "Key released: code = %d, '%c'\n", keycode, charCode);
            userPortBreakoutDialog->KeyUp(charCode);
            break;
    }

}

