//
//  BeebEm-Bridge-KbdController.swift
//  BeebEm
//
//  Created by Commander Coder on 28/03/2024.
//

import Foundation
import Cocoa

// need to have given the controller an identified (StoryboardID)
let keyMapWindow: NSWindowController =  NSStoryboard(name: "Main", bundle: nil)
	.instantiateController(withIdentifier: "KeyMapSB") as! NSWindowController

let keyMapView: KeyboardMappingViewController = keyMapWindow.contentViewController as! KeyboardMappingViewController



@_cdecl("swift_UKSetAssignedTo")
public func swift_UKSetAssignedTo( _ text: UnsafePointer<CChar>)
{
	print("\(#function) \(text)")
	keyMapView.setAssignedToText(String(cString: text));
}


@_cdecl("swift_buttonSetControlValue")
public func swift_buttonSetControlValue(_ cmd: UInt32, _ state: Int)
{
	let cmdSTR = conv(cmd)
	
	keyMapView.buttonSetControlValue(cmdSTR, state);
}


@_cdecl("swift_UserKeyboardDialog")
public func swift_UserKeyboardDialog() -> Bool
{
	let modalresp = NSApp.runModal(for: keyMapWindow.window!)
	keyMapWindow.close()
	NSApp.stopModal()

	if (modalresp == NSApplication.ModalResponse.OK)
	{
		return true
	}
	return false
}

