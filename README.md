# OV7670 Library for the RP2040 Raspberry Pi Pico

## Preface
TODO: Add information about what the sensor is and how it works

## Repo
This repo includes:
Firmware for the OV7670 camera sensor module
A Web application for interfacing with it.
A python serial server which is used by the Web application to communicate with the sensor

The 'Terminal' directory contains the Web application for visualising data from and sending data to the camera, as well as the python 
server which should be run locally to allow the website access to a serial port

TODO: A directory for uf2 firmware build too?



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
| SDA | GP0 |
| SCL | GP1 |
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



![image](https://github.com/user-attachments/assets/2857d3e9-5229-4c7d-9d31-f7428c9b1a0e)

![OV7670 board CAD](https://github.com/user-attachments/assets/0f4484f4-23d4-4e5e-a747-46e66110a29e)


![image](https://github.com/user-attachments/assets/428e6b09-9183-47f8-84f1-6da98499f956)

![image](https://github.com/user-attachments/assets/91f4761a-3701-4d72-b564-42162027da02)

![image](https://github.com/user-attachments/assets/0a7f00a0-b1fc-4342-8396-72233ffa51da)

