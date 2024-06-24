//
//  windowsx.h
//  BeebEm
//
//  Created by Commander Coder on 04/05/2024.
//

#ifndef windowsx_h
#define windowsx_h

// windowsx.h
#define GET_WM_COMMAND_ID(wp, lp)       LOWORD(wp)
#define GET_WM_COMMAND_HWND(wp, lp)     (HWND)(lp)
#define GET_WM_COMMAND_CMD(wp, lp)      HIWORD(wp)

#endif /* windowsx_h */
