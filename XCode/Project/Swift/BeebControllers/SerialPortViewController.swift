//
//  SerialPortViewController.swift
//  BeebEm
//
//  Created by Commander Coder on 12/05/2024.
//

import Foundation
import Cocoa

class SerialPortViewController: NSViewController {
	
	override func viewDidDisappear() {
	 beeb_DbgHandleCommand(conv("idcl")); //IDCANCEL
	}
	
	
	// for bitbuttons and reset button
	@IBAction func SPHandleCommand(_ sender: NSButton) {
		handlingCommand = true
		let cmd: String = sender.identifier?.rawValue.padding(toLength: 4, withPad: " ", startingAt: 0) ?? "none"
		let p = conv(cmd)
		beeb_SPHandleCommand(p)
		handlingCommand = false
	}
}
