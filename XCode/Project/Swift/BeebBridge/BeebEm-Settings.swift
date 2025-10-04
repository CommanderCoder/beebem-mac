//
//  BeebEm-Settings.swift
//  BeebEm
//
//  Created by Commander Coder on 06/05/2024.
//

import Foundation
import Cocoa

// need to have given the controller an identified (StoryboardID
// GetWindowCtrl this will call .instantiateController
let settingsControlWindow: NSWindowController = GetWindowCtrl(for: Dialogs.settingsWindow)

// required in allviews[] for fiddling with the objects of the view
let settingsControlView: SettingsViewController = settingsControlWindow.contentViewController as! SettingsViewController

