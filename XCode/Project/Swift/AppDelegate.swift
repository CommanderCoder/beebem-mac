//
//  AppDelegate.swift
//  BeebEm5
//
//  Created by Commander Coder on 24/06/2020.
//  Copyright © 2020 Andrew Hague. All rights reserved.
//

import Cocoa

var handlingCommand = false

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate
{
    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // Insert code here to initialize your application
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }
    
    func applicationShouldTerminateAfterLastWindowClosed(_ sender: NSApplication) -> Bool {
        return true;
    }
    
    @IBAction func HandleCommand (_ sender: NSMenuItem) {
        
        handlingCommand = true
        let cmd: String = sender.identifier?.rawValue ?? "none"
//        print(cmd)
        beeb_HandleCommand(conv(cmd))
        handlingCommand = false
    }
    
    @IBOutlet weak var tapeControlMenuItem: NSMenuItem!

    var exportFilesWindow: NSWindowController?

    
    @IBAction func SetDriveNumber(_ sender: NSMenuItem) {
        
        let sb = NSStoryboard(name: "Main", bundle: nil)
        // need to have given the controller an identified (StoryboardID)
        exportFilesWindow = sb.instantiateController(
            withIdentifier: "ExportFilesSB") as? NSWindowController
        
        let cmd: String = sender.identifier?.rawValue ?? "none"
        if (0 != beeb_ExportDiscFiles(conv(cmd)))
        {
            exportFilesWindow?.showWindow(self)
        }

    }

    
}
