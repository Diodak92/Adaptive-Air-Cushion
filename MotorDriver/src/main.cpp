#include <Arduino.h>
#include <ArduinoJson.h>
#include <LoRa.h>
#include <cmath>
#include <AdaptiveValve.h>

// H-bridge object instance
// TLE9201 h8(7);
// Declare adaptive valve object
AdaptiveValve ad_valve_1(1020.5);

/* MAIN functions declaration */
// void MAIN_print_hbridge_status(void);

void setup()
{
  // start serial comunication
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Setup started!");
  /*ad_valve_1.begin();
  // check if ADC for vavle 1 was initalized successfully
  if (!ad_valve_1.adc_init_status)
  {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }*/
  Serial.println("Setup successfully completedd!");
}

void loop()
{
  ad_valve_1.set_position(1250);
  delay(2000);
  ad_valve_1.set_position(985.7);
  delay(2000);
}