/*
 * Created by Pi BOTS MakerHub
 *
 * Email: pibotsmakerhub@gmail.com
 * 
 * Github: https://github.com/pibotsmakerhub
 *
 * Join Us on Telegram : https://t.me/pibots 
 * Copyright (c) 2020 Pi BOTS MakerHub
*/

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor

//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
#include <LiquidCrystal.h>
#include<stdio.h>
#include<stdbool.h>

#include "DHT.h"
#define DHTPIN A0    
#define DHTTYPE DHT11   

DHT dht(DHTPIN, DHTTYPE);

int buzzer = 10;
int greenpin=25;
int redpin=26;
int waterlevel;
int waterpin = A3; // sensor pin to used
int temppin=A10;
int fan=30;
int waterlevelup;
int waterleveldown;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(fan, OUTPUT);
  lcd.begin(16, 2);
  digitalWrite(fan,LOW);
  digitalWrite(redpin, LOW);
  digitalWrite(greenpin, LOW);
  digitalWrite(buzzer,LOW);
}

int ReadWaterLevel(){
  waterlevel = analogRead(waterpin);
  return waterlevel;
}

bool iswaterlevelup(){
  if(ReadWaterLevel() >= 100){
    return true;
  }else{
    return false;
  }
}

void loop() {
  // Wait a few seconds between measurements.
  waterleveldown = ReadWaterLevel() < 100;
  waterlevelup =ReadWaterLevel() >= 100;
  
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Water level: ");
  Serial.println(ReadWaterLevel());
  Serial.print("Temperature: ");
  Serial.print(t); Serial.println(" °C");
  delay(2000);

  if(iswaterlevelup() || !iswaterlevelup()){

    if(iswaterlevelup()){
      lcd.setCursor(0, 0); 
      lcd.println("ABC flower farm");
      lcd.setCursor(0, 1); 
      lcd.print("System");
    }else{
      lcd.setCursor(0, 0); 
      lcd.println("Irrigate        ");
      lcd.setCursor(0, 1); 
      lcd.print("     ");
    }
    
    if(t<19){
     lcd.setCursor(0, 1); 
     lcd.print("Turn-Heater ON");
     digitalWrite(fan, LOW);
     digitalWrite(buzzer,HIGH);
     digitalWrite(redpin, HIGH);
     digitalWrite(greenpin, LOW);
    }
    if(t>30){
       lcd.setCursor(0, 1); 
       lcd.print("Turn-Fan ON");
       digitalWrite(fan, HIGH);
       digitalWrite(buzzer,HIGH);
       digitalWrite(redpin, HIGH);
       digitalWrite(greenpin, LOW);
    }
    if(t >= 19 && t <= 30){
       lcd.setCursor(0, 1); 
       lcd.print("Normal temp..");
      digitalWrite(fan, LOW); 
      digitalWrite(redpin, LOW);
      digitalWrite(greenpin, HIGH);
      digitalWrite(buzzer,LOW);
    }
  }
  
}
