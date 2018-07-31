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
Inspired by markqvist's RNode firmware, I checked his source code and made it work for ATmega328p based board. You can build a KISS TNC for LoRa by assembling a quick breadboard prototype, with e.g. Arduino Pro Mini, a Breakout board for Ai-Thinker Ra-02, and a Bluetooth module (optional).
The transmit algorithm used in this repository is copied from markqvist's source code and the receive algorithm is done by API provided by Sandeep Mistry.

If you manage to build one on breadboard with a Bluetooth on-board, you can connect this KISS TNC via APRS Droid and begin using this KISS TNC as an APRS Tracker.

If you are interested in a plug and play APRS LoRa, you can head over to markqvist's shop and find a ready and plug-and-play device to use: https://unsigned.io/projects/rnode

## Contact me
If you want to contact me in anyway regarding this repository you can email me on:
josef.matondang@gmail.com