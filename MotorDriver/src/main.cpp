#include <Arduino.h>
#include <arduino-timer.h>
#include <ArduinoJson.h>
#include <LoRa.h>
#include <loraHelperFunctions.h>
#include <AdaptiveValve.h>

// create a timer with default settings
auto timer = timer_create_default();
// Declare adaptive valve object
AdaptiveValve ad_valve_1(0b1001001, 3, 7, 390.0, 380.0, 400.0);
AdaptiveValve ad_valve_2(0b1001001, 2, 6);
// LoRa Frequency
const long frequency = 868E6;

bool print_position(void *)
{
  // ad_valve_1.get_position();
  // ad_valve_1.print_position();
  return true;
}

void setup()
{
  // start serial comunication
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Setup started!");
  if (!LoRa.begin(frequency))
  {
    Serial.println("Starting LoRa failed!");
    while (true);
  }
  
  Serial.println("LoRa init succeeded.");
  Serial.println();
  Serial.println("LoRa Simple Node");
  Serial.println("Only receive messages from gateways");
  Serial.println("Tx: invertIQ disable");
  Serial.println("Rx: invertIQ enable");
  Serial.println();

  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa_rxMode();

  // check if ADC for vavle 1 was initalized successfully
  Serial.println(ad_valve_1.begin());
  Serial.println(ad_valve_2.begin());
  //timer.every(500, print_position);
  Serial.println("Setup successfully completed!");
}

void loop()
{
  // timer.tick();
  /*  ad_valve_1.set_position(ad_valve_1.decode_position(2));
  delay(2000);
  ad_valve_1.set_position(ad_valve_1.decode_position(6));
  delay(2000);
  */

  // open valve
  // float pos1 = ad_valve_1.decode_position(0), pos2 = ad_valve_1.decode_position(8);
  // Serial.println(pos1);
  // ad_valve_1.set_position(ad_valve_1.decode_position(0));
  // while (ad_valve_1.controller());
  // Serial.println("Valve in position!");
  // delay(2000);
  // close valve
  // Serial.println(pos2);
  // ad_valve_1.set_position(ad_valve_1.decode_position(8));
  // while (ad_valve_1.controller());
  // Serial.println("Valve in position!");
  // delay(2000);
}