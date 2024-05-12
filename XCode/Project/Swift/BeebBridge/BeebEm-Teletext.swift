//
//  BeebEm-Teletext.swift
//  BeebEm
//
//  Created by Commander Coder on 12/05/2024.
//


import Foundation
import Cocoa

// need to have given the controller an identified (StoryboardID)
let teletextWindow: NSWindowController =  NSStoryboard(name: "Main", bundle: nil)
	.instantiateController(withIdentifier: "SerialPortSB") as! NSWindowController

let teletextView: TeletextViewController = teletextWindow.contentViewController as! TeletextViewController


// Serial Port

@_cdecl("swift_TTOpenDialog")
public func swift_TTOpenDialog()
{
	teletextWindow.showWindow(nil);
}
