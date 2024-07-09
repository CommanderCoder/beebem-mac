//
//  Use this file to import your target's public headers that you would like to expose to Swift.
//

// Including from Windows Src folder
#include "Resource.h"

// Exposed to AppDelegate
void beeb_HandleCommand(unsigned int cmdID);
void beeb_Destroy();

void beeb_consumer();


// Exposed to BeebSKView
void beeb_handlekeys(long message, long wParam, long lParam); // long eventkind, unsigned long keycode, char charCode
void beeb_handlemouse(long message, long wParam, long lParam);

void beeb_handlejoystick(long message, long wParam, long lParam);




// Exposed to TapeControlViewController
long beeb_TCHandleCommand(unsigned int cmdID);


long beeb_KLHandleCommand(unsigned int cmdID);


long beeb_DbgHandleCommand(unsigned int cmdID);


long beeb_getTableRowsCount(const char* tablename);
const char* beeb_getTableCellData(unsigned int property, long itemID);

// Exposed to BeebViewController
int beeb_main(long argc, char *argv[]);
void beeb_MainCpuLoop();
int beeb_end();

// Exposed to KeyboardMappingViewController [User Keyboard]
void beeb_UserKeyboardClosed();
long beeb_UKHandleCommand(unsigned int cmdID);
void beeb_ukhandlekeys(long eventkind, unsigned int keycode, char charCode);


// Exposed to RomConfigViewController
long beeb_RCHandleCommand(int cmdID);
const char* beeb_getRCEntry(unsigned int row, unsigned int column);

// Exposed to BreakoutBoxViewController
void beeb_bbhandlekeys(long eventkind, unsigned int keycode, char charCode);
void beeb_BreakoutBoxOpenDialog();
void beeb_BreakoutBoxCloseDialog();
long beeb_BBHandleCommand(unsigned int cmdID);


long beeb_SPHandleCommand(unsigned int cmdID);


// Exposed to ExportDiscViewController
void beeb_ModalOK(void* d);  //Dialog*

// make it bridge between C++ and Swift code

// Exposed to Renderer
struct CColour {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

void beeb_video(long height, long width, struct CColour buffer[]);

// expose in Swift too
#define MK_LBUTTON 0x001L
#define MK_RBUTTON 0x002L
#define MK_MBUTTON 0x010L


enum {
	aEventJoystick1Axis              = 0x0000,
	aEventJoystick1Button            = 0x0001,
	aEventJoystick2Axis              = 0x1000,
	aEventJoystick2Button            = 0x1001,
};
