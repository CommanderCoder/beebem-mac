//
//  BeebEm-Teletext.swift
//  BeebEm
//
//  Created by Commander Coder on 12/05/2024.
//


import Foundation
import Cocoa

// need to have given the controller an identified (StoryboardID)
let teletextWindow: NSWindowController = NSStoryboard(name: "Main", bundle: nil)
	.instantiateController(withIdentifier: "TeletextSB") as! NSWindowController

let teletextView: TeletextViewController = teletextWindow.contentViewController as! TeletextViewController


