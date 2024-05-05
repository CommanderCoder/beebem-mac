//
//  BeebEm-BreakoutBox.swift
//  BeebEm
//
//  Created by Commander Coder on 03/05/2024.
//


import Foundation
import Cocoa


// need to have given the controller an identified (StoryboardID)
let breakoutBoxWindow: NSWindowController =  NSStoryboard(name: "Main", bundle: nil)
	.instantiateController(withIdentifier: "BreakoutBoxSB") as! NSWindowController

let breakoutBoxView: BreakoutBoxViewController = breakoutBoxWindow.contentViewController as! BreakoutBoxViewController


@_cdecl("swift_BreakoutBoxDialog")
public func swift_BreakoutBoxDialog() -> Bool
{
	breakoutBoxWindow.showWindow(nil);
//	let modalresp = NSApp.runModal(for: breakoutBoxWindow.window!)
//	breakoutBoxWindow.close()
//	NSApp.stopModal()
//
//	if (modalresp == NSApplication.ModalResponse.OK)
//	{
		return true
//	}
//	return false
}
