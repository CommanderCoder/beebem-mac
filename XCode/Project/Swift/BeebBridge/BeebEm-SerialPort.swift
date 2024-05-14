//
//  BeebEm-SerialPort.swift
//  BeebEm
//
//  Created by Commander Coder on 12/05/2024.
//


import Foundation
import Cocoa

// need to have given the controller an identified (StoryboardID)
let serialPortWindow: NSWindowController =  NSStoryboard(name: "Main", bundle: nil)
	.instantiateController(withIdentifier: "SerialPortSB") as! NSWindowController

let serialPortView: SerialPortViewController = serialPortWindow.contentViewController as! SerialPortViewController


// Serial Port

@_cdecl("swift_DoModalSP")
public func swift_DoModalSP(caller : UnsafeMutableRawPointer)
{
	print(caller)

	serialPortWindow.showWindow(nil);
}
