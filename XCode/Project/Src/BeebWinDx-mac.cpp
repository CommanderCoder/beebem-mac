//
//  BeebWinDx.cpp
//  BeebEm
//
//  Created by Commander Coder on 10/04/2024.
//

#include <windows.h>
#include <ddraw.h>

#include <stdio.h>

#include "BeebWin.h"
#include "6502core.h"
#include "AVIWriter.h"
#include "DebugTrace.h"
#include "Ext1770.h"
#include "Main.h"
#include "Messages.h"
#include "Resource.h"



// NOTE REGARDING DIFFERENCES TO BeebWinDX.cpp
// Only need to include differences for
/*
 * BeebWin::updateLines
 * BeebWin::IsWindowMinimized
 * pszReleaseCaptureMessage
 * BeebWin::ShouldDisplayTiming
 * BeebWin::DisplayTiming()
 * BeebWin::UpdateWindowTitle()
 * BeebWin::SetMotionBlur
 * BeebWin::UpdateMotionBlurMenu
 
 * and any of the empty methods below
 */

void BeebWin::InitDX(){}
void BeebWin::ResetDX(){}
void BeebWin::ReinitDX(){}
void BeebWin::ExitDX(){}
void BeebWin::UpdateSmoothing(){}
void BeebWin::CloseDX9(){}


void VideoAddLeds_Mac()
{
	swift_SetMachineType(MachineType);
	
	// Swift LEDs here
	
//	if (LEDs.ShowKB)
	{
		if (MachineType == Model::Master128 || MachineType == Model::MasterET) {
			swift_SetLED(CASS,true);
		}
		else {
			swift_SetLED(CASS,LEDs.Motor);
		}
		swift_SetLED(CAPS,LEDs.CapsLock);
		swift_SetLED(SHIFT,LEDs.ShiftLock);
	}

//	if (LEDs.ShowDisc)
	{
		swift_SetLED(HD0,LEDs.HDisc[0]);
		swift_SetLED(HD1,LEDs.HDisc[1]);
		swift_SetLED(HD2,LEDs.HDisc[2]);
		swift_SetLED(HD3,LEDs.HDisc[3]);
		swift_SetLED(FD0,LEDs.FloppyDisc[0]);
		swift_SetLED(FD1,LEDs.FloppyDisc[1]);
	}
}

/****************************************************************************/

// the video buffer found in 'BeebEm-Bridging-Video.cpp'
extern char bridgingVideobuffer[];

// colour information in 32 and 16 bit RGB
// to get from BBC RGB to MacOS RGB
int_fast32_t m_RGB32[256];
int_fast16_t m_RGB16[256];


