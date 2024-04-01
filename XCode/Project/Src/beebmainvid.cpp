

/****************************************************************
BeebEm - BBC Micro and Master 128 Emulator
Copyright (C) 1998  Mike Wyatt

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public
License along with this program; if not, write to the Free
Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA  02110-1301, USA.
****************************************************************/

//
//  beebmainvid.cpp
//  BeebEm5
//
//  Created by Commander Coder on 22/09/2022.
//  Copyright © 2022 Andrew Hague. All rights reserved.
//
// MacOS display rendering support

#include <stdio.h>

#include "main.h"
#include "beebwin.h"
#include "beebemrc.h"
//#include "6502core.h"
//#include "ext1770.h"
//#include "avi.h"
//#include "Messages.h"
//#include "DebugTrace.h"

#ifndef BEEBEM5
extern char videobuffer[];

#include <Carbon/Carbon.h>

extern "C" void swift_SetWindowTitleWithCString(const char* title);

#endif

// NEW updateLines based on bufferblit

// compare against beebwindx.cpp
#ifdef BEEBWIN
/****************************************************************************/
void BeebWin::updateLines(HDC hDC, int starty, int nlines)
{
#endif
void BeebWin::updateLines(int starty, int nlines)
{

    static bool LastTeletextEnabled = false;
    static bool First = true;

#ifdef BEEBWIN
    HRESULT ddrval;
    HDC hdc;
#endif
    int TeletextLines = 0;
    int TextStart=240;
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

    // Changed to/from teletext mode?
    if (LastTeletextEnabled != TeletextEnabled || First)
    {
#ifdef BEEBWIN
        if (m_DisplayRenderer != IDM_DISPGDI && m_DXSmoothing && m_DXSmoothMode7Only)
#endif
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
    if (m_MotionBlur != IDM_BLUR_OFF)
    {
        if (m_MotionBlur == IDM_BLUR_2)
            j = 32;
        else if (m_MotionBlur == IDM_BLUR_4)
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

#ifndef BEEBWIN
    //only one renderer on MACOS
    // - not GDI and not DX9
    // - blit to secondary buffer
    
    {
        // Work out where on screen to blit image
#ifdef BEEBWIN
        RECT destRect;
        RECT srcRect;
        POINT pt;
        GetClientRect( m_hWnd, &destRect );
        pt.x = pt.y = 0;
        ClientToScreen( m_hWnd, &pt );
        OffsetRect(&destRect, pt.x, pt.y);
#else
        // ACH - normally get width and height from the window
        RECT destRect;
        RECT srcRect;
        destRect = {0,0,512,640};  //top, left, bottom, right
#endif

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
        srcRect.left   = 0;
        srcRect.top    = TeletextEnabled ? 0 : starty;
        srcRect.right  = TeletextEnabled ? 552 : ActualScreenWidth;
        srcRect.bottom = TeletextEnabled ? TeletextLines : starty+nlines;
    
        Blt(destRect, srcRect);
        
        
        
    }
    
#else
    if (m_DisplayRenderer == IDM_DISPGDI)
    {
        RECT destRect;
        GetClientRect(m_hWnd, &destRect);
        int win_nlines = destRect.bottom;

        TextStart = win_nlines - 20;

        int xAdj = 0;
        int yAdj = 0;

        if (m_isFullScreen && m_MaintainAspectRatio)
        {
            // Aspect ratio adjustment
            xAdj = (int)(m_XRatioCrop * (float)m_XWinSize);
            yAdj = (int)(m_YRatioCrop * (float)m_YWinSize);
        }

        StretchBlt(hDC, xAdj, yAdj, m_XWinSize - xAdj * 2, win_nlines - yAdj * 2,
                   m_hDCBitmap, 0, starty,
                   TeletextEnabled ? 552 : ActualScreenWidth,
                   TeletextEnabled ? TeletextLines : nlines,
                   SRCCOPY);

        DisplayFDCBoardInfo(hDC, 0, TextStart);
    }
    else
    {
        if (!m_DXInit)
            return;

        if (IsWindowMinimized())
            return;

        if (m_DisplayRenderer == IDM_DISPDX9)
        {
            IDirect3DSurface9 *pSurface;
            ddrval = m_pTexture->GetSurfaceLevel(0, &pSurface);
            if (ddrval == D3D_OK)
            {
                ddrval = pSurface->GetDC(&hdc);
                if (ddrval == D3D_OK)
                {
                    BitBlt(hdc, 0, 0, 800, nlines, m_hDCBitmap, 0, starty, SRCCOPY);
                    DisplayClientAreaText(hdc);
                    pSurface->ReleaseDC(hdc);

                    // Scale beeb screen to fill the D3D texture
                    int width  = TeletextEnabled ? 552 : ActualScreenWidth;
                    int height = TeletextEnabled ? TeletextLines : nlines;
                    //D3DXMatrixScaling(&m_TextureMatrix,
                    //                  800.0f/(float)width, 512.0f/(float)height, 1.0f);
                    D3DXMatrixIdentity(&m_TextureMatrix);
                    m_TextureMatrix._11 = 800.0f/(float)width;
                    m_TextureMatrix._22 = 512.0f/(float)height;

                    if (m_isFullScreen && m_MaintainAspectRatio)
                    {
                        // Aspect ratio adjustment
                        if (m_XRatioAdj > 0.0f)
                        {
                            m_TextureMatrix._11 *= m_XRatioAdj;
                            m_TextureMatrix._41 = m_XRatioCrop * 800.0f;
                        }
                        else if (m_YRatioAdj > 0.0f)
                        {
                            m_TextureMatrix._22 *= m_YRatioAdj;
                            m_TextureMatrix._42 = m_YRatioCrop * -512.0f;
                        }
                    }
                }
                pSurface->Release();
                RenderDX9();
                
            }
            if (ddrval != D3D_OK)
            {
                Report(MessageType::Error, "DirectX failure while updating screen\nFailure code %X\nSwitching to GDI",
                       ddrval);

                PostMessage(m_hWnd, WM_COMMAND, IDM_DISPGDI, 0);
            }
        }
        else
        {
            // Blit the beeb bitmap onto the secondary buffer
            ddrval = m_DDS2One->GetDC(&hdc);
            if (ddrval == DDERR_SURFACELOST)
            {
                ddrval = m_DDS2One->Restore();
                if (ddrval == DD_OK)
                    ddrval = m_DDS2One->GetDC(&hdc);
            }
            if (ddrval == DD_OK)
            {
                BitBlt(hdc, 0, 0, 800, nlines, m_hDCBitmap, 0, starty, SRCCOPY);
                DisplayClientAreaText(hdc);
                m_DDS2One->ReleaseDC(hdc);

                // Work out where on screen to blit image
                RECT destRect;
                RECT srcRect;
                POINT pt;
                GetClientRect( m_hWnd, &destRect );
                pt.x = pt.y = 0;
                ClientToScreen( m_hWnd, &pt );
                OffsetRect(&destRect, pt.x, pt.y);

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
                srcRect.left   = 0;
                srcRect.top    = 0;
                srcRect.right  = TeletextEnabled ? 552 : ActualScreenWidth;
                srcRect.bottom = TeletextEnabled ? TeletextLines : nlines;
            
                ddrval = m_DDS2Primary->Blt( &destRect, m_DDS2One, &srcRect, DDBLT_ASYNC, NULL);
                if (ddrval == DDERR_SURFACELOST)
                {
                    ddrval = m_DDS2Primary->Restore();
                    if (ddrval == DD_OK)
                        ddrval = m_DDS2Primary->Blt( &destRect, m_DDS2One, &srcRect, DDBLT_ASYNC, NULL );
                }
            }
        
            if (ddrval != DD_OK && ddrval != DDERR_WASSTILLDRAWING)
            {
                // Ignore DX errors for now - swapping between full screen and windowed DX
                // apps causes an error while transitioning between display modes.
                // It appears to correct itself after a second or two though.
                // Report(MessageType::Error,
                //        "DirectX failure while updating screen\nFailure code %X", ddrval);
            }
        }
    }

    if (aviWriter)
    {
        StretchBlt(m_AviDC, 0, 0, m_Avibmi.bmiHeader.biWidth, m_Avibmi.bmiHeader.biHeight,
                   m_hDCBitmap, 0, starty,
                   TeletextEnabled ? 552 : ActualScreenWidth,
                   TeletextEnabled ? TeletextLines : nlines,
                   SRCCOPY);

        HRESULT hr = aviWriter->WriteVideo((BYTE*)m_AviScreen);
        if (hr != E_UNEXPECTED && FAILED(hr))
        {
            Report(MessageType::Error, "Failed to write video to AVI file");
            delete aviWriter;
            aviWriter = NULL;
        }
    }

    if (m_CaptureBitmapPending)
    {
        CaptureBitmap(0,
                      starty,
                      TeletextEnabled ? 552 : ActualScreenWidth,
                      TeletextEnabled ? TeletextLines : nlines,
                      TeletextEnabled);

        m_CaptureBitmapPending = false;
    }
#endif
}



/****************************************************************************/
void BeebWin::UpdateSmoothing(void)
{
}


static const char* pszReleaseCaptureMessage = "(Press Ctrl+Alt to release mouse)";

bool BeebWin::ShouldDisplayTiming() const
{
#ifdef BEEBWIN
    return m_ShowSpeedAndFPS && (m_DisplayRenderer == IDM_DISPGDI || !m_isFullScreen);
#else
    return m_ShowSpeedAndFPS;
#endif
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

#ifdef BEEBWIN
        SetWindowText(m_hWnd, m_szTitle);
#else
        // set the window title via swift
        swift_SetWindowTitleWithCString(m_szTitle);
#endif
    }
}


void BeebWin::Blt(RECT destR, RECT srcR)
{
    int_fast32_t *pPtr32; // on 32 bit machine this was 'long' which ought to be 32 bits
    int_fast32_t *pRPtr32;
    int_fast16_t *pPtr16;  // on 32 bit machine this was 'short' which ought to be 16 bits
    int_fast16_t *pRPtr16;

//    PixMapHandle    pmh;
    Ptr             buffer;
    int                bpr;
    float            scalex;
    float            scaley;
    int                ppr;
    int                bpp;
    int width, height;
    int i,j;
    char *p;
    
    bpr = 640*4;// 640 pixels per row, 4 bytes per pixel
    buffer = videobuffer; // videobuffer is char, 8 bits ; size is 640x512x4
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
    
    p = m_screen;
    
    // from m_screen (built by BeebWin) into videobuffer (use by MacOs Swift)
    

    
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
        p = m_screen + (srcR.top + (int) (j * scaley)) * 800 + srcR.left;
        pPtr32 = pRPtr32 + xAdj;
        for (i = 0; i < width; ++i)
        {
            uint32_t val = (uint32_t)m_RGB32[p[sx[i]]];
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
    
        p = m_screen;

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
                p = m_screen + (srcR.top + (int) (j * scaley)) * 800 + srcR.left;
                pPtr32 = pRPtr32 + xAdj;
                for (i = 0; i < width; ++i)
                    *pPtr32++ = m_RGB32[p[sx[i]]];
//                    *pPtr32++ = (p[sx[i]]) > 0 ? 0xffffffff:0;

                pRPtr32 += ppr;
                
            }
            break;
        case 16 :
            for (j = 0; j < height; ++j)
            {
                p = m_screen + (srcR.top + (int) (j * scaley)) * 800 + srcR.left;
                pPtr16 = pRPtr16 + xAdj;
                
                for (i = 0; i < width; ++i)
                    *pPtr16++ = m_RGB16[p[sx[i]]];
//                    *pPtr16++ = (p[sx[i]]) > 0 ? 0xffff:0;

                pRPtr16 += ppr;
                
            }
            break;
        }
    
    
    }
        

}
