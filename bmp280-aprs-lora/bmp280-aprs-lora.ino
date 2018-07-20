#include <Wire.h>
#include <SPI.h>
#include <RH_RF95.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BMP280.h"

// Singleton instance of the radio driver
Adafruit_BMP280 bmp; // I2C
RH_RF95 rf95(8, 7);
uint8_t headerAprs[17] = {0x00, 0x82, 0xA0, 0x98, 0xA4, 0xAE, 0xB0, 0xE0, 
                          0xB2, 0x88, 0x60, 0xA6, 0x90, 0xB2, 0x6B, 0x03, 
                          0xF0};
//uint8_t headerAprs[17] = {0x00, 0x9C, 0x9E, 0x8E, 0x82, 0xA8, 0x8A, 0xE0, 
//                          0xB2, 0x88, 0x60, 0xA6, 0x90, 0xB2, 0x6B, 0x03, 
//                          0xF0};
uint8_t posAprs[19]    = {0x21, 0x30, 0x36, 0x31, 0x31, 0x2E, 0x37, 0x38,
                          0x53, 0x2F, 0x31, 0x30, 0x36, 0x34, 0x37, 0x2E,
                          0x32, 0x30, 0x45};
uint8_t headerCwop[14] = {0x5F, 0x2E, 0x2E, 0x2E, 0x2F, 0x2E, 0x2E, 0x2E, 
                          0x67, 0x2E, 0x2E, 0x2E, 0x74, 0x30};
//uint8_t rain[1]        = {0x62};
uint8_t rain[16]       = {0x72, 0x2E, 0x2E, 0x2E, 0x70, 0x2E, 0x2E, 0x2E, 
                          0x50, 0x2E, 0x2E, 0x2E, 0x68, 0x2E, 0x2E, 0x62};
uint8_t comment[7]     = {0x4C, 0x6F, 0x52, 0x61, 0x20, 0x57, 0x58};

void sendRadio(uint8_t * temp, uint16_t tempSize, uint8_t * pres, uint16_t presSize) {
  uint8_t myBuffer[256];
  uint16_t offset = 0;
  for (uint16_t i = 0; i < sizeof(headerAprs); i++) {
    myBuffer[offset++] = headerAprs[i];
  }

  for (uint16_t j = 0; j < sizeof(posAprs); j++) {
    myBuffer[offset++] = posAprs[j];
  }
  
  for (uint16_t k = 0; k < sizeof(headerCwop); k++) {
    myBuffer[offset++] = headerCwop[k];
  }
  
  
  for (uint16_t l = 0; l < tempSize; l++) {
    myBuffer[offset++] = temp[l];
  }
  
  for (uint16_t m = 0; m < sizeof(rain); m++) {
    myBuffer[offset++] = rain[m];
  }
  
  for (uint16_t n = 0; n < presSize; n++) {
    myBuffer[offset++] = pres[n];
  }

  for (uint16_t o = 0; o < sizeof(comment); o++) {
    myBuffer[offset++] = comment[o];
  }
  //Serial.println(offset);
  rf95.send(myBuffer, offset);
  rf95.waitPacketSent();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  if (!rf95.init())
    Serial.println("lora init failed");
  else
    Serial.println("lora init success");
  if (!bmp.begin())
    Serial.println("bmp280 init failed");
  else
    Serial.println("bmp280 init success");
  delay(10000);
}
void loop() {
  // put your main code here, to run repeatedly:
  char charTempAprs[3];
  char charPresAprs[6];
  uint16_t sizeTempAprs = sizeof(charTempAprs);
  uint16_t sizePresAprs = sizeof(charPresAprs);
  float tempCelsius = bmp.readTemperature();
  float tempFahrenheit = tempCelsius*9/5+32;
  int tempAprs = (int)tempFahrenheit;
  Serial.println(tempAprs);
  float presPascal = bmp.readPressure();
  float presHectoPascal = presPascal/100;
  int presAprs = (int)presHectoPascal;
  Serial.println(presAprs);
  String stringPresAprs = String(presAprs);
  String stringTempAprs = String(tempAprs);
  stringTempAprs.toCharArray(charTempAprs, sizeTempAprs);
  stringPresAprs.toCharArray(charPresAprs, sizePresAprs);
  sendRadio(charTempAprs, sizeTempAprs-1, charPresAprs, sizePresAprs-1);
  delay(602000);
}


