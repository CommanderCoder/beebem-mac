//
//  BeebEm-Bridging-Video.cpp
//  BeebEm
//
//  Created by Commander Coder on 10/04/2024.
//

#include "BeebEm-Bridging-Video.hpp"

struct CColour{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

int y = 0;
const int width = 640;
const int height = 512;


const int bpp = 4; // 4 bytes or 32 bits
unsigned char bridgingVideobuffer[bpp * width * height]; // 200x200 by 4 bytes

extern "C" void beeb_video(int height, int width, struct CColour buffer[])
{
	for (int i = 0; i < width*height; i++)
	{
		buffer[i].b = bridgingVideobuffer[0+(i*bpp)];
		buffer[i].g = bridgingVideobuffer[1+(i*bpp)];
		buffer[i].r = bridgingVideobuffer[2+(i*bpp)];
		buffer[i].a = bridgingVideobuffer[3+(i*bpp)];
	}
	// move a green line everytime this is updated (50fps)
	y += 1;
	y %= height-2;
}
