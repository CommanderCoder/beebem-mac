//
//  SettingsViewController.swift
//  BeebEm
//
//  Created by Commander Coder on 04/10/2025.
//

import Cocoa

class SettingsViewController: NSViewController {
    
    @IBOutlet weak var backgroundButton: NSButton!
    
    @IBOutlet weak var panelButton: NSButton!
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }
    
    @IBAction func backgroundToggled(_ sender: NSButton) {
        if sender.state == .on {
            beeb_setBlackBackground(1);
        } else {
            beeb_setBlackBackground(0);
        }
    }
    
    @IBAction func panelToggled(_ sender: NSButton) {
        if sender.state == .on {
            beeb_setPanelOff(1);
        } else {
            beeb_setPanelOff(0);
        }
    }
    
}


@_cdecl("swift_SetBlackBackground")
public func  swift_SetBlackBackground(_ m : Bool)
{
    let beige = #colorLiteral(red: 0.9221246839, green: 0.8101663589, blue: 0.611025393, alpha: 1)
    if (m) {
        beebViewControllerInstance?.OuterBox.fillColor = .black
    } else {
        beebViewControllerInstance?.OuterBox.fillColor = beige
    }
    
    guard let viewCtrl = allViews[Dialogs.settingsWindow] as? SettingsViewController else {return}

    viewCtrl.backgroundButton.state = m ? .on : .off
}


@_cdecl("swift_SetPanelOff")
public func  swift_SetPanelOff(_ m : Bool)
{
    beebViewControllerInstance?.LEDsStack.isHidden = m
    guard let viewCtrl = allViews[Dialogs.settingsWindow] as? SettingsViewController else {return}
    viewCtrl.panelButton.state =  m ? .on : .off
}


