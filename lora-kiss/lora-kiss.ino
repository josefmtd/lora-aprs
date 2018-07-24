#include <LoRa.h>
#include <SPI.h>

#define FEND    0xC0
#define FESC    0xDB
#define TFEND   0xDC
#define TFESC   0xDD

#define maxPacketSize 255

void putSerial(const uint8_t *const send_buffer, const uint16_t send_buffer_size) {
  Serial.write(send_buffer, send_buffer_size);
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  while (!Serial);
  
  if (!LoRa.begin(434E6)) {
    debug("LoRa Init Failed");
  }
  else {
    LoRa.onReceive(onReceive);
    LoRa.receive();
    debug("LoRa Init Success");
  }
}

void loop() {
  // do nothing
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

