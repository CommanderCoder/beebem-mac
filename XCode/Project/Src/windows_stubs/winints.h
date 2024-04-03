//
//  winints.h
//  BeebEm
//
//  Created by Commander Coder on 01/04/2024.
//

#ifndef winints_h
#define winints_h


#include <stdint.h>
#include <string>
typedef uint32_t DWORD;   // DWORD = unsigned 32 bit value
typedef uint16_t WORD;    // WORD = unsigned 16 bit value
typedef uint8_t BYTE;     // BYTE = unsigned 8 bit value

typedef uint32_t LONG;   // LONG = unsigned 32 bit value
typedef uint32_t UINT;   // UINT = unsigned 32 bit value

typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagRGBQUAD {
  BYTE rgbBlue;
  BYTE rgbGreen;
  BYTE rgbRed;
  BYTE rgbReserved;
} RGBQUAD;


// from Carbon
struct RECT {
  short               top;
  short               left;
  short               bottom;
  short               right;
};
typedef struct RECT                     RECT;

// 16 rows and 2 columns (only column 0 needs the bank value)
struct RCItem {
	int bank;
	std::string name;
};


#endif /* winints_h */
