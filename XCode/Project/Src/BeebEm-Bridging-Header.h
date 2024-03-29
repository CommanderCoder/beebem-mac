//
//  Use this file to import your target's public headers that you would like to expose to Swift.
//


// Exposed to AppDelegate
void beeb_HandleCommand(unsigned int cmdID);
long beeb_ExportDiscFiles( unsigned int driveID);

// Exposed to BeebViewController
int beeb_main(long argc, char *argv[]);
void beeb_MainCpuLoop();
int beeb_end();

// Exposed to Renderer
struct CColour{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

void beeb_video(long height, long width, struct CColour buffer[]);

// Exposed to BeebSKView
void beeb_handlekeys(long eventkind, unsigned long keycode, char charCode);



