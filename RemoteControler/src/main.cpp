#include <Arduino.h>
#include <ArduinoJson.h>
#include <LoRa.h>

class RemoteController
{
private:
  int switch_pin0, switch_pin1, switch_pin2;

public:
  bool switch_state_0, switch_state_1, switch_state_2;
  RemoteController(int sw_pin0, int sw_pin1, int sw_pin2)
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

// switches objects
RemoteController heightSwitch(3, 4, 5);
RemoteController massSwitch(0, 1, 2);

void setup()
{
  // start serial comunication
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Setup started!");
  heightSwitch.set_switch_mode();
  massSwitch.set_switch_mode();
  Serial.println("Setup successfully completedd!");
}

void loop()
{
  // put your main code here, to run repeatedly:
 
  Serial.print("Height switch state: ");
  Serial.print(heightSwitch.read_swich_state());
  Serial.print("\t Mass switch state: ");
  Serial.print(massSwitch.read_swich_state());
  Serial.print("\n");
  delay(2000);
}