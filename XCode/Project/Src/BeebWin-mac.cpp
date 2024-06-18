//
//  BeebWin-mac.cpp
//  BeebEm
//
//  Created by Commander Coder on 13/06/2024.
//

#include <windows.h>

#include <stdio.h>

#include "BeebWin.h"
#include "SysVia.h"
#include "UserVia.h"

LRESULT BeebWin::AppProc(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	switch (nMessage)
	{
	   case WM_COMMAND_def: // message: command from application menu
			HandleCommand(LOWORD(wParam));
			break;
		
		//SYSKEYUP, KEYUP, SYSKEYDOWN, KEYDOWN
		
	   case MM_JOY1MOVE:
			   ScaleJoystick(LOWORD(lParam), HIWORD(lParam));
			   break;

	   case MM_JOY1BUTTONDOWN:
	   case MM_JOY1BUTTONUP:
		   JoystickButton[0] = (wParam & (JOY_BUTTON1 | JOY_BUTTON2)) != 0;
		   break;

		case WM_INPUT:
		{
		   int xDelta = GET_X_LPARAM(lParam);
		   int yDelta = GET_Y_LPARAM(lParam);

			m_RelMousePos.x += xDelta;

			if (m_RelMousePos.x < 0)
			{
				m_RelMousePos.x = 0;
			}
			else if (m_RelMousePos.x > m_XWinSize)
			{
				m_RelMousePos.x = m_XWinSize;
			}

			m_RelMousePos.y += yDelta;

			if (m_RelMousePos.y < 0)
			{
				m_RelMousePos.y = 0;
			}
			else if (m_RelMousePos.y > m_YWinSize)
			{
				m_RelMousePos.y = m_YWinSize;
			}

			ScaleMousestick(m_RelMousePos.x,
							m_RelMousePos.y);

			ChangeAMXPosition(xDelta, yDelta);
		}
			break;

	   case WM_MOUSEMOVE:
		   if (!m_MouseCaptured)
		   {
			   int xPos = GET_X_LPARAM(lParam);
			   int yPos = GET_Y_LPARAM(lParam);

			   ScaleMousestick(xPos, yPos);
			   SetAMXPosition(xPos, yPos);

			   // Experiment: show menu in full screen when cursor moved to top of window
			   HideMenu(yPos > 2);
		   }
		   break;

	   case WM_LBUTTONDOWN:
		   if (AMXMouseEnabled && m_CaptureMouse && !m_MouseCaptured)
		   {
			   CaptureMouse();
		   }
		   else
		   {
			   SetMousestickButton(0, (wParam & MK_LBUTTON) != 0);
			   AMXButtons |= AMX_LEFT_BUTTON;
		   }
		   break;

	   case WM_LBUTTONUP:
		   SetMousestickButton(0, (wParam & MK_LBUTTON) != 0);
		   AMXButtons &= ~AMX_LEFT_BUTTON;
		   break;

	   case WM_MBUTTONDOWN:
		   AMXButtons |= AMX_MIDDLE_BUTTON;
		   break;

	   case WM_MBUTTONUP:
		   AMXButtons &= ~AMX_MIDDLE_BUTTON;
		   break;

	   case WM_RBUTTONDOWN:
		   SetMousestickButton(1, (wParam & MK_RBUTTON) != 0);
		   AMXButtons |= AMX_RIGHT_BUTTON;
		   break;

	   case WM_RBUTTONUP:
		   SetMousestickButton(1, (wParam & MK_RBUTTON) != 0);
		   AMXButtons &= ~AMX_RIGHT_BUTTON;
		   break;

	}
	return 0;

}
