#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

unsigned long delayTime = 602000;

uint8_t headerAprs[17]  = {0x00, 0x82, 0xA0, 0x98, 0xA4, 0xAE, 0xB0, 0xE0,
                           0xB2, 0x88, 0x60, 0xA6, 0x90, 0xB2, 0x6B, 0x03,
                           0xF0};
//uint8_t headerAprs[17] = {0x00, 0x9C, 0x9E, 0x8E, 0x82, 0xA8, 0x8A, 0xE0, 
//                          0xB2, 0x88, 0x60, 0xA6, 0x90, 0xB2, 0x6B, 0x03, 
//                          0xF0};
//uint8_t posAprs[19]     = {0x21, 0x30, 0x36, 0x32, 0x31, 0x2E, 0x35, 0x35,
//                           0x53, 0x2F, 0x31, 0x30, 0x36, 0x34, 0x30, 0x2E,
//                           0x34, 0x30, 0x45};
uint8_t posAprs[19]     = {0x21, 0x30, 0x36, 0x31, 0x31, 0x2E, 0x37, 0x36,
                           0x53, 0x2F, 0x31, 0x30, 0x36, 0x34, 0x37, 0x2E,
                           0x32, 0x30, 0x45};
uint8_t headerCwop[14]  = {0x5F, 0x2E, 0x2E, 0x2E, 0x2F, 0x2E, 0x2E, 0x2E, 
                           0x67, 0x2E, 0x2E, 0x2E, 0x74, 0x30};
uint8_t rain[13]        = {0x72, 0x2E, 0x2E, 0x2E, 0x70, 0x2E, 0x2E, 0x2E, 
                           0x50, 0x2E, 0x2E, 0x2E, 0x68};
uint8_t barometric      = 0x62;
uint8_t comment[7]      = {0x4C, 0x6F, 0x52, 0x61, 0x20, 0x57, 0x58};

void sendRadio(uint8_t * temp, uint16_t tempSize, uint8_t * humid, uint16_t humidSize, uint8_t * pres, uint16_t presSize) {
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
  
  for (uint16_t n = 0; n < humidSize; n++) {
    myBuffer[offset++] = humid[n];
  }

  myBuffer[offset++] = barometric;

  for (uint16_t o = 0; o < presSize; o++) {
    myBuffer[offset++] = pres[o];
  }

  for (uint16_t p = 0; p < sizeof(comment); p++) {
    myBuffer[offset++] = comment[p];
  }
  Serial.write(myBuffer,offset);
  Serial.println();
  LoRa.beginPacket();
  LoRa.write(myBuffer, offset);
  LoRa.endPacket();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  Serial.println(F("BME280 test"));

  bool status;

  LoRa.setPins(8, 4, 7);
  
  if(!LoRa.begin(434E6)) {
    Serial.println("LoRa Init Failed");
    while(1);
  }
  if(!bme.begin())
    Serial.println("Could not find a valid BME280 sensor");

  Serial.println("-- Default Test --");
}

void loop() {
  //put your main code here, to run repeatedly:
  //printValues();
  char charTempAprs[3];
  char charPresAprs[6];
  char charHumidAprs[3];
  uint16_t sizeTempAprs = sizeof(charTempAprs);
  uint16_t sizePresAprs = sizeof(charPresAprs);
  uint16_t sizeHumidAprs = sizeof(charHumidAprs);
  float tempCelsius = bme.readTemperature();
  float tempFahrenheit = tempCelsius*9/5+32;
  int tempAprs = (int)tempFahrenheit;
  Serial.println(tempAprs);
  float humidPercentage = bme.readHumidity();
  int humidAprs = (int)humidPercentage;
  Serial.println(humidAprs);
  float presPascal = bme.readPressure();
  float presHectoPascal = presPascal/10;
  int presAprs = (int)presHectoPascal;
  Serial.println(presAprs);
  String stringPresAprs = String(presAprs);
  String stringTempAprs = String(tempAprs);
  String stringHumidAprs = String(humidAprs);
  stringTempAprs.toCharArray(charTempAprs, sizeTempAprs);
  stringPresAprs.toCharArray(charPresAprs, sizePresAprs);
  stringHumidAprs.toCharArray(charHumidAprs, sizeHumidAprs);
  sendRadio(charTempAprs, sizeTempAprs-1, charHumidAprs, sizeHumidAprs-1, charPresAprs, sizePresAprs-1);  
  delay(delayTime);
}

//void printValues() {
//  Serial.print("Temperature = ");
//  Serial.print(bme.readTemperature());
//  Serial.println(" *C");
//
//  Serial.print("Pressure = ");
//  Serial.print(bme.readPressure() / 100.0F);
//  Serial.println(" hPa");
//
//  Serial.print("Humidity = ");
//  Serial.print(bme.readHumidity());
//  Serial.println(" %");
//
//  Serial.println();
//}

