#include <Arduino.h>
#include <ArduinoJson.h>
#include <LoRa.h>
#include <cmath>
#include <AdaptiveValve.h>

// H-bridge object instance
// TLE9201 h8(7);
// Declare adaptive valve object
AdaptiveValve ad_valve_1(0b1001001, 3, 1020.5);
AdaptiveValve ad_valve_2(0b1001001, 2, 1020.5);

/* MAIN functions declaration */
// void MAIN_print_hbridge_status(void);

void setup()
{
  // start serial comunication
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Setup started!");
  // check if ADC for vavle 1 was initalized successfully
  Serial.println(ad_valve_1.begin());
  Serial.println(ad_valve_2.begin());
  Serial.println("Setup successfully completedd!");
}

void loop()
{
  ad_valve_1.set_position(1250);
  delay(2000);
  ad_valve_1.set_position(985.7);
  delay(2000);
  Serial.println(ad_valve_1.get_position());
  Serial.println(ad_valve_2.get_position());
}