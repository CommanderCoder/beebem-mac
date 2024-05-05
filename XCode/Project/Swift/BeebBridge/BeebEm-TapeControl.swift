//
//  BeebEm-Bridge-TapeController.swift
//  BeebEm
//
//  Created by Commander Coder on 28/03/2024.
//

import Foundation
import Cocoa

// need to have given the controller an identified (StoryboardID)
let tapeControlWindow: NSWindowController =  NSStoryboard(name: "Main", bundle: nil)
	.instantiateController(withIdentifier: "TapeControlSB") as! NSWindowController

let tapeControlView: TapeControlViewController = tapeControlWindow.contentViewController as! TapeControlViewController


// Tape Control

@_cdecl("swift_TCReload")
public func swift_TCReload()
{
//    print("\(#function) \(String(cString:text)) \(b)")
	tapeControlView.reloadFileList()
}


@_cdecl("swift_TCSelectItem")
public func swift_TCSelectItem( _ b:Int )
{
//    print("\(#function) \(String(cString:text)) \(b) ")
	tapeControlView.selectRowInTable(UInt(b))
}

@_cdecl("swift_TCReturnItem")
public func swift_TCReturnItem(_ text: UnsafePointer<CChar>) -> UInt
{
//    print("\(#function) \(String(cString:text)) \(b) ")
	return tapeControlView.returnRowInTable()
}



@_cdecl("swift_TCOpenDialog")
public func swift_TCOpenDialog()
{
	tapeControlWindow.showWindow(nil);
}

@_cdecl("swift_TCGetSelected")
public func swift_TCGetSelected() -> UInt
{
	return tapeControlView.returnRowInTable()
}


