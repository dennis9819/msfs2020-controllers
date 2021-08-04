# FSSCP (FligtSimulatorSerialControllsProtocoll)

## General Syntax
PC > Controller : String terminated by `\r\n`
Controller > PC : String terminated by `\n`

### First 2 Chars (REQUEST/EVENT)
* `EV` > Event / Function 
* `GS` > Get Setting 
* `SS` > Set Setting 
* `S1` > Set Servo 1 

Then follows a blank char ` `.

### Char 4 to *
#### Event / Function
The Suffix devines the event or function

Always implemented functions:
* RESET > `EV RESET` resets the controller
* CONNECTED > Sent by controller. Tells the Software that the controller is connected and is cureently initialized. It also contains the device ID. Example: `EV CONNECTED 0x0012`
* WARN > Sent by controller.  Warning event. Conatines the type of the warning. Example: `EV WARN ZERO_DEV_ID`
* INIT_DONE > Sent by controller. Tells the Software that the controller is ready.

TODO: complete