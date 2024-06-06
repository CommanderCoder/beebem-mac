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

		// NSSupportsSuddenTermination set to NO in 'plist'
		// so this will be called
		beeb_Destroy()
    }
    
    func applicationShouldTerminateAfterLastWindowClosed(_ sender: NSApplication) -> Bool {
        return true;
    }
    
    @IBAction func HandleCommand (_ sender: NSMenuItem) {
        
        handlingCommand = true
        let cmd: String = sender.identifier?.rawValue ?? "none"
//        print(cmd)
		let dq = DispatchQueue(label: "CPU")
		dq.suspend()
		beeb_HandleCommand(conv(cmd))
		dq.resume()
        handlingCommand = false
    }
    
    @IBOutlet weak var tapeControlMenuItem: NSMenuItem!

    
}
