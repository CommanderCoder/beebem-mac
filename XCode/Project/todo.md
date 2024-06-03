#  Todo

## PRIORITY
* General testing
* IDE HDD - not tested? https://acorn.huininga.nl/pub/software/BeebEm/BeebEm-4.14.68000-20160619/Help/harddisks.html
* music 5000 http://8bs.com/emum5000.htm#m500 - working but needs massive buffers.  need to figure out why the timing is off.
* ECONET not tested
* breakout board - test
* check User Def keyboard


* joystick/mousestick
* debugger
* preference options
* Sound volume
* rs423 - using DCB, SetCommState and WaitCommEvent stuff and hSerialPort as a serial port file handle : mac to use 'unix'

* textview

* text to speech : use AVSpeechSynthesizer?




Files stored in the 'Resources' but all user modifiable files 
are in 'UserData' which is copied to
'Application Support/BeebEm' if this folder doesn't already exist.  
If any file is missing from this folder on
startup then it is copied in via 'CheckUserData'.

Windows version puts files in location referenced by 'm_UserDataFolder'
Checks UserData exists, then BeebFile, BeebState, Econet.cfg, 
AUNMap, Phroms,cfg, Roms,cfg
if UserData didn't exist; creates it and copies files if needed
Needs: UserData/*.*, RomFile.cfg and Preferences.cfg

