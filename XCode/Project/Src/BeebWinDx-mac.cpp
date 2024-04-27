//
//  BeebWinDx.cpp
//  BeebEm
//
//  Created by Commander Coder on 10/04/2024.
//

#include <windows.h>

#include "BeebWin.h"

extern char bridgingVideobuffer[];


void Blt(char* screenbuffer, RECT destR, RECT srcR);

// colour information in 32 and 16 bit RGB
// to get from BBC RGB to MacOS RGB
int_fast32_t m_RGB32[256];
int_fast16_t m_RGB16[256];

void BeebWin::CreateColours(bmiData bmi, int ledcolbase)
{
	for (int i = 0; i < ledcolbase + 4; ++i)
	{
	  m_RGB32[i] = ((( ((bmi.bmiColors[i].rgbRed) << 8)  + (bmi.bmiColors[i].rgbGreen )) << 8) + (bmi.bmiColors[i].rgbBlue));
	  m_RGB32[i] |= 0xff000000;

	  m_RGB16[i] = ((( ((bmi.bmiColors[i].rgbRed >> 3) << 5)  + (bmi.bmiColors[i].rgbGreen >> 3)) << 5) + (bmi.bmiColors[i].rgbBlue >> 3));

	//      printf("RGB32[%d] = %08x, RGB16[%d] = %04x\n", i, m_RGB32[i], i, m_RGB16[i]);

	}
}


//only one renderer on MACOS
// - not GDI and not DX9
// - blit to secondary buffer
void BeebWin::MacVideo(int top, int left, int bottom, int right)
{
	// Work out where on screen to blit image
	// ACH - normally get width and height from the window
	RECT destRect;
	destRect = {0,0,512,640};  //top, left, bottom, right

	if (m_isFullScreen && m_MaintainAspectRatio)
	{
		// Aspect ratio adjustment
		int xAdj = (int)(m_XRatioCrop * (float)(destRect.right - destRect.left));
		int yAdj = (int)(m_YRatioCrop * (float)(destRect.bottom - destRect.top));
		destRect.left += xAdj;
		destRect.right -= xAdj;
		destRect.top += yAdj;
		destRect.bottom -= yAdj;
	}

	// Blit the whole of the secondary buffer onto the screen
	RECT srcRect;
	srcRect.left   = left;
	srcRect.top    = top;
	srcRect.right  = right;
	srcRect.bottom = bottom;

	Blt(m_screen, destRect, srcRect);
}

void Blt(char* screenbuffer, RECT destR, RECT srcR)
{
	int_fast32_t *pPtr32; // on 32 bit machine this was 'long' which ought to be 32 bits
	int_fast32_t *pRPtr32;
	int_fast16_t *pPtr16;  // on 32 bit machine this was 'short' which ought to be 16 bits
	int_fast16_t *pRPtr16;

//    PixMapHandle    pmh;
	char* buffer;
	int bpr;
	float scalex;
	float scaley;
	int ppr;
	int bpp;
	int width, height;
	int i,j;
	char *pixelVal;
	
	bpr = 640*4;// 640 pixels per row, 4 bytes per pixel
	buffer = bridgingVideobuffer; // videobuffer is char, 8 bits ; size is 640x512x4
	bpp = 32; // 4 bytes per pixel

	if (bpp == 32)
	{
		ppr = bpr / 4;
	}
	else if (bpp == 16)
	{
		ppr = bpr / 2;
	}
	else
	{
		ppr = bpr;
	}

	width = destR.right;
	height = destR.bottom;

	int xAdj = 0;
	int yAdj = 0;
	
	pixelVal = screenbuffer;
	
	// screenbuffer from m_screen (built by BeebWin) into videobuffer (use by MacOs Swift)
	

	
	// NO IDEA WHAT HAPPENS IF THIS IS FULLSCREEN - WHAT PAINTS IT?
	char* bufferptr = (buffer - destR.top * bpr - destR.left * 4 - yAdj * bpr);  // Skip past rows for window's menu bar,
	pRPtr32 = (int_fast32_t *) bufferptr;

	scalex = (float) ((srcR.right - srcR.left)) / (float) ((width));
	scaley = (float) ((srcR.bottom - srcR.top)) / (float) ((height));
	int sx[2000];

	for (i = 0; i < width; ++i)
	{
		sx[i] = (int) (i * scalex);
	}

	for (j = 0; j < height; ++j)
	{
		pixelVal = screenbuffer + (srcR.top + (int) (j * scaley)) * 800 + srcR.left;
		pPtr32 = pRPtr32 + xAdj;
		for (i = 0; i < width; ++i)
		{
			uint32_t val = (uint32_t)m_RGB32[pixelVal[sx[i]]];
//            uint32_t val = (p[sx[i]]) > 0 ? 0xffffffff:0;
			// argb
//            val |= 0xff003f00;
			*pPtr32++ = val;
		}
		
		pRPtr32 += ppr;
	}

// END TEST
	
	if (destR.top != 0)        // running full screen - don't paint !
	{
	
		pixelVal = screenbuffer;

		printf("%d : %d\n",
			   0 - destR.top * bpr - destR.left * 4 - yAdj * bpr,
			   0 - destR.top * bpr - destR.left * 2 - yAdj * bpr);
		
		pRPtr32 = (int_fast32_t *) (buffer - destR.top * bpr - destR.left * 4 - yAdj * bpr);        // Skip past rows for window's menu bar, rect.top = -22 (on my system), plus any left margin
		pRPtr16 = (short *) (buffer - destR.top * bpr - destR.left * 2 - yAdj * bpr);        // Skip past rows for window's menu bar, rect.top = -22 (on my system)

		scalex = (float) ((srcR.right - srcR.left)) / (float) ((width));
		scaley = (float) ((srcR.bottom - srcR.top)) / (float) ((height));
	

		// Pre-calculate the x scaling factor for speed

		int sx[2000];

		for (i = 0; i < width; ++i)
		{
			sx[i] = (int) (i * scalex);
		}

		switch (bpp)
		{
		case 32 :
			for (j = 0; j < height; ++j)
			{
				pixelVal = screenbuffer + (srcR.top + (int) (j * scaley)) * 800 + srcR.left;
				pPtr32 = pRPtr32 + xAdj;
				for (i = 0; i < width; ++i)
					*pPtr32++ = m_RGB32[pixelVal[sx[i]]];
//                    *pPtr32++ = (p[sx[i]]) > 0 ? 0xffffffff:0;

				pRPtr32 += ppr;
				
			}
			break;
		case 16 :
			for (j = 0; j < height; ++j)
			{
				pixelVal = screenbuffer + (srcR.top + (int) (j * scaley)) * 800 + srcR.left;
				pPtr16 = pRPtr16 + xAdj;
				
				for (i = 0; i < width; ++i)
					*pPtr16++ = m_RGB16[pixelVal[sx[i]]];
//                    *pPtr16++ = (p[sx[i]]) > 0 ? 0xffff:0;

				pRPtr16 += ppr;
				
			}
			break;
		}
	
	
	}
		

}
