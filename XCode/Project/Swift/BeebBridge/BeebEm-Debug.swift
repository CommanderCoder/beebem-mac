//
//  BeebEm-Debug.swift
//  BeebEm
//
//  Created by Commander Coder on 06/05/2024.
//

import Foundation
import Cocoa

// need to have given the controller an identified (StoryboardID)
let dbgControlWindow: NSWindowController = GetWindowCtrl(for: Dialogs.debugWindow)

let dbgControlView: DebuggerViewController = dbgControlWindow.contentViewController as! DebuggerViewController

