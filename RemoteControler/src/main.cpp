#include <Arduino.h>
#include <ArduinoJson.h>
#include <LoRa.h>

#define ADC_BATTERY A0      // baterry pin
#define GREEN_LED_BATTERY 6 // baterry LED

class Switch
{
private:
  int switch_pin0, switch_pin1, switch_pin2;

public:
  bool switch_state_0, switch_state_1, switch_state_2;
  Switch(int sw_pin0, int sw_pin1, int sw_pin2)
  {
    switch_pin0 = sw_pin0;
    switch_pin1 = sw_pin1;
    switch_pin2 = sw_pin2;
  }

  void set_switch_mode()
  {
    pinMode(switch_pin0, INPUT_PULLUP);
    pinMode(switch_pin1, INPUT_PULLUP);
    pinMode(switch_pin2, INPUT_PULLUP);
  }

  int read_swich_state()
  {
    // read state of switches (NC logic)
    switch_state_0 = digitalRead(switch_pin0);
    switch_state_1 = digitalRead(switch_pin1);
    switch_state_2 = digitalRead(switch_pin2);
    // code state 0 if both switches are shorted
    if ((switch_state_0 == true) & (switch_state_2 == false))
    {
      return 0;
    }
    // code state 1 if switch0 is shorted and switch1 is open
    else if ((switch_state_0 == false) & (switch_state_2 == true))
    {
      return 2;
    }
    // code state 1 if both switches are open
    else if ((switch_state_0 & switch_state_2) == false)
    {
      return 1;
    }
    // return error for uknown states
    else
    {
      return -1;
    }
  }
};

class Battery
{
private:
  int battery_pin;
  int led_indicator_pin;

public:
  Battery(int bat_pin, int led_pin)
  {
    battery_pin = bat_pin;
    led_indicator_pin = led_pin;
  };

  // init this one in void setup
  void setup_battery()
  {
    analogReadResolution(12);
    pinMode(battery_pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(led_indicator_pin), blink, CHANGE);
  }

  float read_battery_voltage()
  {
    // Convert the analog reading (which goes from 0 - 4095) to a voltage (0 - 4.2V):
    return (analogRead(battery_pin) * (3.3 / 4095.0)) * (4.2 / 3.3); //* (4.2 / 4095.0);
  }
  bool low_battery_warning(float battery_voltage)
  {
    if (battery_voltage >= 3.15)
      return false;
    else
      return true;
  }
  static void blink()
  {
  }
};

// create json object for storing data
const int capacity_out = JSON_OBJECT_SIZE(2);
StaticJsonDocument<capacity_out> output_data;
// Battery object
Battery remoteBattery(ADC_BATTERY, GREEN_LED_BATTERY);
// switches objects
Switch heightSwitch(3, 4, 5);
Switch massSwitch(0, 1, 2);

void setup()
{
  // start serial comunication
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Setup started!");
  remoteBattery.setup_battery();
  heightSwitch.set_switch_mode();
  massSwitch.set_switch_mode();
  Serial.println("Setup successfully completedd!");
}

void loop()
{
  // put your main code here, to run repeatedly:
  int switches_state, switch_state0, switch_state1;
  int valve_level;

  /*
   Serial.print("Height switch state: ");
   Serial.print(heightSwitch.read_swich_state());
   Serial.print("\t Mass switch state: ");
   Serial.print(massSwitch.read_swich_state());
   Serial.print("\n");
 */

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

  output_data["height_mass_code"] = valve_level;
  serializeJson(output_data, Serial);
  Serial.print('\n');
  Serial.print("Battery voltage: ");
  Serial.print(remoteBattery.read_battery_voltage());
  Serial.print(" [V] \n");

  delay(2000);
}