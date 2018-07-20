# APRS on LoRa

## Introduction
This is an attempt to create an APRS compatible system on top of LoRa Modulation. For the TNC, we will be using a modified version of flok99's KISS modem and will be running on a LoRa32u4 by DIYmall. APRS software that will be used for i-Gate purpose is APRX, the program will run on a Raspberry Pi 3 board.

## Weather Station
One of the first implementation of the APRS on LoRa will be a simple weather station based on the Bosch BME280 sensor. This sensor includes Temperature, Humidity, and Barometric Pressure. We will be using Adafruit's library for BME280.

The sensor readings will be sent every 10 minutes (the minimum amount of interval for Wx updates) and will be written in APRS format for Complete Weather Report Format with Lat/Long position, no Timestamp.

## Next Step
One of the possibility that can be implemented is to add Bluetooth capability and full KISS functionality to the modem so it can work with APRSDroid.