/****************************************************************************/
void BeebWin::updateLines(HDC hDC, int starty, int nlines)
{
	static bool LastTeletextEnabled = false;
	static bool First = true;
	
//	HRESULT ddrval;
//	HDC hdc;
	int TeletextLines = 0;
//	int TextStart=240;
	int i,j;

	// Not initialised yet?
	if (m_screen == NULL)
		return;

	// Check for text view
	if (m_TextViewEnabled)
	{
		TextView();
		return;
	}

	// make sure the LEDs are up to date
	VideoAddLeds_Mac();
	
	// Changed to/from teletext mode?
	if (LastTeletextEnabled != TeletextEnabled || First)
	{
		if (m_DisplayRenderer != DisplayRendererType::GDI && m_DXSmoothing && m_DXSmoothMode7Only)
		{
			UpdateSmoothing();
		}

		LastTeletextEnabled = TeletextEnabled;
		First = false;
	}

	// Use last stored params?
	if (starty == 0 && nlines == 0)
	{
		starty = m_LastStartY;
		nlines = m_LastNLines;
	}
	else
	{
		m_LastStartY = starty;
		m_LastNLines = nlines;
	}

	++m_ScreenRefreshCount;
	TeletextLines = 500 / TeletextStyle;

	// Do motion blur
	if (m_MotionBlur != 0)
	{
		if (m_MotionBlur == 2)
			j = 32;
		else if (m_MotionBlur == 4)
			j = 16;
		else // blur 8 frames
			j = 8;

		for (i = 0; i < 800*512; ++i)
		{
			if (m_screen[i] != 0)
			{
				m_screen_blur[i] = m_screen[i];
			}
			else if (m_screen_blur[i] != 0)
			{
				m_screen_blur[i] += j;
				
				if (m_screen_blur[i] > 63)
					m_screen_blur[i] = 0;
			}
		}

		memcpy(m_screen, m_screen_blur, 800*512);
	}

	
	//MacVideo
	// Work out where on screen to blit image
	// ACH - normally get width and height from the window
	RECT destRect;
	destRect = {0,0,512,640};  //top, left, bottom, right

	if (m_FullScreen && m_MaintainAspectRatio)
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
	srcRect.left   = 0;
	srcRect.top    = TeletextEnabled ? 0 : starty;
	srcRect.right  = TeletextEnabled ? 552 : ActualScreenWidth;
	srcRect.bottom = TeletextEnabled ? TeletextLines : starty+nlines;


	//BLT
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

	width = destRect.right;
	height = destRect.bottom;

	int xAdj = 0;
	int yAdj = 0;
	
	pixelVal = m_screen;
	
	// screenbuffer from m_screen (built by BeebWin) into videobuffer (use by MacOs Swift)
	

	
	// NO IDEA WHAT HAPPENS IF THIS IS FULLSCREEN - WHAT PAINTS IT?
	char* bufferptr = (buffer - destRect.top * bpr - destRect.left * 4 - yAdj * bpr);  // Skip past rows for window's menu bar,
	pRPtr32 = (int_fast32_t *) bufferptr;

	scalex = (float) ((srcRect.right - srcRect.left)) / (float) ((width));
	scaley = (float) ((srcRect.bottom - srcRect.top)) / (float) ((height));
	int sx[2000];

	for (i = 0; i < width; ++i)
	{
		sx[i] = (int) (i * scalex);
	}

	for (j = 0; j < height; ++j)
	{
		pixelVal = m_screen + (srcRect.top + (int) (j * scaley)) * 800 + srcRect.left;
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
	
	if (destRect.top != 0)        // running full screen - don't paint !
	{
	
		pixelVal = m_screen;

		printf("%d : %d\n",
			   0 - destRect.top * bpr - destRect.left * 4 - yAdj * bpr,
			   0 - destRect.top * bpr - destRect.left * 2 - yAdj * bpr);
		
		pRPtr32 = (int_fast32_t *) (buffer - destRect.top * bpr - destRect.left * 4 - yAdj * bpr);        // Skip past rows for window's menu bar, rect.top = -22 (on my system), plus any left margin
		pRPtr16 = (short *) (buffer - destRect.top * bpr - destRect.left * 2 - yAdj * bpr);        // Skip past rows for window's menu bar, rect.top = -22 (on my system)

		scalex = (float) ((srcRect.right - srcRect.left)) / (float) ((width));
		scaley = (float) ((srcRect.bottom - srcRect.top)) / (float) ((height));
	

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
				pixelVal = m_screen + (srcRect.top + (int) (j * scaley)) * 800 + srcRect.left;
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
				pixelVal = m_screen + (srcRect.top + (int) (j * scaley)) * 800 + srcRect.left;
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


/****************************************************************************/
bool BeebWin::IsWindowMinimized() const
{
	return swift_IsMiniaturized();
}

/****************************************************************************/

static const char* pszReleaseCaptureMessage = "(Press Ctrl+Alt to release mouse)";

bool BeebWin::ShouldDisplayTiming() const
{
	return m_ShowSpeedAndFPS && (m_DisplayRenderer == DisplayRendererType::GDI || !m_FullScreen);
}

void BeebWin::DisplayTiming()
{
	if (ShouldDisplayTiming())
	{
		if (m_MouseCaptured)
		{
			sprintf(m_szTitle, "%s  Speed: %2.2f  fps: %2d  %s",
					WindowTitle, m_RelativeSpeed, (int)m_FramesPerSecond, pszReleaseCaptureMessage);
		}
		else
		{
			sprintf(m_szTitle, "%s  Speed: %2.2f  fps: %2d",
					WindowTitle, m_RelativeSpeed, (int)m_FramesPerSecond);
		}
		SetWindowText(m_hWnd, m_szTitle);
	}
}

void BeebWin::UpdateWindowTitle()
{
	if (ShouldDisplayTiming())
	{
		DisplayTiming();
	}
	else
	{
		if (m_MouseCaptured)
		{
			sprintf(m_szTitle, "%s  %s",
					WindowTitle, pszReleaseCaptureMessage);
		}
		else
		{
			strcpy(m_szTitle, WindowTitle);
		}
		SetWindowText(m_hWnd, m_szTitle);
	}
}


/****************************************************************************/


void BeebWin::SetMotionBlur(int MotionBlur)
{
	m_MotionBlur = MotionBlur;

	UpdateMotionBlurMenu();
}

void BeebWin::UpdateMotionBlurMenu()
{
	static const struct { UINT ID; int MotionBlur; } MenuItems[] =
	{
		{ IDM_BLUR_OFF, 0 },
		{ IDM_BLUR_2,   2 },
		{ IDM_BLUR_4,   4 },
		{ IDM_BLUR_8,   8 }
	};

	UINT SelectedMenuItemID = 0;

	for (int i = 0; i < _countof(MenuItems); i++)
	{
		if (m_MotionBlur == MenuItems[i].MotionBlur)
		{
			SelectedMenuItemID = MenuItems[i].ID;
			break;
		}
	}

	CheckMenuRadioItem(IDM_BLUR_OFF, IDM_BLUR_8, SelectedMenuItemID);
}

/****************************************************************************/
