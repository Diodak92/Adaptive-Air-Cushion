#ifndef __Battery__
#define __Battery__

#include <Arduino.h>

class Battery
{
private:
  int _battery_pin;
  int _led_indicator_pin;

public:
  Battery(int bat_pin, int led_pin);
  float read_battery_voltage();
  bool low_battery_warning(float battery_voltage);
};

#endif