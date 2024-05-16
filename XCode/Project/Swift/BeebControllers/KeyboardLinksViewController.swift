//
//  KeyboardLinksViewController.swift
//  BeebEm
//
//  Created by Commander Coder on 13/05/2024.
//

import Foundation
import Cocoa

class KeyboardLinksViewController: NSViewController {
	
	override func viewDidDisappear() {
	// beeb_KLHandleCommand(conv("idcl")); //IDCANCEL
	}
	
	@IBAction func OKCancelPressed(sender: NSButton!)
	{
		print("\(sender.title) Pressed {\(sender.identifier?.rawValue ?? "unknown")}")
		
		let cmd: String = sender.identifier?.rawValue.padding(toLength: 4, withPad: " ", startingAt: 0) ?? "none"
		
		let p = conv(cmd)

		beeb_KLHandleCommand(p)

	}

	@IBOutlet weak var btnBits: NSStackView!

	override func viewDidLoad() {
		super.viewDidLoad()
		// Do view setup here.
		
//		for case let btn as NSButton in btnBits.subviewsRecursive() {
//			
//			print("\(btn.title) {\(btn.identifier?.rawValue ?? "unknown")}")
//			btn.action = #selector(kbdbtnPressed)
//		}
		
//        bbViewControllerInstance = self
	}
//
//	@objc
//	func kbdbtnPressed(sender: NSButton!)
//	{
//		print("\(sender.title) Pressed {\(sender.identifier?.rawValue ?? "unknown")}")
//
//		let cmd: String = sender.identifier?.rawValue.padding(toLength: 4, withPad: " ", startingAt: 0) ?? "none"
//
//		let p = conv(cmd)
//
//		beeb_KLHandleCommand(p)
//	}
}
