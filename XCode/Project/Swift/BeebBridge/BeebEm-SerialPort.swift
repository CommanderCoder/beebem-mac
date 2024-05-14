//
//  BeebEm-SerialPort.swift
//  BeebEm
//
//  Created by Commander Coder on 12/05/2024.
//


import Foundation
import Cocoa

// need to have given the controller an identified (StoryboardID)
let serialPortWindow: NSWindowController = GetWindowCtrl(for: Dialogs.serialPort)

let serialPortView: SerialPortViewController = serialPortWindow.contentViewController as! SerialPortViewController
