/**
 * @file Battery.cpp
 * @author Tomasz Marcin Kowalski
 * @brief Function definitions for battery
 * @version 1.0
 * @date 2022-12-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "Battery.h"

/**
 * @brief Initializes battery object
 * @param bat_pin Battery measurement pin
 * @param led_pin Led indicator pin
 */

Battery::Battery(int bat_pin, int led_pin)
{
  _battery_pin = bat_pin;
  _led_indicator_pin = led_pin;
  analogReadResolution(12);
  pinMode(_battery_pin, INPUT);
  pinMode(_led_indicator_pin, OUTPUT);
  digitalWrite(_led_indicator_pin, HIGH);
}

/**
 * @brief Read single cell LiPo battery voltage
 * @return Battery voltage in range 0 : 4.2 [V]
 */

float Battery::read_battery_voltage()
{
  // Convert the analog reading (which goes from 0 - 4095) to a voltage (0 - 4.2V):
  return (analogRead(_battery_pin) * (3.223 / 4095.0)) * (4.2 / 3.223);
}

/**
 * @brief Check if battery is discharged
 * @return True if battery voltege drops below 3.15 [V]
 */

bool Battery::low_battery_warning(float battery_voltage)
{
  if (battery_voltage >= 3.15)
    return false;
  else
    return true;
}