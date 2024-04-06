# OV7670 Library for the RP2040 Raspberry Pi Pico

This repo includes firmware for the OV7670 camera sensor module and a Web application for interfacing with it.

The 'Terminal' directory contains the Web application for visualising data from and sending data to the camera

TODO: Move cmake stuff into a separate directory so theres separate firmware and terminal folders. Then separate library from firmware. A directory for uf2 firmware build too?



![61f8wxd43BL _AC_UF894,1000_QL80_](https://github.com/deev123/OV7670/assets/91595217/3f21142b-7b1b-4eff-91cd-c2885df08a63)
![61gR37LPc8L _SX522_](https://github.com/deev123/OV7670/assets/91595217/ed44449c-9444-4dcb-a5d5-3aa60e89d2c1)

https://amzn.eu/d/2ike6i4


# Pin Configuration:

| OV7670 Pin | PICO Pin |
| - | - |
| 3.3V | 3V3(OUT) |
| RET | 3V3(OUT) |
| DGND | GND |
| PWDN | GND |
| SDA | GP1 |
| SCL | GP2 |
| XLK | GP21 |
| VS | GP6 |
| HS | GP5 |
| PLK | GP7 |
| D0 | GP8 |
| D1 | GP9 |
| D2 | GP10 |
| D3 | GP11 |
| D4 | GP12 |
| D5 | GP13 |
| D6 | GP14 |
| D7 | GP15 |

TODO: Wiring Diagram
