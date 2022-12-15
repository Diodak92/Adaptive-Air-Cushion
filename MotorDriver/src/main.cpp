#include <Arduino.h>
#include <arduino-timer.h>
#include <ArduinoJson.h>
#include <LoRa.h>
#include <loraHelperFunctions.h>
#include <AdaptiveValve.h>

// create a timer with default settings
auto timer = timer_create_default();
// Declare adaptive valve object
AdaptiveValve ad_valve_1(0b1001001, 3, 7, 410.0, 410.0, 480.0);
AdaptiveValve ad_valve_2(0b1001001, 2, 6);
// LoRa Frequency
const long frequency = 868E6;
// Declare object for Json file
DynamicJsonDocument doc(256);

void setup()
{
  // start serial comunication
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("Setup started!");
  if (!LoRa.begin(frequency))
  {
    Serial.println("Starting LoRa failed!");
    while (true)
      ;
  }

  Serial.println("LoRa init succeeded.");
  Serial.println();
  Serial.println("LoRa Simple Node");
  Serial.println("Only receive messages from gateways");
  Serial.println("Tx: invertIQ disable");
  Serial.println("Rx: invertIQ enable");
  Serial.println();

  // LoRa.onReceive(onReceive);
  // LoRa.onTxDone(onTxDone);
  // LoRa_rxMode();

  // check if ADC for vavle 1 was initalized successfully
  Serial.println(ad_valve_1.begin());
  Serial.println(ad_valve_2.begin());
  // timer.every(500, print_position);
  Serial.println("Setup successfully completed!");
}

int remote_command = 0;

void loop()
{

  // print controler position
  //if (runEvery(1000))
  //{
  //  ad_valve_1.print_position();
  //}

  // regulator loop
  ad_valve_1.controller();
  if (runEvery(1000))
  {
    ad_valve_1.print_position();
    Serial.println(ad_valve_1.in_position);
  }
/*
  // change set position
  if (runEvery(3500))
  {
    if (ad_valve_1.in_position & (remote_command == 0))
    {
      remote_command = 8;
      ad_valve_1.set_position(ad_valve_1.decode_position(remote_command));
    }
    if (ad_valve_1.in_position & (remote_command == 8))
    {
      remote_command = 0;
      ad_valve_1.set_position(ad_valve_1.decode_position(remote_command));
    }
  }
*/

   // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      // received a packet

      // read packet
      while (LoRa.available()) {
        Serial.print((char)LoRa.read());
        //deserializeJson(doc, LoRa.read());
        //int remote_code = doc["height_mass_code"];
        //Serial.println(remote_code);
      }

      // print RSSI of packet
      //Serial.print("' with RSSI ");
      //Serial.println(LoRa.packetRssi());
    }


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