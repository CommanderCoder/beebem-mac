//
//  SerialPortDialog-mac.hpp
//  BeebEm
//
//  Created by Commander Coder on 12/05/2024.
//

#ifndef SerialPortDialog_mac_hpp
#define SerialPortDialog_mac_hpp

#include <stdio.h>

#include <string>

#include "Dialog.h"
#include "Serial.h"

class SerialPortDialog : public Dialog
{
	public:
		SerialPortDialog(
			HINSTANCE hInstance,
			HWND hwndParent,
			SerialType Destination,
			const char* PortName,
			const char* IPAddress,
			int IPPort,
			bool IP232RawComms,
			bool IP232Handshake
		);

	public:
		SerialType GetDestination() const { return m_Destination; }
		const std::string& GetSerialPortName() const { return m_SerialPortName; }
		const std::string& GetIPAddress() const { return m_IPAddress; }
		int GetIPPort() const { return m_IPPort; }
		bool GetIP232RawComms() const { return m_IP232RawComms; }
		bool GetIP232Handshake() const { return m_IP232Handshake; }
		bool DoModal();

	virtual bool WM_INITDIALOG();
	virtual bool WM_COMMAND(WPARAM wParam);
	virtual void WM_NOTIFY() {}

	private:

//		virtual INT_PTR DlgProc(
//			UINT   nMessage,
//			WPARAM wParam,
//			LPARAM lParam
//		);

		void EnableSerialPortControls(bool bEnable);
		void EnableIP232Controls(bool bEnable);

	private:
		SerialType m_Destination;
		std::string m_SerialPortName;
		std::string m_IPAddress;
		int m_IPPort;
		bool m_IP232RawComms;
		bool m_IP232Handshake;
};

extern SerialPortDialog* serialPortDialog;

#endif /* SerialPortDialog_mac_hpp */
