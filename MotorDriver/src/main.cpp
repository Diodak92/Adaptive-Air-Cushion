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
  // change valve position
  ad_valve_1.set_position(10);
  ad_valve_1.controller();
  ad_valve_1.print_position();
  // tested -- motor drive is running in one direction if measured
  // position is lower than set_position with _displacement_tolerance
  // stops when position is within tolerance range and is running again but
  // in opposite direction if set_position is higher than set_position
  // with _displacement_tolerance

  // open valve
  // ad_valve_1.set_position(0);
  //ad_valve_1.print_position();
  //delay(10000);
  // while (ad_valve_1.controller())
  // {
  //  ad_valve_1.print_position();
  //  delay(2000);
  //}
  // close valve
  // ad_valve_1.set_position(50);
  // while (ad_valve_1.controller())
  //{
  //  ad_valve_1.print_position();
  //  delay(2000);
  // }
}