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
	
}
