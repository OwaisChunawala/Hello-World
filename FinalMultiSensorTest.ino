#include <OneWire.h>
//p
#include<LiquidCrystal.h>// header file for lcd 
#include<Wire.h>
#include"RTClib.h"
#include<SD.h>
#include<SPI.h>
//~p
#define ANALOG_SENSOR_PIN  A0 //soil Moisture sensor
#define DIGITAL_SENSOR_PIN 3//light sensor
#define LEDPIN    13  // The onboard LED
//p
LiquidCrystal lcd(9,8,7,6,5,10);//register select pin,enable pin,D4 pin,D5 pin,D6 pin,D7 pin
RTC_DS1307 rtc;// Arduino analog pin 4 = SDA and analog pin 5 = SCL these are dedicated pins 
File Anukul_Data_Storage;
String Mega_Data_String;
//~p
int  switch_state;  /* Holds the last digital value */
int  LightAnalogValue; /* Holds the last analog value */
OneWire  ds(2); 
void setup() {
 
//p
  while(!Serial);
  pinMode(LEDPIN, OUTPUT);
  Serial.begin(9600);// put your setup code here, to run once:
  if(!rtc.begin()){
    Serial.println("Couldn't find RTC");
    while(1);
  }
   //o
  //adjusting time
  //rtc.adjust(DateTime(2017, 3, 12, 16, 47, 0));
  //o
  if(!rtc.isrunning()){
    Serial.println("RTC is not running!");
       // following line sets the RTC to the date & time this sketch was compiled
     //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
     rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  Mega_Data_String = String();
  lcd.begin(16,2);//this declares the size of the lcd monitor
  if(SD.begin(4)){//SD card initislisation
    Serial.println("SD Card initialisation Success");
  }
  else {
    Serial.println("SD Card initialisation Failed");
    return;
  }
//~p
}

void loop() {
   byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;

  if ( !ds.search(addr)) {
    //Serial.println("No more addresses.");
    //Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }

 /* Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }*/

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  //Serial.println();

  /*// the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;
  }*/

  ds.reset();
  ds.select(addr);
  ds.write(0x44);        // start conversion, use ds.write(0x44,1) with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  //Serial.print("  Data = ");
  //Serial.print(present, HEX);
  //Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    //Serial.print(data[i], HEX);
    //Serial.print(" ");
  }
  //Serial.print(" CRC=");
  //Serial.print(OneWire::crc8(data, 8), HEX);
  //Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
    
  }
  
  ///////*************************Above this is not related
  
  switch_state = digitalRead(DIGITAL_SENSOR_PIN);  // photosensor part
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  //p Prints the date and time on the serial monitor
  DateTime now = rtc.now();
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
  int sensorValue = analogRead(A0);
 /* Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.print(" Fahrenheit, ");// put your main code here, to run repeatedly:
  //following is for soil moisture level
  Serial.print("soil moisture value = ");
  Serial.print(sensorValue);//soil Moisture value is stored in sensorValue*/
  Mega_Data_String = ", Temperature_celcius = " + String(celsius) + ", Temperature_fahrenheit = " + fahrenheit + ", Soil Moisture Value = " + sensorValue;
  Serial.print(Mega_Data_String);
  lcd.print("SM Value = ");
  lcd.setCursor(12,0);//set cursor
  lcd.print(sensorValue);
  lcd.setCursor(0,1);
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
  lcd.print(":");
  lcd.print(now.second(), DEC);
  Anukul_Data_Storage = SD.open("Anukul_Data_Storage.txt",FILE_WRITE);//Create/Open file
  if(Anukul_Data_Storage){// storing the data on the SD card
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
    Anukul_Data_Storage.print(Mega_Data_String);
    Anukul_Data_Storage.close();
  }
  else {
    Serial.print(", Error File could not be opened, ");
  }
  //~p
  // following is for photosensor. I had an digital sensor therefore I used this "on" and "off"
 if (switch_state == LOW)
  {
    digitalWrite(LEDPIN, HIGH);
    Serial.println(" Digital Signal ON ");
  }  
  else
  {
    digitalWrite(LEDPIN, LOW);
    Serial.println(" Digital Signal OFF");
}}

