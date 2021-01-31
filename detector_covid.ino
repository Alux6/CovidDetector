#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "Adafruit_CCS811.h"
#include <DHT.h>

#define dhtPin 6
#define dhtTipo DHT11

DHT tempSens(dhtPin, dhtTipo);

Adafruit_CCS811 Sensor;
LiquidCrystal_I2C Pant(0x27, 16, 2);

long TiempoDebug = 0;
long Seg = 0;
long Tiempo = 0;
int Debug = 500;
int Tmp = 5000;
int CO2 = 0;
int Part1 = 1;
int Part2 = 0;
float t = tempSens.readTemperature();

void setup() {
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  Serial.begin(9600);
  tempSens.begin();
  Sensor.begin();
  Pant.init();
  Pant.backlight();
}

void loop() {
/*El tiempo y la concentración de CO2 sólo se actualiza cuando hay algún cambio, así ahorramos energía a la pila y tiempo al programa, 
y evitamos el efecto de actualización continua de la pantalla que queda feísimo */
  if ((millis() - Tiempo) <= Tmp) {
    if (Part1 == 1) {
      Pant.clear();
      Pant.setCursor(0, 0);
      Pant.print("Tiempo transcur.");
      Part1 = 0;
      Part2 = 1;
    }
    if ((millis() / 1000) != Seg) {
      Pant.setCursor(0, 1);
      Pant.print((millis() / 1000) + 1);
      Seg = millis() / 1000;
      Pant.print(" Segundos");
    }
  }
/*sólo conservamos el Debug del sensor de temperatura*/
  if ((millis() - TiempoDebug) > Debug) {
    t = tempSens.readTemperature();
    Serial.println(t);
    if (t <= 20) {
      digitalWrite(10, 0);
      digitalWrite(11, 255);
    }
    else if ((t > 20) && (t < 26)) {
      digitalWrite(10, 255);
      digitalWrite(11, 255);
    }
    else {
      digitalWrite(10, 255);
      digitalWrite(11, 0);
    }
    TiempoDebug = millis();
  }
/* He sustituido todos los while para evitar bloqueo de código*/
  if (((millis() - Tiempo) > Tmp) && ((millis() - Tiempo) < (2 * Tmp))) {
    Sensor.readData();
    Sensor.geteCO2();
    if (Part2 == 1) {
      Pant.clear();
      Pant.setCursor(0, 0);
      Pant.print("Calidad Aire");
      Part1 = 1;
      Part2 = 0;
    }
    if (CO2 != Sensor.geteCO2()) {
      Pant.setCursor(0, 1);
      CO2 = Sensor.geteCO2();
      Pant.print(Sensor.geteCO2());
      Pant.print(" PPM");
    }
  }
  else if ((millis() - Tiempo) >= (2 * Tmp)) {
    Tiempo = millis();
  }
}
