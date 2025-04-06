//
//  RomConfigViewController.swift
//  BeebEm5
//
//  Created by Commander Coder on 30/11/2022.
//  Copyright © 2022 Andrew Hague. All rights reserved.
//

import Cocoa


class RomConfigViewController: NSViewController 
{

    private var selectedRow : Int = 0

    var rows = 17 // 16 slots + 1 for the OS

    
    @IBOutlet weak var tableView: NSTableView!

	@IBOutlet weak var modelSelection: NSComboBox!

    override func viewDidAppear() {
		tableView.reloadData()
    }
    
    override func viewDidDisappear() {
		NSApp.stopModal()
    }
    @IBAction func SelectROM(_ sender: Any) {
		beeb_RCHandleCommand(IDC_SELECTROM)
    }
    @IBAction func MarkWritable(_ sender: Any) {
		beeb_RCHandleCommand(IDC_MARKWRITABLE)
    }
    @IBAction func RAM(_ sender: Any) {
		beeb_RCHandleCommand(IDC_RAM)
    }
    @IBAction func Empty(_ sender: Any) {
		beeb_RCHandleCommand(IDC_EMPTY)
    }
    @IBAction func LoadConfig(_ sender: Any) {
		beeb_RCHandleCommand(IDC_LOAD)
    }
    @IBAction func SaveConfig(_ sender: Any) {
		beeb_RCHandleCommand(IDC_SAVE)
    }
	@IBAction func Up(_ sender: Any) {
		beeb_RCHandleCommand(IDC_UP)
	}
	@IBAction func Down(_ sender: Any) {
		beeb_RCHandleCommand(IDC_DOWN)
	}
    @IBAction func OK(_ sender: Any) {
		beeb_RCHandleCommand(0x30) //IDOK
//        self.view.window!.performClose(nil)
    }
	
    @IBAction func Cancel(_ sender: Any) {
		beeb_RCHandleCommand(0x40) //IDCANCEL
    }

    override func viewDidLoad() {

        super.viewDidLoad()
        // Do view setup here.

		
        tableView.delegate = self
        tableView.dataSource = self
        tableView.reloadData()
		
		
		modelSelection.delegate = self
//		modelSelection.dataSource = self
		modelSelection.reloadData()

    }
    
    func returnRowInTable() -> Int {
        return selectedRow // needs to start at 0
    }
	func setRowInTable(_ s: Int) {
		selectedRow = s // needs to start at 0
		setFocus()
	}

    func setFocus() {
		tableView.reloadData()
        tableView.selectRowIndexes([selectedRow], byExtendingSelection: false)
    }
    
}


// combo box

//extension RomConfigViewController: NSComboBoxDataSource {
//
//	func numberOfRows(in tableView: NSComboBox) -> Int {
//	  return rows
//	}
//}
extension RomConfigViewController: NSComboBoxDelegate {
	func comboBoxSelectionDidChange(_ notification: Notification) {
		let m = (notification.object as! NSComboBox).indexOfSelectedItem
		let p = IDC_MODEL | Int32(m<<16)
		
		beeb_RCHandleCommand(p)
		tableView.reloadData()
	}
}

//table view

extension RomConfigViewController: NSTableViewDataSource {

    func numberOfRows(in tableView: NSTableView) -> Int {
      return rows
    }

}

extension RomConfigViewController: NSTableViewDelegate {

    // this function will create a cell - by the column (as a tablecolumn) and the row (as an integer))
    func tableView(_ tableView: NSTableView, viewFor tableColumn: NSTableColumn?, row: Int) -> NSView? {

        let cellIdentifier: NSUserInterfaceItemIdentifier = tableColumn!.identifier
        var col = 0 // "BANK"
        if cellIdentifier.rawValue == "FILE"
        {
            col = 1
        }
        let text: String = String(cString:beeb_getRCEntry(UInt32(row), UInt32(col)))

        if let cell = tableView.makeView(withIdentifier: cellIdentifier , owner: nil) as? NSTableCellView {
          cell.textField?.stringValue = text
          return cell
        }
        
        return nil
    }
    
    
    func tableViewSelectionDidChange(_ notification: Notification) {
        selectedRow = tableView.selectedRow
//        print("\(#function) \(selectedRow) \(notification.name)")

    }
}
