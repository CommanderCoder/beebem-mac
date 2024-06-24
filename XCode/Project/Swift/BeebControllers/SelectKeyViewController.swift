//
//  SelectKeyViewController.swift
//  BeebEm
//
//  Created by Commander Coder on 12/05/2024.
//


import Cocoa

class SelectKeyViewController: NSViewController {
	
	override func viewDidDisappear() {
	 beeb_DbgHandleCommand(conv("idcl")); //IDCANCEL
	}
	
}
