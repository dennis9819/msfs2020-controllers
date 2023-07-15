# FSSCP (FligtSimulatorSerialControllsProtocoll) V2

## General Syntax
Polling:
- PC sends <1byte: command> <1byte: payload length> <n-byte payload>
- Device response: <1byte: reponse length or 0xff(error)> <n-byte: reponse>

Event:
- Device sends: <1byte: payload lenght> <n-byte payload>
- PC: no response

## Initialization
* PC Connects to CH340 Serial->USB converter
* PC sends reset signaml via CH340 Serial->USB converter
* Controller sends initial identification package
* Connection established

### Commands (REQUEST)
* `0x01` > CMD_IDENT
* `0x3F` > CMD_RESET
* `0x02` > CMD_DATA_RD
* `0x82` > CMD_DATA_WR
* `0x84` > CMD_ID_WR

#### CMD_IDENT
This command requests the device-identifier and device-type.

Example request
```
0x01 0x00
```

Expected return:
```
 0x04 0x00 0x00 0x00 0x00
 \__/ \_______/ \_______/
   |      |         + Device Type
   |      +---------- Device ID
   +----------------- Payload Length (always 4 in this case)   
```

#### CMD_RESET
This command resets the device

Example request
```
0x3F 0x00
```

Expected return:
```
 0x00 
 \__/ 
   +-- Payload Length (always 0 in this case)   
```


#### CMD_DATA_RD
This command reads a variable (8-Bit)

Example request
```
0x02 0x02 <id> <value>

0x02 0x02 0x03 0xFE
- Stores Value 0xFE in variable 0x03
```

Expected return:
```
 0x02 0x00 0x00
 \__/ \__/ \__/
   |   |     + new Value
   |   +------ variable ID
   +---------- Payload Length (always 2 in this case)   
```

#### CMD_DATA_WR
This command writes a variable (8-Bit)

Example request
```
0x82 0x01 <id>

0x82 0x01 0x03
- Reads value of variable 0x03
```

Expected return:
```
 0x02 0x00 0x00
 \__/ \__/ \__/
   |   |     + Value
   |   +------ variable ID
   +---------- Payload Length (always 2 in this case)   
```

#### CMD_ID_WR
This command writes a variable (8-Bit)

Example request
```
0x84 0x02 <16-bit uid>

0x84 0x02 0x12 0x34
 -> sets id to 0x1234
```

Expected return:
```
 0x02 0x00 0x00
 \__/ \_______/     
   |      +-- Device ID
   +--------- Payload Length (always 2 in this case)   
```
