#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "Adafruit_CCS811.h"
#include <DHT.h>

#define dhtPin 6
#define dhtTipo DHT11

DHT tempSens(dhtPin, dhtTipo);

Adafruit_CCS811 Sensor;
LiquidCrystal_I2C Pant(0x27,16,2);
long TiempoDebug = 0;
long TiempoDebug1 = 0;
long Tiempo = 0;
long Tiempo1 = 0;
int Debug = 200;
int Tmp = 5000;
int CO2 = 1000;
void setup() {

  // put your setup code here, to run once:
pinMode(11, OUTPUT);
pinMode(10, OUTPUT);
pinMode(9, OUTPUT);
Serial.begin(9600);
tempSens.begin();
Sensor.begin();
Pant.init();
Pant.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
float t = tempSens.readTemperature();
if(((millis() - Tiempo) < Tmp)){
  Pant.clear();
  Pant.setCursor(0,0);
  Pant.print("Tiempo transcur.");
  while (((millis() - Tiempo) < Tmp)){
    if((millis() - TiempoDebug) > Debug){
      Pant.setCursor(0,1);
      Pant.print((millis()/1000)+1);
      Pant.print(" Segundos");
      TiempoDebug = millis();
    }
  }
}
if((millis()-TiempoDebug1)> Debug){
 t = tempSens.readTemperature();
 Serial.println(t);
 if(t <= 20){
  digitalWrite(10, 0);
  digitalWrite(11, 255);
}
else if((t > 20)&&(t<26)){
  digitalWrite(10, 255);
  digitalWrite(11, 255);
}
else{
  digitalWrite(10, 255);
  digitalWrite(11, 0);  
}
  TiempoDebug1 = millis();
}
if(((millis() - Tiempo) > Tmp)&&((millis() - Tiempo) < (2*Tmp))){
    Pant.clear();
    Pant.setCursor(0,0);
    Pant.print("Calidad Aire");
    while (((millis() - Tiempo) < (2* Tmp))){
      if((millis() - TiempoDebug) > Debug){
        Sensor.readData();
        Pant.setCursor(0,1);
        Pant.print(Sensor.geteCO2());
        Pant.print(" PPM");
        TiempoDebug = millis();
      }
  }
  Tiempo = millis();
}
}
