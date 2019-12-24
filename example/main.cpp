#include "LC709203F.h"
#include <Arduino.h>
#include <Wire.h>

LC709203F sensor;

void setup() {

  // Wire.setClock(10000);
  Serial.begin(9600);
  Serial.setTimeout(60000);
  while (!Serial)
    ;
  Wire.begin();
  sensor.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Voltage: ");
  Serial.println(sensor.getBatteryVoltage());
  Serial.print("Capacity: ");
  Serial.println(sensor.getBatteryLevelAccu());
  // Serial.println(getVersion());
  delay(2000);
}
