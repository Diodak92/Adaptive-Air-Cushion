#include <Arduino.h>
#include <arduino-timer.h>
#include <ArduinoJson.h>
#include <LoRa.h>
#include <loraHelperFunctions.h>
#include <Switch.h>
#include <Battery.h>

#define ADC_BATTERY A0  // baterry pin
#define LED_PWR_GREEN 6 // baterry LED
#define LED_LINK_BLUE 7 // lora status LED

// LoRa Frequency
const long frequency = 868E6;
// create a timer with default settings
auto timer = timer_create_default();
// create json object for storing data
const int capacity_out = JSON_OBJECT_SIZE(2);
StaticJsonDocument<capacity_out> output_data;
// switches objects
Switch heightSwitch(3, 4, 5);
Switch massSwitch(0, 1, 2);
// Battery object
Battery remoteBattery(ADC_BATTERY, LED_PWR_GREEN);

// read battery voltage toggle LED if battery is low
bool toggle_led(void *)
{
  float battery_voltage = remoteBattery.read_battery_voltage();
  if (remoteBattery.low_battery_warning(battery_voltage))
  {
    digitalWrite(LED_PWR_GREEN, !digitalRead(LED_PWR_GREEN));
    // Serial.println("Battery low!");
  }
  else
  {
    digitalWrite(LED_PWR_GREEN, HIGH);
  }
  return true;
}

void setup()
{
  // start serial comunication
  Serial.begin(115200);
  while (!Serial);
  // turn LEDs ON
  digitalWrite(LED_PWR_GREEN, HIGH);
  digitalWrite(LED_LINK_BLUE, HIGH);
  // signal if LoRa init failed
  if (!LoRa.begin(frequency))
  {
    while (true)
    {
      digitalWrite(LED_LINK_BLUE, HIGH);
      delay(200);
      digitalWrite(LED_LINK_BLUE, LOW);
      delay(1000);
    }
  }

  Serial.println("LoRa init succeeded.");
  Serial.println();
  Serial.println("LoRa Simple Gateway");
  Serial.println("Only receive messages from nodes");
  Serial.println("Tx: invertIQ enable");
  Serial.println("Rx: invertIQ disable");
  Serial.println();
  // setup interrupts and mode for LoRa
  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa_rxMode();

  // call the toggle_led function every X millis
  timer.every(500, toggle_led);
}

void loop()
{
  // put your main code here, to run repeatedly:
  int switches_state, switch_state0, switch_state1;
  int valve_level;

  timer.tick(); // tick the timer

  /*
   Serial.print("Height switch state: ");
   Serial.print(heightSwitch.read_swich_state());
   Serial.print("\t Mass switch state: ");
   Serial.print(massSwitch.read_swich_state());
   Serial.print("\n");
 */

  // read state of switches and code it as one variable
  switch_state0 = heightSwitch.read_swich_state();
  switch_state1 = massSwitch.read_swich_state();
  switches_state = (switch_state0 << 2) + switch_state1;

  switch (switches_state)
  {
  case 0b0000:
    valve_level = 0; // Height: L, Mass: L
    break;
  case 0b0001:
    valve_level = 1; // Height: L, Mass: M
    break;
  case 0b0010:
    valve_level = 2; // Height: L, Mass: H
    break;
  case 0b0100:
    valve_level = 3; // Height: M, Mass: L
    break;
  case 0b0101:
    valve_level = 4; // Height: M, Mass: M
    break;
  case 0b0110:
    valve_level = 5; // Height: M, Mass: H
    break;
  case 0b1000:
    valve_level = 6; // Height: H, Mass: L
    break;
  case 0b1001:
    valve_level = 7; // Height: H, Mass: M
    break;
  case 0b1010:
    valve_level = 8; // Height: H, Mass: H
    break;

  default:
    valve_level = -1;
    break;
  }

  // add switches status to JSON object
  output_data["height_mass_code"] = valve_level;

  if (runEvery(1000)) // repeat every 1000 millis
  {
    LoRa_txMode();                    // set tx mode
    LoRa.beginPacket();               // start packet
    serializeJson(output_data, LoRa); // add payload
    LoRa.endPacket(true);             // finish packet and send it
    serializeJson(output_data, Serial); // add payload
  }
}