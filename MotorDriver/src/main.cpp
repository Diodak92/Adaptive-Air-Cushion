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
DynamicJsonDocument doc(1024);

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

void loop()
{
  // regulator loop
  ad_valve_1.controller();

  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    // read packet
    while (LoRa.available())
    {
      // Serial.print((char)LoRa.read());
      // Serial.print(LoRa.readString());
      // String remote_code_json = LoRa.readString();
      deserializeJson(doc, LoRa.readString());
      int remote_code = doc["height_mass_code"];
      ad_valve_1.set_position(ad_valve_1.decode_position(remote_code));
      Serial.println(remote_code);
    }

    // print RSSI of packet
    // Serial.print("' with RSSI ");
    // Serial.println(LoRa.packetRssi());
  }

    // print controler position
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
}