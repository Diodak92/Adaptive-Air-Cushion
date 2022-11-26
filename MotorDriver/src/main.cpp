#include <Arduino.h>
#include <ArduinoJson.h>
#include <LoRa.h>
#include <AdaptiveValve.h>

// H-bridge object instance
// TLE9201 h8(7);
// Declare adaptive valve object
AdaptiveValve ad_valve_1(0b1001001, 3, 7);
AdaptiveValve ad_valve_2(0b1001001, 2, 6);

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
  Serial.println("Setup successfully completed!");
}

void loop()
{
  // open valve
  ad_valve_1.set_position(0);
  while (ad_valve_1.controller())
  {
    ad_valve_1.print_position();
    delay(500);
  }
  // close valve
  ad_valve_1.set_position(70);
  while (ad_valve_1.controller())
  {
    ad_valve_1.print_position();
    delay(500);
  }
}