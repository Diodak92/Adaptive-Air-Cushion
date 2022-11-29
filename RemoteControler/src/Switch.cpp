#include "Switch.h"

Switch::Switch(int sw_pin0, int sw_pin1, int sw_pin2)
{
    _switch_pin0 = sw_pin0;
    _switch_pin1 = sw_pin1;
    _switch_pin2 = sw_pin2;
    pinMode(_switch_pin0, INPUT_PULLUP);
    pinMode(_switch_pin1, INPUT_PULLUP);
    pinMode(_switch_pin2, INPUT_PULLUP);
}

int Switch::read_swich_state()
{
    // read state of switches (NC logic)
    switch_state_0 = digitalRead(_switch_pin0);
    switch_state_1 = digitalRead(_switch_pin1);
    switch_state_2 = digitalRead(_switch_pin2);
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
