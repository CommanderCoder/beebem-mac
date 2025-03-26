//
//  BeebListWindowController.swift
//  BeebEm5
//
//  Created by Commander Coder on 08/01/2023.
//  Copyright © 2023 Andrew Hague. All rights reserved.
//

import Foundation
import AppKit

//https://www.appcoda.com/macos-programming-tableview/

class BeebListData {
    
    var selectedFiles : [Int32] = []
    
    var rows: [ String ] = []
    
    func clear()
    {
        rows = []
    }
    
    func setrow(_ val : String)
    {
        rows.append(val)
    }
}

class ExportDiscViewController: NSViewController {

	var beeblistdata = BeebListData()

    var driveNumber : String?
    
    @IBOutlet weak var tableView: NSTableView!
 
    override func viewDidLoad() {
        
        super.viewDidLoad()
        
        tableView.allowsMultipleSelection = true

        tableView.delegate = self
        tableView.dataSource = self
        
    }
    
    override func viewDidAppear() {
        super.viewDidAppear()
        
        // App Delegate will show this window
        // it will attempt to reload data
        // this will render the rows and columns (getting rows from beeblistdata rows
        // so need to fill beeblistdata with info
        // for the DRIVE selected.
        // disable to the menu item
        // for the DRIVE selected.
        
        tableView.reloadData()
    }

	
    @IBAction func exportSelected(_ sender : NSButton) {
        
//		print("Exported \(sender)")
		
		NSApp.stopModal(withCode: NSApplication.ModalResponse.OK)
        
        beeblistdata.selectedFiles.removeAll()
        for r in tableView.selectedRowIndexes
        {
            beeblistdata.selectedFiles.append(Int32(r))
//            print(">> \(r)")
        }
        
    }
	
	@IBAction func exportClose(_ sender : NSButton) {
		NSApp.stopModal(withCode: NSApplication.ModalResponse.cancel)
	}
}

extension ExportDiscViewController: NSTableViewDataSource {
    func numberOfRows(in tableView: NSTableView) -> Int {
        return beeblistdata.rows.count
    }
}

extension ExportDiscViewController: NSTableViewDelegate {
    
    
    fileprivate enum CellIdentifiers {
      static let ColumnCell = NSUserInterfaceItemIdentifier(rawValue: "COLUMN")
    }

    func tableView(_ tableView: NSTableView, rowViewForRow row: Int) -> NSTableRowView? {
//        print("\(#function) \(row) \(beeblistdata.rows[row])")
        return nil
    }
    
    func tableView(_ tableView: NSTableView, viewFor tableColumn: NSTableColumn?, row: Int) -> NSView? {
//        print("\(#function) \(tableColumn!.identifier) \(row) \(beeblistdata.rows[row])")

        if let cell = tableView.makeView(withIdentifier: tableColumn!.identifier, owner: nil) as? NSTableCellView {
            cell.textField?.stringValue = beeblistdata.rows[row]
            return cell
        }
        return nil
    }
    
}



extension ExportDiscViewController: NSWindowDelegate {
	func windowShouldClose(_ sender: NSWindow) -> Bool {
		print("not closing the window!")
		return false
	}
}
