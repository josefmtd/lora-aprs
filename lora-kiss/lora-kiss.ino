#include <LoRa.h>
#include <SPI.h>

#define FEND    0xC0
#define FESC    0xDB
#define TFEND   0xDC
#define TFESC   0xDD

#define maxPacketSize 255

#define kissNoCrc   0x00
#define kissFlexnet 0x20
#define kissSmack   0x80

void putSerial(const uint8_t *const txBuffer, const uint16_t txBufferSize) {
  Serial.write(txBuffer, txBufferSize);
}

void putRadio(const uint8_t *const txBuffer, const uint16_t txBufferSize) {
  LoRa.beginPacket();
  LoRa.write(txBuffer, txBufferSize);
  LoRa.endPacket();
}

void putByte(uint8_t *const out, uint16_t *const offset, const uint8_t buffer) {
  if (buffer == FEND) {
    out[(*offset)++] = FESC;
    out[(*offset)++] = TFEND;
  }

  else if (buffer == FESC) {
    out[(*offset)++] = FESC;
    out[(*offset)++] = TFESC;
  }

  else {
    out[(*offset)++] = buffer;
  }
}

void debug(const char *const debug_string) {
  uint8_t myBuffer[128];
  const uint8_t ax25_ident[] = { 0x92, 0x88, 0x8A, 0x9C, 0xA8, 0x40, 0xE0, 0x88,
  0x8A, 0x84, 0xAA, 0x8E, 0x60, 0x63, 0x03, 0xF0};

  uint16_t offset = 0;
  myBuffer[offset++] = 0x00;

  // Adding AX25 Address to the Buffer
  for (uint8_t i = 0; i < sizeof(ax25_ident); i++) {
    myBuffer[offset++] = ax25_ident[i];
  }

  uint8_t length = strlen(debug_string);

  for (uint8_t i = 0; i < length; i++) {
    myBuffer[offset++] = debug_string[i];
  }

  const uint8_t fend = FEND;
  putSerial(&fend, 1);

  for (uint8_t i = 0; i < offset; i++) {
    uint8_t tinyBuffer[4];
    uint16_t offset1 = 0;
    putByte(tinyBuffer, &offset1, myBuffer[i]);
    putSerial(tinyBuffer, offset1);
  }

  putSerial(&fend, 1);
}

//void kissEscapeBuffer(uint8_t *const out, uint16_t *const offset, const uint8_t charBuffer) {
//  if (charBuffer == FEND) {
//    out[(*offset)++] = FESC;
//    out[(*offset)++] = TFEND;
//  }
//  else if (charBuffer == FESC) {
//    out[(*offset)++] = FESC;
//    out[(*offset)++] = TFESC;
//  }
//  else {
//    out[(*offset)++] = charBuffer;
//  }
//}
//
//void debug(const char *const debugString) {
//  uint8_t tempBuffer[128];
//  const uint8_t ax25Ident[] = { 0x92, 0x88, 0x8A, 0x9C, 0xA8, 0x40, 0xE0, 0x88,
//                                0x8A, 0x84, 0xAA, 0x8E, 0x60, 0x63, 0x03, 0xF0  };
//  uint16_t offset = 0;
//  tempBuffer[offset++] = 0x00;
//
//  // Adding AX25 Address to the Buffer
//  for (uint8_t i = 0; i < sizeof(ax25Ident); i++) {
//    tempBuffer[offset++] = ax25Ident[i];
//  }
//
//  uint8_t debugStringLen = strlen(debugString);
//
//  for (uint8_t i = 0; debugStringLen; i++) {
//    tempBuffer[offset++] = debugString[i];
//  }
//
//  const uint8_t fend = FEND;
//  putSerial(&fend, 1);
//
//  for (uint8_t i = 0; i < offset; i++) {
//    uint8_t tinyBuffer[4];
//    uint16_t offset1 = 0;
//    kissEscapeBuffer(tinyBuffer, &offset1, tempBuffer[i]);
//    putSerial(tinyBuffer, offset1);
//  }
//
//  putSerial(&fend, 1);
//}

bool getSerial(uint8_t *const rxBuffer, const uint16_t rxBufferSize, const unsigned long int timeOut) {
  for (uint16_t i = 0; i < rxBufferSize; i++) {
    while(!Serial.available()) {
      if (millis() >= timeOut) {
        return false;
      }
    }
    rxBuffer[i] = Serial.read();
  }
  return true;
}

void processSerial() {
  bool first = true;
  bool frameValid = false;
  bool escape = false;
  uint8_t bufferSmall[maxPacketSize];
  
  uint16_t offset = 0;

  const unsigned long int end = millis() + 5000;

  while(millis() < end && frameValid == false && offset < maxPacketSize) {
    uint8_t buffer = 0;

    if (!getSerial(&buffer, 1, end)) {
      debug("SERIAL TIMEOUT");
      break;
    }

    switch(buffer) {
      case FEND:
        if (first) {
          first = false;
        }
        else {
          frameValid = true;
        }
        break;
      case FESC:
        escape = true;
      default:
        if (escape) {
          if (buffer == TFEND) {
            bufferSmall[offset++] = FEND;
          }
          else if (buffer == TFESC) {
            bufferSmall[offset++] = FESC;
          }
          else {
            debug("ERROR ESCAPE");
          }

          escape = false;
        }
        else {
          bufferSmall[offset++] = buffer;
        }
    }
  }

  if (frameValid) {
    if (offset > 1) {
      switch(bufferSmall[0]) {
        case kissNoCrc:
          // debug("Should transmit");
          putRadio(&bufferSmall[0], offset);
          break;
        case kissFlexnet:
          debug("CANNOT ACCEPT FLEXNET");
          break;
        case kissSmack:
          debug("CANNOT ACCEPT SMACK");
        default:
          debug("FRAME UNKNOWN");
      }
    }
    else {
      debug("TOO SMALL FOR A KISS FRAME");
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  while (!Serial);
  //for LoRa32u4
  LoRa.setPins(8, 4, 7);
  if (!LoRa.begin(434E6)) {
    debug("LoRa Init Failed");
  }
  else {
    LoRa.enableCrc();
    LoRa.setCodingRate4(9);
    LoRa.onReceive(onReceive);
    LoRa.receive();
    debug("LoRa Init Success");
  }
}

void loop() {
  // do nothing
  uint16_t serialIn = Serial.available();
  if (serialIn != 0) {
    // debug("IT'S SOMETHING");
    processSerial();
  }
}

void onReceive(int PacketSize) {
  uint8_t bufferSmall[maxPacketSize];
  uint8_t bufferBig[maxPacketSize * 2];
  uint16_t offsetSmall = 0;
  uint16_t offsetBig = 0;

  while(LoRa.available()) {
    bufferSmall[offsetSmall++]=(char)LoRa.read();
  }

  bufferBig[offsetBig++] = FEND;
  for (uint16_t i = 0; i < offsetSmall; i++) {
    putByte(bufferBig, &offsetBig, bufferSmall[i]);
  }
  bufferBig[offsetBig++] = FEND;

  putSerial(bufferBig, offsetBig);
}
