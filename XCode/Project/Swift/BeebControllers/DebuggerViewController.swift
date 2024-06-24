//
//  DebuggerViewController.swift
//  BeebEm
//
//  Created by Commander Coder on 06/05/2024.
//

import Cocoa

class DebuggerViewController: NSViewController {
	
	
	
	override func viewDidDisappear() {
	 beeb_DbgHandleCommand(conv("idcl")); //IDCANCEL
	}
	
}
