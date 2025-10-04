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
		
		let timer = Timer.scheduledTimer(withTimeInterval: 1.0/10, repeats: true)
		{ _ in
			// update the event consumer
			beeb_consumer()
		}

		RunLoop.current.add(timer, forMode: .common)
    }

	func applicationWillFinishLaunching(_ notification: Notification) {
		UserDefaults.standard.set(true, forKey: "NSDisabledDictationMenuItem")
		UserDefaults.standard.set(true, forKey: "NSDisabledCharacterPaletteMenuItem")
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

    // this is triggered by CMD-, settings menu
    @IBAction func showSettingsWindow(_ sender: Any) {
        // NOTE: settingsControlWindow is created in SettingsViewController

        if let window = settingsControlWindow.window {
            // Bring the window to the front
            window.makeKeyAndOrderFront(sender)
        }
    }
    
}
