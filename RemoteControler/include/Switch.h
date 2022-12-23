/**
 * @file Switch.h
 * @author Tomasz Marcin Kowalski
 * @brief Function/Class prototypes for three-position switch 
 * @version 1.0
 * @date 2022-12-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __Switch__
#define __Switch__

#include <Arduino.h>

/**
 * @brief This class implements functions for reading three-position switch
 */

class Switch
{
private:
  int _switch_pin0, _switch_pin1, _switch_pin2;

public:
  bool switch_state_0, switch_state_1, switch_state_2;
  Switch(int sw_pin0, int sw_pin1, int sw_pin2);
  int read_swich_state();
};

#endif