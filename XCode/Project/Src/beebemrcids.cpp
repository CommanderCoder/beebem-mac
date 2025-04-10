//
//  beebemrcids.cpp
//  BeebEm5
//
//  Created by Commander Coder on 05/10/2022.
//  Copyright © 2022 Andrew Hague. All rights reserved.
//

#include <stdio.h>

#include "beebemrcids.h"

std::map<int,int> ID2RC =
{
{'sfps', IDM_SPEEDANDFPS},
{'rund', IDM_RUNDISC},
{'opn0', IDM_LOADDISC0},
{'opn1', IDM_LOADDISC1},
{'new0', IDM_NEWDISC0},
{'new1', IDM_NEWDISC1},
{'rest', IDM_FILE_RESET},
{'bbcb', IDM_MODELB},
{'bbci', IDM_MODELBINT},
{'bbcp', IDM_MODELBPLUS},
{'bbcm', IDM_MASTER128},
{'bbce', IDM_MASTER_ET},
{'tnil', IDM_TUBE_NONE},
{'tube', IDM_TUBE_ACORN65C02},
//    {'t186', whatever happend to tube 186? },
{'t512', IDM_TUBE_MASTER512  }, //new
{'az80', IDM_TUBE_ACORNZ80  },
{'tz80', IDM_TUBE_TORCHZ80  },
{'tarm', IDM_TUBE_ACORNARM  },
{'tspr', IDM_TUBE_SPROWARM  }, //new
{'sRT ', IDM_REALTIME },
{'s50 ', IDM_50FPS},
{'s25 ', IDM_25FPS},
{'s10 ', IDM_10FPS},
{'s5  ', IDM_5FPS },
{'s1  ', IDM_1FPS },
{'f100', IDM_FIXEDSPEED100 },
{'f5  ', IDM_FIXEDSPEED5   },
{'f2  ', IDM_FIXEDSPEED2   },
{'f1.5', IDM_FIXEDSPEED1_5 },
{'f1.2', IDM_FIXEDSPEED1_25},
{'f1.1', IDM_FIXEDSPEED1_1 },
{'f0.9', IDM_FIXEDSPEED0_9 },
{'f0.7', IDM_FIXEDSPEED0_75},
{'f50 ', IDM_FIXEDSPEED50  },
{'f0.2', IDM_FIXEDSPEED0_25},
{'f10 ', IDM_FIXEDSPEED10  },
{'f0.5', IDM_FIXEDSPEED0_5 },
{'f0.1', IDM_FIXEDSPEED0_1 },
{'monr', IDM_MONITOR_RGB  },
{'monb', IDM_MONITOR_BW   },
{'mona', IDM_MONITOR_AMBER},
{'mong', IDM_MONITOR_GREEN},
{'wrp0', IDM_WRITE_PROTECT_DISC0},
{'wrp1', IDM_WRITE_PROTECT_DISC1},
{'wpol', IDM_WRITE_PROTECT_ON_LOAD},
{'siz1', ID_}, //160x128 selected
{'siz2', ID_},
{'siz3', ID_},
{'siz4', ID_},
{'siz5', ID_},
{'siz6', ID_},
{'siz7', ID_},
{'siz8', ID_},
{'siz9', ID_},
{'siza', ID_},
//{'sizb', IDM_VIEW_DD_1600X1200},//1600x1200 selected
{'vfsc', ID_},//View full screen selected
{'vmar', IDM_MAINTAINASPECTRATIO},//Maintain Aspect Ratio selected
{'volh', IDM_HIGHVOLUME  },
{'volm', IDM_MEDIUMVOLUME},
{'voll', IDM_LOWVOLUME   },
{'volf', IDM_FULLVOLUME},
{'sndc', IDM_SOUNDCHIP},
{'sond', IDM_SOUNDONOFF},
{'sped', IDM_SPEECH}, //--gone
{'enet', IDM_ECONET},
{'igil', ID_},//Ignore Illegal Instructions - see OPCODES
{'hard', IDM_BASIC_HARDWARE_ONLY},//Basic Hardware
{'docu', ID_},//Documented Only
{'extr', ID_},//Common Extras
{'full', ID_},//Full Set
{'ifd0', IDM_DISC_IMPORT_0},
{'ifd1', IDM_DISC_IMPORT_1},
{'ifd2', IDM_DISC_IMPORT_2},
{'ifd3', IDM_DISC_IMPORT_3},
{'efd0', IDM_DISC_EXPORT_0},
{'efd1', IDM_DISC_EXPORT_1},
{'efd2', IDM_DISC_EXPORT_2},
{'efd3', IDM_DISC_EXPORT_3},
{'roma', IDM_ALLOWWRITES_ROM0},
{'romb', IDM_ALLOWWRITES_ROM1},
{'romc', IDM_ALLOWWRITES_ROM2},
{'romd', IDM_ALLOWWRITES_ROM3},
{'rome', IDM_ALLOWWRITES_ROM4},
{'romf', IDM_ALLOWWRITES_ROM5},
{'romg', IDM_ALLOWWRITES_ROM6},
{'romh', IDM_ALLOWWRITES_ROM7},
{'romi', IDM_ALLOWWRITES_ROM8},
{'romj', IDM_ALLOWWRITES_ROM9},
{'romk', IDM_ALLOWWRITES_ROMA},
{'roml', IDM_ALLOWWRITES_ROMB},
{'romm', IDM_ALLOWWRITES_ROMC},
{'romn', IDM_ALLOWWRITES_ROMD},
{'romo', IDM_ALLOWWRITES_ROME},
{'romp', IDM_ALLOWWRITES_ROMF},
{'ledr', IDM_RED_LEDS     },
{'ledg', IDM_GREEN_LEDS   },
{'ledk', IDM_SHOW_KBLEDS  },
{'ledd', IDM_SHOW_DISCLEDS},
{'savp', IDM_SAVE_PREFS},
{'qukl', IDM_QUICKLOAD},
{'quks', IDM_QUICKSAVE},
{'rsts', IDM_LOADSTATE},
{'savs', IDM_SAVESTATE},
{'prnt', IDM_PRINTERONOFF},
{'pfle', IDM_PRINTER_FILE},
{'plp1', IDM_PRINTER_LPT1},
{'plp2', IDM_PRINTER_LPT2}, //new vv
{'plp3', IDM_PRINTER_LPT3},
{'plp4', IDM_PRINTER_LPT4},
{'pclp', IDM_PRINTER_CLIPBOARD},
//{'pcc1', IDM_PRINTER_COM1},
//{'pcc2', IDM_PRINTER_COM2},
//{'pcc3', IDM_PRINTER_COM3},
//{'pcc4', IDM_PRINTER_COM4}, //new ^^
{'kmas', IDM_MAPAS},
{'copy', IDM_EDIT_COPY},
{'past', IDM_EDIT_PASTE},
{'trac', ID_}, // trace186
{'amxo', IDM_AMXONOFF       },
{'amx3', IDM_AMX_320X256    },
{'amx6', IDM_AMX_640X256    },
{'amx1', IDM_AMX_160X256    },
{'axp5', IDM_AMX_ADJUSTP50  },
{'axp3', IDM_AMX_ADJUSTP30  },
{'axp1', IDM_AMX_ADJUSTP10  },
{'axm1', IDM_AMX_ADJUSTM10  },
{'axm3', IDM_AMX_ADJUSTM30  },
{'axm5', IDM_AMX_ADJUSTM50  },
{'amxl', IDM_AMX_LRFORMIDDLE},
{'ofwm', IDM_FREEZEINACTIVE},
{'msea', IDM_ANALOGUE_MOUSESTICK},
{'msed', IDM_DIGITAL_MOUSESTICK},
{'tplo', IDM_LOADTAPE},
{'tpfa', IDM_TAPE_FAST  },
{'tpmf', IDM_TAPE_MFAST },
{'tpms', IDM_TAPE_MSLOW },
{'tpno', IDM_TAPE_NORMAL},
{'tpso', IDM_TAPESOUND},
{'tpcr', IDM_SFX_RELAY},
{'ddso', IDM_SFX_DISCDRIVES},
{'sudf', IDM_SELECT_USER_DATA_FOLDER},
{'tpre', IDM_REWINDTAPE},
{'tpul', IDM_UNLOCKTAPE},
{'tpco', IDM_TAPECONTROL},
{'dbgr', IDM_SHOWDEBUGGER},
{'upbo', IDM_BREAKOUT},
{'uprm', IDM_USER_PORT_RTC_MODULE},
{'abou', IDM_ABOUT},
{'kusr', IDM_DEFINEKEYMAP},
{'lukm', IDM_LOADKEYMAP},
{'sukm', IDM_SAVEKEYMAP},
{'pacm', IDM_AUTOSAVE_PREFS_CMOS},
{'pfol', IDM_AUTOSAVE_PREFS_FOLDERS},
{'pall', IDM_AUTOSAVE_PREFS_ALL},
{'udkm', IDM_USERKYBDMAPPING},
{'dkm ', IDM_DEFAULTKYBDMAPPING},
{'lkm ', IDM_LOGICALKYBDMAPPING},
{'vidc', IDM_CAPTUREVIDEO},
{'vide', IDM_ENDVIDEO},
{'skp0', IDM_VIDEOSKIP0},
{'skp1', IDM_VIDEOSKIP1},
{'skp2', IDM_VIDEOSKIP2},
{'skp3', IDM_VIDEOSKIP3},
{'skp4', IDM_VIDEOSKIP4},
{'skp5', IDM_VIDEOSKIP5},
{'rec1', IDM_VIDEORES1},
{'rec2', IDM_VIDEORES2},
{'rec3', IDM_VIDEORES3},
{'rec4', ID_}, //--unused
{'mbof', IDM_BLUR_OFF},
{'mb2f', IDM_BLUR_2},
{'mb4f', IDM_BLUR_4},
{'mb8f', IDM_BLUR_8},
{'ejd0', IDM_EJECTDISC0},
{'ejd1', IDM_EJECTDISC1},
{'snev', ID_},
{'txlf', IDM_TRANSLATE_CRLF},
{'txte', IDM_TELETEXT},
//{'txtd', ID_TELETEXTFILES},
//{'txts', ID_TELETEXTLOCALHOST},
//{'txtc', ID_TELETEXTCUSTOM},
{'txts', IDM_SELECT_TELETEXT_DATA_SOURCE},
{'flpe', IDM_FLOPPY_DRIVE},
{'hdsc', IDM_SCSI_HARD_DRIVE},
{'hdde', IDM_IDE_HARD_DRIVE},
{'hdre', IDM_SELECT_HARD_DRIVE_FOLDER},
//{'rs42', ID_},
//{'sdts', ID_},
//{'sdep', ID_},
//{'sdsp', ID_},
{'page', ID_}, // page setup
{'prns', ID_}, // print window
{'cpyc', ID_}, // copy window to clipboard
{'invb', ID_}, // invertbackground
{'swtd', IDM_CAPTURESCREEN},
{'txvw', IDM_TEXTVIEW},
{'ttsp', IDM_TEXTTOSPEECH_ENABLE},
{'mus5', IDM_MUSIC5000},
    
{'mbcn', IDM_8271},
{'mbca', IDM_FDC_ACORN}, //Acorn 1770 Controller
{'mbco', IDM_FDC_OPUS}, //OPUS 1770 Controller - on DLL on windows
{'mbcw', IDM_FDC_WATFORD}, //Watford 1770 - on DLL on windows
//    {'mrty', ID_RTCY2KADJUST},
	{'nojk', IDM_JOYSTICK_DISABLED},
	{'jstk', IDM_JOYSTICK},
    {'hcur', IDM_HIDECURSOR},
    {'kf10', IDM_MAPFUNCS},
    {'kdis', IDM_DISABLEKEYSALL},
    {'kwin', IDM_DISABLEKEYSWINDOWS},
    {'ksht', IDM_DISABLEKEYSSHORTCUT},
    {'kbrk', IDM_DISABLEKEYSBREAK},
    {'kesc', IDM_DISABLEKEYSESCAPE},
    {'sram', IDM_SWRAMBOARD},
    {'erom', IDM_ROMCONFIG},

	{'sril', IDM_SERIAL},
	{'srld', IDM_SELECT_SERIAL_DESTINATION},
	
	
    // userportbreakout
    {'upb0',IDK_BIT0},
    {'upb1',IDK_BIT1},
    {'upb2',IDK_BIT2},
    {'upb3',IDK_BIT3},
    {'upb4',IDK_BIT4},
    {'upb5',IDK_BIT5},
    {'upb6',IDK_BIT6},
    {'upb7',IDK_BIT7},
    {'upc7',IDC_IB7},
    {'upc6',IDC_IB6},
    {'upc5',IDC_IB5},
    {'upc4',IDC_IB4},
    {'upc3',IDC_IB3},
    {'upc2',IDC_IB2},
    {'upc1',IDC_IB1},
    {'upc0',IDC_IB0},
	
	{'spsp',IDC_SERIAL_PORT_DESTINATION_SERIAL_PORT},
	{'spts',IDC_SERIAL_PORT_DESTINATION_TOUCH_SCREEN},
	{'spip',IDC_SERIAL_PORT_DESTINATION_IP},

	{'sprt',IDC_SERIAL_PORT},
	{'ipad',IDC_IP_ADDRESS},
	{'ippt',IDC_IP_PORT},
	
	{'iprc',IDC_IP323_RAW_COMMS},
	{'iphs',IDC_IP232_HANDSHAKE},
    

    //keyb
    {'A   ',IDK_A},
    {'B   ',IDK_B},
    {'C   ',IDK_C},
    {'D   ',IDK_D},
    {'E   ',IDK_E},
    {'F   ',IDK_F},
    {'G   ',IDK_G},
    {'H   ',IDK_H},
    {'I   ',IDK_I},
    {'J   ',IDK_J},
    {'K   ',IDK_K},
    {'L   ',IDK_L},
    {'M   ',IDK_M},
    {'N   ',IDK_N},
    {'O   ',IDK_O},
    {'P   ',IDK_P},
    {'Q   ',IDK_Q},
    {'R   ',IDK_R},
    {'S   ',IDK_S},
    {'T   ',IDK_T},
    {'U   ',IDK_U},
    {'V   ',IDK_V},
    {'W   ',IDK_W},
    {'X   ',IDK_X},
    {'Y   ',IDK_Y},
    {'Z   ',IDK_Z},
    {'0   ',IDK_0},
    {'1   ',IDK_1},
    {'2   ',IDK_2},
    {'3   ',IDK_3},
    {'4   ',IDK_4},
    {'5   ',IDK_5},
    {'6   ',IDK_6},
    {'7   ',IDK_7},
    {'8   ',IDK_8},
    {'9   ',IDK_9},
    {'F0  ',IDK_F0},
    {'F1  ',IDK_F1},
    {'F2  ',IDK_F2},
    {'F3  ',IDK_F3},
    {'F4  ',IDK_F4},
    {'F5  ',IDK_F5},
    {'F6  ',IDK_F6},
    {'F7  ',IDK_F7},
    {'F8  ',IDK_F8},
    {'F9  ',IDK_F9},
    {'LFT ',IDK_LEFT},
    {'RGT ',IDK_RIGHT},
    {'UP  ',IDK_UP},
    {'DN  ',IDK_DOWN},
    {'BRK ',IDK_BREAK},
    {'COPY',IDK_COPY},
    {'DEL ',IDK_DEL},
    {'CAPS',IDK_CAPS},
    {'TAB ',IDK_TAB},
    {'CTRL',IDK_CTRL},
    {'SPC ',IDK_SPACE},
    {'RET ',IDK_RETURN},
    {'ESC ',IDK_ESC},
    {'LSFT',IDK_SHIFT_L},
    {'RSHT',IDK_SHIFT_R},
    {'SLCK',IDK_SHIFT_LOCK},
    {';+  ',IDK_SEMI_COLON},
    {'-=  ',IDK_EQUALS},
    {',<  ',IDK_COMMA},
    {'^~  ',IDK_CARET},
    {'.>  ',IDK_DOT},
    {'slqy',IDK_FWDSLASH},
    {'cnst',IDK_STAR},
    {'[{  ',IDK_OPEN_SQUARE},
    {'bkpi',IDK_BACKSLASH},
    {']}  ',IDK_CLOSE_SQUARE},
    {'@   ',IDK_AT},
    {'uspd',IDK_UNDERSCORE},
    
    
	{'tpmp', IDC_TAPE_CONTROL_MAP},
    {'tppl', IDC_TAPE_CONTROL_PLAY},
    {'tpst', IDC_TAPE_CONTROL_STOP},
    {'tpej', IDC_TAPE_CONTROL_EJECT},
    {'tprc', IDC_TAPE_CONTROL_RECORD},
    {'tpld', IDC_TAPE_CONTROL_LOAD_TAPE},
    {'tpnt', IDC_TAPE_CONTROL_NEW_TAPE},

    {'tpsp', IDC_PLAYING},
    {'tpsr', IDC_RECORDING},
    {'tpss', IDC_STOPPED},
    {'tpnm', IDC_TAPE_FILENAME},

	{'idok', IDOK},
	{'idcl', IDCANCEL},

	// keyboard links
	{'skbl', IDM_SET_KEYBOARD_LINKS},
	{'kbl7', IDC_KEYBOARD_BIT7},
	{'kbl6', IDC_KEYBOARD_BIT6},
	{'kbl5', IDC_KEYBOARD_BIT5},
	{'kbl4', IDC_KEYBOARD_BIT4},
	{'kbl3', IDC_KEYBOARD_BIT3},
	{'kbl2', IDC_KEYBOARD_BIT2},
	{'kbl1', IDC_KEYBOARD_BIT1},
	{'kbl0', IDC_KEYBOARD_BIT0},
    
};

