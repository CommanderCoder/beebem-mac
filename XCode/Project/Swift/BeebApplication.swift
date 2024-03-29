//
//  BeebApplication.swift
//  BeebEm5
//
//  Created by Commander Coder on 05/09/2021.
//  Copyright © 2021 Andrew Hague. All rights reserved.
//

import Cocoa

class BeebApplication: NSApplication {
    
    // capture events that are not part of a menu item
    // i.e. KEYBOARD SHORTCUTS
    override func sendEvent(_ event: NSEvent) {
//                    
//                    
//        if event.type == NSEvent.EventType.keyDown {
//            if (event.modifierFlags.rawValue & NSEvent.ModifierFlags.deviceIndependentFlagsMask.rawValue) == NSEvent.ModifierFlags.command.rawValue {
//                switch event.charactersIgnoringModifiers!.lowercased() {
//                      case "t":
//                        print("cmd t")
//                        return
//                     default:
//                         break
//                     }
//                 }
//            else if (event.modifierFlags.rawValue & NSEvent.ModifierFlags.deviceIndependentFlagsMask.rawValue) == (NSEvent.ModifierFlags.command.rawValue | NSEvent.ModifierFlags.shift.rawValue) {
//                     if event.charactersIgnoringModifiers == "Z" {
//                        print("cmd SHIFT Z")
//                        return
//                     }
//                 }
//             }
//        
        
        return super.sendEvent(event)
    }

}
