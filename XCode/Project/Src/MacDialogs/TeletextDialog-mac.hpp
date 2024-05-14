//
//  TeletextDialog-mac.hpp
//  BeebEm
//
//  Created by Commander Coder on 12/05/2024.
//

#ifndef TeletextDialog_mac_hpp
#define TeletextDialog_mac_hpp

#include <stdio.h>

#include <string>

#include "Dialog.h"
#include "Teletext.h"

class TeletextDialog : public Dialog
{
	public:
		TeletextDialog(
			HINSTANCE hInstance,
			HWND hwndParent,
			TeletextSourceType Source,
			const std::string& DiscsPath,
			const std::string* FileName,
			const std::string* IPAddress,
			const u_short* IPPort
		);

	public:
		TeletextSourceType GetSource() const;
		const std::string& GetFileName(int Index) const;
		const std::string& GetIPAddress(int Index) const;
		u_short GetPort(int Index) const;

	bool DoModal();

	private:
	virtual bool WM_INITDIALOG();
	virtual bool WM_COMMAND(WPARAM wParam);
	virtual void WM_NOTIFY(){};

	BOOL OnCommand(int Notification, int nCommandID);

//		virtual INT_PTR DlgProc(
//			UINT   nMessage,
//			WPARAM wParam,
//			LPARAM lParam
//		);
//
//		INT_PTR OnCommand(int Notification, int nCommandID);
	
		void SelectFile(int Channel);

		BOOL GetChannelIPControls(int Channel);
		void SetChannelIPControls(int Channel);

		void EnableFileControls(bool bEnable);
		void EnableIPControls(bool bEnable);

	private:
		TeletextSourceType m_TeletextSource;
		std::string m_DiscsPath;
		std::string m_FileName[4];
		std::string m_IPAddress[4];
		u_short m_IPPort[4];
};
#endif /* TeletextDialog_mac_hpp */
