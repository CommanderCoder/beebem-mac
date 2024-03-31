//
//  BreakoutBoxViewController.swift
//  BeebEm5
//
//  Created by Commander Coder on 20/10/2022.
//  Copyright © 2022 Andrew Hague. All rights reserved.
//

import Cocoa
import Carbon

class BreakoutBoxViewController: NSViewController {

    
    // allow key detection
    override var acceptsFirstResponder: Bool { return true }

    override func keyDown(with event: NSEvent) {
        let cs = event.characters?.cString(using: .ascii)?[0] ?? 64
        
        print("keydown \(cs)")
        beeb_bbhandlekeys(kEventRawKeyDown, UInt32(event.keyCode), cs)
    }
    
    override func keyUp(with event: NSEvent) {
        let cs = event.characters?.cString(using: .ascii)?[0] ?? 64
        
        print("keyup \(cs)")
        beeb_bbhandlekeys(kEventRawKeyUp, UInt32(event.keyCode), cs)
    }

    override func viewDidAppear() {
        beeb_BreakoutBoxOpenDialog()
    }
    
    override func viewDidDisappear() {
        beeb_BreakoutBoxCloseDialog()
    }
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
        
        for case let btn as NSButton in btnBits.subviewsRecursive() {
            
            print("\(btn.title) {\(btn.identifier?.rawValue ?? "unknown")}")
            btn.action = #selector(kbdbtnPressed)
        }
        
//        bbViewControllerInstance = self
    }
    
    @objc
    func kbdbtnPressed(sender: NSButton!)
    {
        print("\(sender.title) Pressed {\(sender.identifier?.rawValue ?? "unknown")}")
        BBHandleCommand(sender)
    }


    
    @IBOutlet weak var btnBits: NSStackView!
    // ADD THE ACTION TO ALL THE BUTTONS
    @IBOutlet weak var ass: NSTextField!
    // CHANGE THE TEXT
    
    
    
    
    
    // for bitbuttons and reset button
    @IBAction func BBHandleCommand(_ sender: NSButton) {
        handlingCommand = true
        let cmd: String = sender.identifier?.rawValue.padding(toLength: 4, withPad: " ", startingAt: 0) ?? "none"
        
        if (cmd == "uprs")
        {
            // breakoutbox
            // beeb_BBReset()
            print ("beeb_BBReset()")
        }
        else
        {
            let p = conv(cmd)
            beeb_BBHandleCommand(p)
        }
        handlingCommand = false

    }
}
