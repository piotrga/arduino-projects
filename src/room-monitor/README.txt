TODOs:

Questions:
DONE - clock, millis() seems to reset very often (16bit?) 
DONE - scaling temp sensor 
DONE - scaling light sensor
How to build a nice cover? Are there any on the market?

IDEAS:
---
- Show buffer full/half full status
- Show battery status
- Show temperature using diods
- Show temperature using display 8x8
- Show probing as blink



Scaling temp from LM35DT:
	10mV/degree, ie 30C = 300mV
	Port sensitivity = 0-1100mV mapped to 0-1023.
	V(mV) = (pin_reading / 1024) * 1100
	Temp = V / 10 - 2

MultiMeter, VC99 20 

Rapid Electronics - cheap provider
Maplin - breadboards


SeedStudio - company which burns PCBs and can also solder the components on them

DS1820 - Dallas temperature sensor

APC220 - Wireless modules 



5 - 20 K

		5 + x 