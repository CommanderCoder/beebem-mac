//
//  KeyboardMappingViewController.swift
//  BeebEm5
//
//  Created by Commander Coder on 31/10/2021.
//  Copyright © 2021 Andrew Hague. All rights reserved.
//

import Cocoa
import Carbon // for key events







class KeyboardMappingViewController: NSViewController {

    @IBOutlet weak var keyboardBtns: NSStackView!
    
    @IBOutlet weak var assignedTo: NSTextField!
    
    override func viewDidAppear() {
    }
    
    override func viewDidDisappear() {
		beeb_UKHandleCommand(conv("idcl")) //IDCANCEL
		beeb_UserKeyboardClosed();
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // map all NSbuttons in keyboardBtns to 'kdbbtnPressed'
        for case let btn as NSButton in keyboardBtns.subviewsRecursive() {
                        btn.action = #selector(kbdbtnPressed)
            //            print("Mapped key \(btn.title) {\(btn.identifier?.rawValue ?? "unknown")}")
        }

    }
    
    @objc
    func kbdbtnPressed(sender: NSButton!)
    {
//        print("\(sender.title) Pressed {\(sender.identifier?.rawValue ?? "unknown")}")
        UKHandleCommand(sender)
    }
        
    // for reset button
    @IBAction func UKHandleCommand(_ sender: NSButton) {
        handlingCommand = true
//        let cmd: String = sender.identifier?.rawValue ?? "none"
        let cmd: String = sender.identifier?.rawValue.padding(toLength: 4, withPad: " ", startingAt: 0) ?? "none"
        
        if (cmd == "ukrs")
        {
            // beeb_UKReset()
            print ("beeb_UKReset()")
        }
        else
        {
            let p = conv(cmd)
            beeb_UKHandleCommand(p)
        }
        handlingCommand = false
    }

    // allow key detection
    override var acceptsFirstResponder: Bool { return true }

    override func keyDown(with event: NSEvent) {
        let cs = event.characters?.cString(using: .ascii)?[0] ?? 64
        
        print("keydown \(cs)")
        beeb_ukhandlekeys(kEventRawKeyDown, UInt32(event.keyCode), cs)
    }
    
    func setAssignedToText(_ text: String)
    {
        assignedTo.stringValue = text
    }

    func ButtonByIdentifier(_ id: String) -> NSButton? {

        for case let btn as NSButton in keyboardBtns.subviewsRecursive() {
            if (btn.identifier?.rawValue.trimmingCharacters(in: .whitespaces) == id.trimmingCharacters(in: .whitespaces))
            {
                return btn
            }
        }
        return nil
    }


    func buttonSetControlValue(_ cmdSTR: String, _ state: Int)
    {
        if let n = ButtonByIdentifier(cmdSTR)
        {
            print("TEscv",cmdSTR)
            n.state = (state==1) ? .on : .off
        }
    }
	
	
}
