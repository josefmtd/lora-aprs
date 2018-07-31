#ifndef CONFIG_H
  #define CONFIG_H

  #define MAJ_VERS 0x01
  #define MIN_VERS 0x00

  #define MCU_328P 0x90
  #define MCU_32U4 0x91

  #if defined(__AVR_ATmega328P__)
    #define MCU_VARIANT MCU_328P
    #warning "Firmware is being compiled for ATmega328p based boards"
  #elif defined(__AVR_ATmega32U4__)
    #define MCU_VARIANT MCU_32U4
    #warning "Firmware is being compiled for ATmega32u4 based boards"
  #else
    #error "The firmware cannot be compiled for the selected MCU variant"
  #endif
  
  #define MTU  255
  #define CMD_L       4
  int     lastRssi = -292;
  uint8_t lastRssiRaw = 0x00;
  size_t  readLength = 0;

  #if MCU_VARIANT == MCU_328P
    const int pinNSS = 10;
    const int pinNRST = 9;
    const int pinDIO0 = 2;
    // const int pinLedRx = 5;
    // const int pinLedTx = 4;

  #endif

  #if MCU_VARIANT == MCU_32U4
    const int pinNSS  = 8;
    const int pinNRST = 4;
    const int pinDIO0 = 7;
  #endif

  const long serialBaudRate   = 38400;
  const int  rssiOffset      = 292;

  const int  loraRxTurnaround = 50;

  // Default LoRa settings
  int       loraSpreadingFactor = 9;
  int       loraCodingRate      = 5;
  int       loraTxPower         = 17;
  uint32_t  loraBandwidth       = 125E6;
  uint32_t  loraFrequency       = 434E6;

  uint8_t txBuffer[MTU];
  uint8_t rxBuffer[MTU];

  uint32_t statRx = 0;
  uint32_t statTx = 0;

  bool outboundReady = false;

  bool statSignalDetected = false;
  bool statSignalSynced   = false;
  bool statRxOngoing      = false;
  bool dcd                = false;
  bool dcdLed             = false;
  bool dcdWaiting         = false;
  uint16_t dcdCount       = 0;
  uint16_t dcdThreshold   = 15;

  uint32_t statusIntervalms = 3;
  uint32_t lastStatusUpdate = 0;

  // Status flags
  const uint8_t SIG_DETECT = 0x01;
  const uint8_t SIG_SYNCED = 0x02;
  const uint8_t RX_ONGOING = 0x04;

#endif