std::map<int,int>
invertMap(std::map<int,int> const& myMap)
{
    std::map<int,int> other;
  
    // Traverse the map and
    // pushback the values -> keys
    for (auto const& pair : myMap) {
        other[pair.second] = pair.first;
    }
    return other;
}

std::map<int,int> RC2ID = invertMap(ID2RC);


int
ConvID2RC(unsigned int cmdID)
{
	char* cmdCHR = (char*)&cmdID;

	auto cmdRC = ID2RC.find(cmdID);
	if (cmdRC != ID2RC.end())
	{
		return cmdRC->second;
	}

	printf("ConvID2RC %c%c%c%c", cmdCHR[3], cmdCHR[2], cmdCHR[1], cmdCHR[0]);
	printf(" NOT FOUND \n" );
	return 0;
}


int
ConvRC2ID(unsigned int nID)
{
    auto cmdID = RC2ID.find(nID);
    if (cmdID != RC2ID.end())
    {
        return cmdID->second;
    }
    
    char* cmdCHR = (char*)&cmdID->second;
    if (cmdCHR[0] != 0)
    {
        printf("ConvRC2ID %d %c%c%c%c", nID, cmdCHR[3], cmdCHR[2], cmdCHR[1], cmdCHR[0]);
        printf(" NOT FOUND\n");
    }
	return -1;
}
