/**
 * @file main.cpp
 * @author Tomasz Marcin Kowalski
 * @brief Main function for controling adaptive air cushion
 * @version 1.0
 * @date 2022-12-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <Arduino.h>
#include <arduino-timer.h>
#include <ArduinoJson.h>
#include <LoRa.h>
#include <loraHelperFunctions.h>
#include <AdaptiveValve.h>

// create a timer with default settings
auto timer = timer_create_default();
// Declare adaptive valve object
// ADC I2C addres, ADC channel, motor driver (TLE) channel,
// set position, lower position limit, higher position limit
AdaptiveValve ad_valve_1(0b1001001, 3, 7, 410.0, 410.0, 480.0);
AdaptiveValve ad_valve_2(0b1001001, 2, 6, 410.0, 410.0, 480.0);
AdaptiveValve ad_valve_3(0b1001001, 1, 5, 410.0, 410.0, 480.0);
AdaptiveValve ad_valve_4(0b1001001, 0, 4, 410.0, 410.0, 480.0);
AdaptiveValve ad_valve_5(0b1001000, 3, 3, 410.0, 410.0, 480.0);
AdaptiveValve ad_valve_6(0b1001000, 2, 2, 410.0, 410.0, 480.0);
AdaptiveValve ad_valve_7(0b1001000, 1, 1, 410.0, 410.0, 480.0);
AdaptiveValve ad_valve_8(0b1001000, 0, 0, 410.0, 410.0, 480.0);
// LoRa Frequency [Hz]
const long frequency = 868E6;
// Declare object for Json file
DynamicJsonDocument doc(1024);

// change this vale for enableling serial output
bool serial_enable = false;

void setup()
{
  // start serial comunication
  if (serial_enable)
  {
    Serial.begin(115200);
    while (!Serial);
    Serial.println("Setup started!");
  }
  // start LoRa communication
  if (!LoRa.begin(frequency))
  {
    if (serial_enable)
    {
      Serial.println("Starting LoRa failed!");
    }
    while (true);
  }
  else
  {
    if (serial_enable)
    {
      Serial.println("LoRa init succeeded.");
    }
  }

  // initialize adaptive valves
  if (serial_enable)
  {
    // check if ADC for vavle 1 was initalized successfully
    Serial.println(ad_valve_1.begin());
    Serial.println(ad_valve_2.begin());
    Serial.println(ad_valve_3.begin());
    Serial.println(ad_valve_4.begin());
    Serial.println(ad_valve_5.begin());
    Serial.println(ad_valve_6.begin());
    Serial.println(ad_valve_7.begin());
    Serial.println(ad_valve_8.begin());
    Serial.println("Setup successfully completed!");
  }
  else
  {
    ad_valve_1.begin();
    ad_valve_2.begin();
    ad_valve_3.begin();
    ad_valve_4.begin();
    ad_valve_5.begin();
    ad_valve_6.begin();
    ad_valve_7.begin();
    ad_valve_8.begin();
  }
}

void loop()
{
  // regulator loop
  ad_valve_1.controller(2.5);
  ad_valve_2.controller();
  ad_valve_3.controller();
  ad_valve_4.controller();
  ad_valve_5.controller();
  ad_valve_6.controller();
  ad_valve_7.controller();
  ad_valve_8.controller();

  // try to parse LoRa packet
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    // read packet
    while (LoRa.available())
    {
      // print recived message
      if (serial_enable)
      {
        Serial.print(LoRa.readString());
      }

      // deserialize and unpack value from JSON object
      deserializeJson(doc, LoRa.readString());
      int remote_code = doc["height_mass_code"];
      ad_valve_1.set_position(ad_valve_1.decode_position(remote_code));
      ad_valve_2.set_position(ad_valve_2.decode_position(remote_code));
      ad_valve_3.set_position(ad_valve_3.decode_position(remote_code));
      ad_valve_4.set_position(ad_valve_4.decode_position(remote_code));
      ad_valve_5.set_position(ad_valve_5.decode_position(remote_code));
      ad_valve_6.set_position(ad_valve_6.decode_position(remote_code));
      ad_valve_7.set_position(ad_valve_7.decode_position(remote_code));
      ad_valve_8.set_position(ad_valve_8.decode_position(remote_code));
      
      // print unpacked remote control code 
      if (serial_enable)
      {
        Serial.println(remote_code);
      }
    }

    // print strength of LoRa signal
    // print RSSI of packet
    // Serial.print("' with RSSI ");
    // Serial.println(LoRa.packetRssi());
  }

  // print controler position
  if (serial_enable)
  {
    if (runEvery(1000))
    {
      ad_valve_1.print_position();
    }
  }
}