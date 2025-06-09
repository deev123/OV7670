# OV7670 Library for the RP2040 Raspberry Pi Pico

## Preface

The OV7670 is a bare-bones CMOS camera sensor module with a SCCB interface for registers setting the sensor settings and 8 data pins which stream half pixels in a VGA like way with a HSync and VSyc deliniating frames.
There is a version of the sensor with a buffer but this repo does not use it. Instead it uses the Pico to capture and store the image data.

<img src="https://github.com/deev123/OV7670/assets/91595217/3f21142b-7b1b-4eff-91cd-c2885df08a63" alt="OV7670 image 1" width="300"/>
<img src="https://github.com/deev123/OV7670/assets/91595217/ed44449c-9444-4dcb-a5d5-3aa60e89d2c1" alt="OV7670 image 2" width="300"/>

<a href="https://amzn.eu/d/2ike6i4">OV7670 sensor on Amazon</a>

## Repo
This repo includes:
- C firmware source code for the OV7670 camera sensor module and Raspberry Pi Pico RP2040
- A Web application for interfacing with it (which reads serial via a separate server)
- A Python serial server which is used by the web application to communicate with the sensor

Lib and Src contain the firmware

The 'Terminal' directory contains the web application for visualising data from and sending data to the camera, as well as the python 
server which should be run locally to allow the website access to a serial port



## Pin Configuration:

The pin wiring of D0-D7 allows for the most efficient reading of the bits for reading the half pixel bytes:

<img src="https://github.com/user-attachments/assets/c8c345af-8da8-4975-9492-c4372a878572" alt="Read Half Pixel Function" width="300"/>

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


## PCB

I designed a double sided PCB in KiCAD to fit the OV7670 and Pico.
The lines are length matched (hence the sqiggly traces) making it suitable for high frequency reading when I can get it to work.

<img src="https://github.com/user-attachments/assets/2857d3e9-5229-4c7d-9d31-f7428c9b1a0e" alt="Schematic" width="800"/>


<img src="https://github.com/user-attachments/assets/0f4484f4-23d4-4e5e-a747-46e66110a29e" alt="PCB CAD Front and Back" height="300"/>
<img src="https://github.com/user-attachments/assets/891a94db-da34-4024-95e3-d06abe09543e" alt="PCB CAD Front" height="300"/>
<img src="https://github.com/user-attachments/assets/9f7b451d-e112-457c-b90f-a0f1d7ca32cd" alt="PCB CAD Back" height="300"/>


<img src="https://github.com/user-attachments/assets/e271a4e0-3ada-447c-be37-1c3a59831741" alt="PCB CAD 3D" width="300"/>


There are optional pullup resistors on the I2C SDA and SCL lines but the internal ones seem to be sufficient.

There is also space for an additional female header to access spare pins to connect with something else.


I got the PCB printed from <a href="https://jlcpcb.com">JLC PCB</a>


<img src="https://github.com/user-attachments/assets/428e6b09-9183-47f8-84f1-6da98499f956" alt="PCB Photo 1" height="400"/>
<img src="https://github.com/user-attachments/assets/91f4761a-3701-4d72-b564-42162027da02" alt="PCB Photo 2" height="400"/>
<img src="https://github.com/user-attachments/assets/0a7f00a0-b1fc-4342-8396-72233ffa51da" alt="PCB Photo 3" height="400"/>


## Web Terminal

You can paste data copied from a terminal like putty or teraterm into a box for it to render

You need to have the image width set up right at least for correct rendering of rows and the height should be set to fit the image

There is also an option for big or little endian interpretation of the pixels (which are 2 bytes RGB565)


<img src="https://github.com/user-attachments/assets/bb8c1681-ce2f-4564-b108-dabc695cab8f" alt="Web Terminal Paste data tab" width="800"/>


Or listen to the serial. For this you need to have the Flask server to read the serial port.


<img src="https://github.com/user-attachments/assets/bb8c1681-ce2f-4564-b108-dabc695cab8f" alt="Web Terminal listen data tab" width="800"/>


The Pico has limited memory and since I am not yet using two cores I need to store the pixels before I can transmit them, so at the moment I skip pixels to downscale the image to something that fits in memory


<img src="https://github.com/user-attachments/assets/ff023704-df12-46f0-a5bf-b15ef3cc5529" alt="Picture from OV7670" width="800"/>


There is some unfinished work:
- I want to utilise both cores of the RP2040 processor so I can capture and trasmit the pixels at the same time in something like a producer consumer pattern. This will allow transmitting a frame of full resolution in one pass
- I need to modify register values to properly set up the camera settings to capture a coloured image
- I need to fix some data integrity issues
