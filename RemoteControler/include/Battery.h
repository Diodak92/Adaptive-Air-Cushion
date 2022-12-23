/**
 * @file Switch.h
 * @author Tomasz Marcin Kowalski
 * @brief Function/Class prototypes for battery
 * @version 1.0
 * @date 2022-12-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __Battery__
#define __Battery__

#include <Arduino.h>

/**
 * @brief This class implements functions for reading and battery voltage and signaling discharge
 */

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