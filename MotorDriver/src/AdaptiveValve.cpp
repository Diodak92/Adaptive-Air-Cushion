#include "AdaptiveValve.h"

AdaptiveValve::AdaptiveValve(u_int8_t ads_i2c_addr, uint8_t ads_channel,
                             uint8_t tle_cs_pin,
                             float set_position,
                             float displacement_min,
                             float displacement_max)
{
    _ads_i2c_addr = ads_i2c_addr;
    _ads_channel = ads_channel;
    _tle_cs_pin = tle_cs_pin;
    _displacement_min = displacement_min;
    _displacement_max = displacement_max;
    _set_position = constrain(set_position, _displacement_min, _displacement_max);
}

bool AdaptiveValve::begin()
{
    _tle.begin(_tle_cs_pin);
    return _ads.begin(_ads_i2c_addr);
}

void AdaptiveValve::set_position(float new_position)
{
    Serial.print("Old position: ");
    Serial.print(_set_position);
    Serial.print(" [mm] \t");
    _set_position = constrain(new_position, _displacement_min, _displacement_max);
    Serial.print("New position: ");
    Serial.print(_set_position);
    Serial.print(" [mm] \n");
}

float AdaptiveValve::get_position()
{
    u = _ads.computeVolts(_ads.readADC_SingleEnded(_ads_channel));
    ang_position = (u * _ang_position_max) / _u_max;
    displacement = (ang_position / 360.0) * 3.14159 * _measurement_gear_diameter;
    return displacement;
}

bool AdaptiveValve::controller()
{
    // update position on each call
    get_position();
    // check if valve position is within tolerance
    if (abs(_set_position - displacement) >= _displacement_tolerance)
    {
        // control motor direction
        if (_set_position >= displacement)
        {
            // turn motor on in forward direction
            _tle.set_pwm_dir(1, 1);
        }
        else
        {
            // turn motor on in reverse direction
            _tle.set_pwm_dir(1, 0);
        }
        return false;
    }
    else
    {
        // turn off the motor
        _tle.set_pwm_dir(0, 0);
        return true;
    }
}

void AdaptiveValve::print_position()
{
    get_position();
    Serial.print("Set position: ");
    Serial.print(_set_position);
    Serial.print(" [mm] \t");
    Serial.print(" Measured position: ");
    Serial.print(displacement);
    Serial.print(" [mm] \n");
}
