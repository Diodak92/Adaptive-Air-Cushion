#include "Battery.h"

  Battery::Battery(int bat_pin, int led_pin)
  {
    _battery_pin = bat_pin;
    _led_indicator_pin = led_pin;
    analogReadResolution(12);
    pinMode(_battery_pin, INPUT);
    pinMode(_led_indicator_pin, OUTPUT);
    digitalWrite(_led_indicator_pin, HIGH);
  }

  float Battery::read_battery_voltage()
  {
    // Convert the analog reading (which goes from 0 - 4095) to a voltage (0 - 4.2V):
    return (analogRead(_battery_pin) * (3.223 / 4095.0)) * (4.2 / 3.223);
  }

  bool Battery::low_battery_warning(float battery_voltage)
  {
    if (battery_voltage >= 3.15)
      return false;
    else
      return true;
  }