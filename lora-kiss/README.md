# LoRa RX-Only KISS TNC

This sketch is for an Arduino based KISS TNC using a LoRa radio.
This sketch will be compatible with Semtech SX1276/77/78/79 based boards.

This sketch is based on library made by Sandeep Mistry on LoRa.
You can access the library on https://github.com/sandeepmistry/arduino-LoRa

## How to Make Your Own KISS TNC

To create your own KISS TNC, you will need an Arduino (I am using Pro Mini) and
a LoRa module (I am using Ai Thinker Ra-02 breakout).

You can connect your LoRa module with your Arduino as follows:

| LoRa Module | Arduino |
| :---------: | :-----: |
| VCC | GND |
| SCK | SCK (13) |
| MISO | MISO (12) |
| MOSI | MOSI (11) |
| NSS | 10 |
| RST | 9 |
| DIO0 | 2 |

`NSS`,  `RST` and `DIO0` pins can be changed if you don't have them available

After connecting your module correctly to the Arduino Pro Mini, you now can
compile and upload the code. Make sure you already have sandeepmistry's arduino-LoRa
library available on your Arduino IDE, if not you can refer to his README on how
to install the library.

## APRS i-Gate

You can also use this KISS TNC as an APRS i-Gate, you can connect this to
APRSIS-32 (on Windows) or you can use APRX for Linux. This can also work
with a Raspberry Pi 3.
