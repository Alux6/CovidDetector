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
bool T1 = true;
bool T2 = true;
bool b1 = true;
bool b2 = true;
int Debug = 1000;
long Tiempo2 = 60000;
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
  if ((millis() - Seg) > Tiempo2) {
    Pitar1 = 0;
    Pitar2 = 0;
    b1 = true;
    b2 = true;
  }
  /* Cada segundo el Sensor de temperatura y el sensor de CO2 realizan una lectura, con 0.5 segundos de diferencia entre la lectura
     de uno con respecto al otro. */
  if ((millis() - TiempoDebug) > Debug) {
    tempSens.readTemperature();
    if (t != tempSens.readTemperature()) {
      Pant.setCursor(9, 1);
      t = tempSens.readTemperature();
      Pant.print(tempSens.readTemperature());
      Pant.print("C");
    }
    /* Los leds se actualizan en función de si la temperatura es apropiada para trabajar o no. */
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
  /* Todos los procesos que involucran tiempo, excepto los pitidos, están hechos sin bucles para que no se pare
    el programa en ningún momento. */
  if ((millis() - TiempoDebug1) > Debug) {
    Sensor.readData();
    Sensor.geteCO2();
    /* Los leds se actualizan en función de si la concentración de partes de CO2 por millón son apropiadas
      para estar en una habitación, adicionalmente cuando el nivel de peligro es moderado, suena una secuencia
      de dos pitidos. */
    if (CO2 != Sensor.geteCO2()) {
      Pant.setCursor(0, 1);
      CO2 = Sensor.geteCO2();
      Pant.print(Sensor.geteCO2());
      Pant.print(" PPM ");
    }
    if (CO2 <= 700) {
      Pitar1 = 0;
      Pitar2 = 0;
      digitalWrite(13, HIGH);
      digitalWrite(12, LOW);
      digitalWrite(11, LOW);
    }
    else if ((CO2 > 700) && (CO2 < 1500)) {
      if (b1) {
        Seg = millis();
        b1 = !b1;
      }
      digitalWrite(13, LOW);
      digitalWrite(12, HIGH);
      digitalWrite(11, LOW);

      while (Pitar1 == 0) {
        if (T1) {
          Tiempo = millis();
          T1 = false;
        }
        if (((millis() - Tiempo) >= 200) && ((millis() - Tiempo) < 400)) {
          tone(8, 4000);
        }
        else if (((millis() - Tiempo) >= 400) && ((millis() - Tiempo) < 600)) {
          noTone(8);
        }
        else if (((millis() - Tiempo) >= 600) && ((millis() - Tiempo) < 800)) {
          tone(8, 4000);
        }
        else if ((millis() - Tiempo) >= 800) {
          noTone(8);
          Pitar1 = 1;
          T1 = true;
        }
        else {

        }
      }
    }
    else {
      if (b2) {
        Seg = millis();
        b2 = !b2;
      }
      digitalWrite(13, LOW);
      digitalWrite(12, LOW);
      digitalWrite(11, HIGH);
      Pitar1 = 1;
      while (Pitar2 == 0) {
        if (T2) {
          Tiempo = millis();
          T2 = false;
        }
        if (((millis() - Tiempo) >= 200) && ((millis() - Tiempo) < 400)) {
          tone(8, 4000);
        }
        else if (((millis() - Tiempo) >= 400) && ((millis() - Tiempo) < 600)) {
          noTone(8);
        }
        else if (((millis() - Tiempo) >= 600) && ((millis() - Tiempo) < 800)) {
          tone(8, 4000);
        }
        else if (((millis() - Tiempo) >= 800) && ((millis() - Tiempo) < 1000)) {
          noTone(8);
        }
        else if (((millis() - Tiempo) >= 1000) && ((millis() - Tiempo) < 1200)) {
          tone(8, 4000);
        }
        else if (((millis() - Tiempo) >= 1200) && ((millis() - Tiempo) < 1400)) {
          noTone(8);
        }
        else if (((millis() - Tiempo) >= 1400) && ((millis() - Tiempo) < 1600)) {
          tone(8, 4000);
        }
        else if ((millis() - Tiempo) >= 1600) {
          noTone(8);
          Pitar2 = 1;
          T2 = true;
        }
      }
    }
    TiempoDebug1 = millis();
  }
}
