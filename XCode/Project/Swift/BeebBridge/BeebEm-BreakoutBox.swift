//
//  BeebEm-BreakoutBox.swift
//  BeebEm
//
//  Created by Commander Coder on 03/05/2024.
//


import Foundation
import Cocoa


// need to have given the controller an identified (StoryboardID)
let breakoutBoxWindow: NSWindowController = GetWindowCtrl(for:Dialogs.breakoutBox)

let breakoutBoxView: BreakoutBoxViewController = breakoutBoxWindow.contentViewController as! BreakoutBoxViewController


@_cdecl("swift_BBSetAssignedTo")
public func swift_BBSetAssignedTo( _ text: UnsafePointer<CChar>)
{
    print("\(#function) \(text)")
    breakoutBoxView.setAssignedToText(String(cString: text));
}
