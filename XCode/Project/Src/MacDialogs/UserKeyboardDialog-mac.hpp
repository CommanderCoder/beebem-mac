//
//  UserKeyboardDialog-mac.hpp
//  BeebEm
//
//  Created by Commander Coder on 01/05/2024.
//

#ifndef UserKeyboardDialog_mac_hpp
#define UserKeyboardDialog_mac_hpp

#include <stdio.h>

#include <windows.h>
//bool UK_WM_INITDIALOG();
bool UK_WM_COMMAND(WPARAM wParam);
bool UK_WM_DRAWITEM(WPARAM wParam);
bool UK_WM_CLEAR_KEY_MAPPING(WPARAM wParam);
bool UK_WM_SELECT_KEY_DIALOG_CLOSED(WPARAM wParam);


extern int selectKeyDialog_Key;
extern bool selectKeyDialog_Shift;


// Public declarations.

bool UserKeyboardDialog(HWND hwndParent);


#endif /* UserKeyboardDIalog_mac_hpp */
