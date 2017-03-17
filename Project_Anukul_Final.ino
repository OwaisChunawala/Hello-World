#include <OneWire.h>
#include<LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include<Wire.h>
#include"RTClib.h"


#define ANALOG_SENSOR_PIN  A1// Light Sensor
OneWire  ds(2); // Temperature
const int chipSelect = 4;//SD card chipselect
int  LightAnalogValue;
RTC_DS1307 rtc;
LiquidCrystal lcd(9,8,7,6,5,10);//LCD connections



void setup() {
  
  ///***Open serial communications and wait for port to open:***///
  Serial.begin(9600);
  while (!Serial) {;}
  
  ///***Rtc initialisation code***///
  if(!rtc.begin()){
    Serial.println("Couldn't find RTC");
    while(1);
  }
   if(!rtc.isrunning()){
    Serial.println("RTC is not running!");
     rtc.adjust(DateTime(2017, 3, 18, 2, 25, 0));
  }

  ///***Lcd begin***///
  lcd.begin(16,2);
  
  ///***see if the SD card is present and can be initialized:***///
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}



void loop() {
  
  ///***Temperature sensor code***///
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);            // DELAY TEMP
    return;
  }
   if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44);   // start conversion
  delay(1000);     // Delay
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);  // Read Scratchpad
  for ( i = 0; i < 9; i++) {    //9 bytes
    data[i] = ds.read();
  }
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    //at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  //9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; //10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;

  ///***Variables***///
  DateTime now = rtc.now();

  ///***Lightsensor and Soilmoisture Sensor***///
  LightAnalogValue = analogRead(ANALOG_SENSOR_PIN);
  int sensorValue = analogRead(A0);
  
  ///***Code to Print on the LCD monitor***///
  lcd.print("SM Value = ");
  lcd.setCursor(12,0);//set cursor
  lcd.print(String(sensorValue));
  lcd.setCursor(0,1);
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
  lcd.print(":");
  lcd.print(now.second(), DEC);

  ///***SD card Storage***/// 
  File Anukul_Data_Storage = SD.open("datalog.txt", FILE_WRITE);
  if (Anukul_Data_Storage) {
    Anukul_Data_Storage.print(now.day(), DEC);
    Anukul_Data_Storage.print("/");
    Anukul_Data_Storage.print(now.month(), DEC);
    Anukul_Data_Storage.print("/");
    Anukul_Data_Storage.print(now.year(), DEC);
    Anukul_Data_Storage.print(", ");
    Anukul_Data_Storage.print(now.hour(), DEC); 
    Anukul_Data_Storage.print(":");
    Anukul_Data_Storage.print(now.minute(), DEC);
    Anukul_Data_Storage.print(":");
    Anukul_Data_Storage.print(now.second(), DEC);
    Anukul_Data_Storage.print(", Soil Moisture Value = ");
    Anukul_Data_Storage.print(String(sensorValue));
    Anukul_Data_Storage.print(", Temperature_celsius = ");
    Anukul_Data_Storage.print(String(celsius));
    Anukul_Data_Storage.print(", Temperature_fahrenheit = ");
    Anukul_Data_Storage.print(String(fahrenheit));
    Anukul_Data_Storage.print(", Light Sensor Value = ");
    Anukul_Data_Storage.println(String(LightAnalogValue));
    Anukul_Data_Storage.close();

    ///***Printing the Data on the Serial Monitor***///
    Serial.print(now.day(), DEC);
    Serial.print("/");
    Serial.print(now.month(), DEC);
    Serial.print("/");
    Serial.print(now.year(), DEC);
    Serial.print(", ");
    Serial.print(now.hour(), DEC);
    Serial.print(":");
    Serial.print(now.minute(), DEC);
    Serial.print(":");
    Serial.print(now.second(), DEC);
    Serial.print(", Soil Moisture Value = ");
    Serial.print(String(sensorValue));
    Serial.print(", Temperature_celsius = ");
    Serial.print(String(celsius));
    Serial.print(", Temperature_fahrenheit = ");
    Serial.print(String(fahrenheit));
    Serial.print(", Light Sensor Value = ");
    Serial.println(String(LightAnalogValue));
  }
  else {
    Serial.println("error opening datalog.txt");
  }
}









