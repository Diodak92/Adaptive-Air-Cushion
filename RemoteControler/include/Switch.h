#ifndef __Switch__
#define __Switch__

#include <Arduino.h>

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