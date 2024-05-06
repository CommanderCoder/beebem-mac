//
//  BeebEm-Debug.swift
//  BeebEm
//
//  Created by Commander Coder on 06/05/2024.
//

import Foundation
import Cocoa

// need to have given the controller an identified (StoryboardID)
let dbgControlWindow: NSWindowController =  NSStoryboard(name: "Main", bundle: nil)
	.instantiateController(withIdentifier: "DebuggerSB") as! NSWindowController

let dbgControlView: DebuggerViewController = dbgControlWindow.contentViewController as! DebuggerViewController


// Tape Control


@_cdecl("swift_DbgOpenDialog")
public func swift_DbgOpenDialog()
{
	dbgControlWindow.showWindow(nil);
}
