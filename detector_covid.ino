#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "Adafruit_CCS811.h"
#include <DHT.h>

#define dhtPin 6
#define dhtTipo DHT11

DHT tempSens(dhtPin, dhtTipo);

Adafruit_CCS811 Sensor;
LiquidCrystal_I2C Pant(0x27, 16, 2);

long TiempoDebug = 500;
long TiempoDebug1 = 0;
long Pitar1 = 0;
long Pitar2 = 0;
long Seg = 0;
long Tiempo = 0;
int Debug = 1000;
int CO2 = 0;
float t = 0;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  Serial.begin(9600);
  tempSens.begin();
  Sensor.begin();
  Pant.init();
  Pant.backlight();
  Pant.setCursor(0, 0);
  Pant.print("  CO2:    Temp: ");
}

void loop() {
  /*sólo conservamos el Debug del sensor de temperatura*/
  if ((millis() - TiempoDebug) > Debug) {
    tempSens.readTemperature();
    if (t != tempSens.readTemperature()) {
      Pant.setCursor(9, 1);
      t = tempSens.readTemperature();
      Pant.print(tempSens.readTemperature());
      Pant.print("C");
    }
    if (t < 14) {
      digitalWrite(10, LOW);
      digitalWrite(9, LOW);
      digitalWrite(7, HIGH);
    }
    else if ((t >= 14) && (t < 17)) {
      digitalWrite(10, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(7, LOW);
    }
    else if ((t >= 17) && (t <= 25)) {
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(7, LOW);
    }
    else if ((t > 25) && (t <= 28)) {
      digitalWrite(10, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(7, LOW);
    }
    else {
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(7, LOW);
    }
    TiempoDebug = millis();
  }
  /* He sustituido todos los while para evitar bloqueo de código*/
  if ((millis() - TiempoDebug1) > Debug) {
    Sensor.readData();
    Sensor.geteCO2();
    if (CO2 != Sensor.geteCO2()) {
      Pant.setCursor(0, 1);
      CO2 = Sensor.geteCO2();
      Pant.print(Sensor.geteCO2());
      Pant.print(" PPM");
    }
    if (CO2 <= 700) {
      Pitar1 = 0;
      Pitar2 = 0;
      digitalWrite(13, HIGH);
      digitalWrite(12, LOW);
      digitalWrite(11, LOW);
    }
    else if ((CO2 > 700) && (CO2 < 1500)) {
      Tiempo = millis();
      digitalWrite(13, LOW);
      digitalWrite(12, HIGH);
      digitalWrite(11, LOW);
      while (Pitar1 < 1) {
        if ((millis() - Tiempo) == 625) {
          tone(8, 4000);
        }
        if ((millis() - Tiempo) == 750) {
          noTone(8);
        }
        if ((millis() - Tiempo) == 875) {
          tone(8, 4000);
        }
        if ((millis() - Tiempo) == 1000) {
          noTone(8);
          Pitar1 += 1;
        }
      }
    }
    else {
      Tiempo = millis();
      digitalWrite(13, LOW);
      digitalWrite(12, LOW);
      digitalWrite(11, HIGH);
      while (Pitar2 < 2) {
        if ((millis() - Tiempo) == 625) {
          tone(8, 4000);
        }
        if ((millis() - Tiempo) == 750) {
          noTone(8);
        }
        if ((millis() - Tiempo) == 875) {
          tone(8, 4000);
        }
        if ((millis() - Tiempo) == 1000) {
          noTone(8);
          Tiempo = millis();
          Pitar2 += 1;
        }
      }
    }
    TiempoDebug1 = millis();
  }
}
