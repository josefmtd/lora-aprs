# APRS on LoRa

## Introduction
This is an attempt to create an APRS compatible system on top of LoRa Radios. For the TNC, an Arduino with a LoRa Radio module will be used, the program is based on Sandeep Mistry's Arduino LoRa library which is available from https://github.com/sandeepmistry/arduino-Lora. This repository is compatible with SX1276/77/78/79 based LoRa Radio modules.
The KISS TNC on this repository can be integrated with an i-Gate software such as APRX for Linux or APRSIS-32 for Windows.

## Weather Station
One of APRS application is to share Weather Station data on RF, this repository also has a simple sketch on how to send Temperature, Humidity, and Pressure via APRS format using KISS frame on LoRa.
The sensor used in this sketch is a Bosch BME280 sensor that can easily be bought from Adafruit in a form of a breakout board. The library is also available from Adafruit Sensors library.
You can find more information about the library on https://github.com/adafruit/Adafruit_Sensor and https://github.com/adafruit/Adafruit_BME280_Library

The sensor readings will be sent every 10 minutes (the minimum amount of interval for Wx updates) and will be written in APRS format for Complete Weather Report Format with Lat/Long position, no Timestamp.

## KISS TNC
The KISS TNC on this repository can be reproduced by using Pro Mini 328p 3V3 with an Ai Thinker Ra-02 and a Bluetooth Module (optional). You can find more information on how to setup your own KISS TNC on
`lora-kiss/README.md`

If you have a Bluetooth module available, you can set it for 38400 baud and connect the Bluetooth with the Arduino so you can go ahead and connect the module via APRS Droid and you can receive weather reports from your phone.
You can also use the Bluetooth as a connection method to your Raspberry Pi and if you attach the KISS TNC to your Linux Kernel you can have a ready APRS i-Gate for LoRa

## Contact me
If you want to contact me in anyway regarding this repository you can email me on:
josef.matondang@gmail.com