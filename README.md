# Arduino-DALI Library


## by [Industrial Shields](https://www.industrialshields.com/automation-solutions-based-on-arduino-industrial-plc-raspberry-pi-panel-pc)


 Arduino-DALI Library implements DALI communication protocol in the **Ardbox-DALI and M-Duino-DALI family controllers** based on Arduino.


### Getting Started


This section shows the necessary prerequisites and the installation of Industrial Shields boards to use this library.


### Prerequisites


1- The [Arduino IDE](https://www.arduino.cc/en/Main/Software) environment 1.8.10 or higher

2- The [Industrial Shields Arduino boards](https://www.industrialshields.com/first-steps-with-the-industrial-arduino-based-plc-s-and-the-panel-pc-s-raspberry-pi-based). If you download these boards for Arduino IDE from Industrial Shields, the DALI library will also come in the same package


### Installing

Steps to install Industrial Shields Boards on Arduino IDE:

1- Open the Arduino IDE, version 1.8.10 or superior.

2- Press the “Preferences” option to “File” menu and open the preferences window.

3- In the text box “Additional boards manager URLs”, add the direction:

> http://apps.industrialshields.com/main/arduino/boards/package_industrialshields_index.json

4- Close the preferences window with the “Ok” button.

5- Click on “Tools” menu , and open the “Boards” submenu, and click the “Boards Manager” option, to open the Boards Manager window.

6- Search “industrialshields” to the search filter and select to the list and click “Install”. **Download the 1.1.24 version or upper.**

7- Close the “Boards Manager”.

Once it is performed that steps, you are available to select each PLC that you wish to work on “Tools” > “Boards”


## DALI Library

```
#include "DALI.h"
```
The **DALI library** sends and receives data through DALI pins used in the M-Duino DALI family and Ardbox DALI family controllers. **Check the user manual of your DALI family PLC to see the location of the data transfer pins via DALI.**


```
DALI dali;
```

Use this statement before the `setup()` function to indicate your DALI family controller the internal connections between the Arduino board and the DALI module attached inside the PLC.

Initializing the library:

```
dali.begin()
```

Use this statement inside the `setup()`function to initialize the library properly.

The initNodes Funtion:
```
int initNodes(6);
```
On this example we're initializing the node 6, defined as a `uint8_t` data type.

To controll two o more nodes, you can use the following function:

```
const uint8_t addresses[] = { 6, 7, 8, 9, 10 };
const int maxAddresses = sizeof(addresses) / sizeof(uint8_t);

int initNodes(const uint8_t* addresses, uint8_t numAddresses);
```

Functions for controlling the node values:

```
void turnOff(uint8_t nodeAddress);
void setMax(uint8_t nodeAddress);
void setValue(uint8_t nodeAddress, uint8_t value);
