//
//  Use this file to import your target's public headers that you would like to expose to Swift.
//

// Including from Windows Src folder
#include "Resource.h"

// Exposed to AppDelegate
void beeb_HandleCommand(unsigned int cmdID);


// Exposed to BeebSKView
void beeb_handlekeys(long eventkind, unsigned long keycode, char charCode);




// Exposed to TapeControlViewController
void beeb_TapeControlOpenDialog();
void beeb_TapeControlCloseDialog();
long beeb_TCHandleCommand(unsigned int cmdID);


long beeb_getTableRowsCount(const char* tablename);
const char* beeb_getTableCellData(unsigned int property, long itemID);

// Exposed to BeebViewController
int beeb_main(long argc, char *argv[]);
void beeb_MainCpuLoop();
int beeb_end();

// Exposed to KeyboardMappingViewController [User Keyboard]
void beeb_UserKeyboardOpen();
long beeb_UKHandleCommand(unsigned int cmdID);
void beeb_ukhandlekeys(long eventkind, unsigned int keycode, char charCode);


// Exposed to RomConfigViewController
void beeb_EditROMConfig();
void beeb_FinishROMConfig();
int RCWindowCommandHandler(int);
const char* beeb_getRCEntry(int row, int column);

// Exposed to BreakoutBoxViewController
void beeb_bbhandlekeys(long eventkind, unsigned int keycode, char charCode);
void beeb_BreakoutBoxOpenDialog();
void beeb_BreakoutBoxCloseDialog();
long beeb_BBHandleCommand(unsigned int cmdID);


// Exposed to ExportDiscViewController
void beeb_exportSelected(void* d);  //ExportFileDialog*

// make it bridge between C++ and Swift code

// Exposed to Renderer
struct CColour {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

void beeb_video(long height, long width, struct CColour buffer[]);

