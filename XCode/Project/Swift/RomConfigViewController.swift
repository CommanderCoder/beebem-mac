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
    static var rcViewControllerInstance : RomConfigViewController?

    private var selectedRow : Int = 0

    var rows = 16

    
    @IBOutlet weak var modelName: NSTextField!
    
    @IBOutlet weak var tableView: NSTableView!

    override func viewDidAppear() {
    }
    
    override func viewDidDisappear() {
    }
    
    @IBAction func BBC_B(_ sender: Any) {
        RCWindowCommandHandler(IDC_BBCB)
    }
    @IBAction func IntegraB(_ sender: Any) {
        RCWindowCommandHandler(IDC_INTEGRAB)
    }
    @IBAction func BBCBPlus(_ sender: Any) {
        RCWindowCommandHandler(IDC_BBCBPLUS)
    }
    @IBAction func Master128(_ sender: Any) {
        RCWindowCommandHandler(IDC_MASTER128)
    }
    @IBAction func SelectROM(_ sender: Any) {
        RCWindowCommandHandler(IDC_SELECTROM)
    }
    @IBAction func MarkWritable(_ sender: Any) {
        RCWindowCommandHandler(IDC_MARKWRITABLE)
    }
    @IBAction func RAM(_ sender: Any) {
        RCWindowCommandHandler(IDC_RAM)
    }
    @IBAction func Empty(_ sender: Any) {
        RCWindowCommandHandler(IDC_EMPTY)
    }
    @IBAction func LoadConfig(_ sender: Any) {
        RCWindowCommandHandler(IDC_LOAD)
    }
    @IBAction func SaveConfig(_ sender: Any) {
        RCWindowCommandHandler(IDC_SAVE)
    }
    
    @IBAction func OK(_ sender: Any) {
        beeb_FinishROMConfig()
        self.view.window!.performClose(nil)
    }
    @IBAction func Cancel(_ sender: Any) {
    }

    override func viewDidLoad() {

        super.viewDidLoad()
        // Do view setup here.

        tableView.delegate = self
        tableView.dataSource = self

        RomConfigViewController.rcViewControllerInstance = self
        tableView.reloadData()
        
        beeb_EditROMConfig();
    }
    
    func returnRowInTable() -> Int {
        return selectedRow // needs to start at 0
    }
 
    func setFocus() {
        tableView.selectRowIndexes([selectedRow], byExtendingSelection: false)
        tableView.reloadData()
    }
    
}




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
        let text: String = String(cString:beeb_getRCEntry(Int32(row), Int32(col)))

        if let cell = tableView.makeView(withIdentifier: cellIdentifier , owner: nil) as? NSTableCellView {
          cell.textField?.stringValue = text
          return cell
        }
        
        return nil
    }
    

    func setModelText(_ n : String )
    {
        print(n)
        modelName.stringValue = n
    }
    
    func tableViewSelectionDidChange(_ notification: Notification) {
        selectedRow = tableView.selectedRow
        print("\(#function) \(selectedRow) \(notification.name)")

    }
}
