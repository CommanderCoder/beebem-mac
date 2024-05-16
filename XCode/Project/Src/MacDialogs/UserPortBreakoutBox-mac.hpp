//
//  UserPortBreakoutBox-mac.hpp
//  BeebEm
//
//  Created by Commander Coder on 02/05/2024.
//

#ifndef UserPortBreakoutBox_mac_hpp
#define UserPortBreakoutBox_mac_hpp

#include <stdio.h>

#include "apples.h"

/* User Port Breakout Box */

class UserPortBreakoutDialog
{
	public:
		UserPortBreakoutDialog(
			HINSTANCE hInstance,
			HWND hwndParent
		);

		bool Open();
		void Close();

		bool KeyDown(int Key);
		bool KeyUp(int Key);

		void ShowBitKey(int key, int ctrlID);
		void ShowInputs(unsigned char data);
		void ShowOutputs(unsigned char data);

//	private:
//		static INT_PTR CALLBACK sDlgProc(
//			HWND   hwnd,
//			UINT   nMessage,
//			WPARAM wParam,
//			LPARAM lParam
//		);
//
//		INT_PTR DlgProc(
//			HWND   hwnd,
//			UINT   nMessage,
//			WPARAM wParam,
//			LPARAM lParam
//		);

	bool WM_INITDIALOG();
	bool WM_COMMAND(WPARAM wParam);
	void WM_CLEAR_KEY_MAPPING();
	bool WM_SELECT_KEY_DIALOG_CLOSED(WPARAM wParam);

	
		void PromptForBitKeyInput(int bitKey);
		bool GetValue(int ctrlID);
		void SetValue(int ctrlID, bool State);

	private:
		HINSTANCE m_hInstance;
		HWND m_hwnd;
		HWND m_hwndParent;
		int m_BitKey;
		unsigned char m_LastInputData;
		unsigned char m_LastOutputData;
};

extern int BitKeys[8];

extern UserPortBreakoutDialog* userPortBreakoutDialog;
#endif /* UserPortBreakoutBox_mac_hpp */
