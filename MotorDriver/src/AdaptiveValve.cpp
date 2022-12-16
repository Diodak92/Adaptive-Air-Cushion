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
    _set_position = set_position; // constrain(set_position, _displacement_min, _displacement_max);
}

bool AdaptiveValve::begin()
{
    _tle.begin(_tle_cs_pin);
    return _ads.begin(_ads_i2c_addr);
}

float AdaptiveValve::decode_position(int remote_settings)
{
    if (remote_settings != -1)
    {
        _remote_code = remote_settings;
    }
    
    return (_remote_code / (_remote_combinations - 1.0)) * (_displacement_max - _displacement_min) + _displacement_min;
}

void AdaptiveValve::set_position(float new_position)
{
    _set_position = new_position; // constrain(new_position, _displacement_min, _displacement_max);
}

float AdaptiveValve::get_position()
{
    u = _ads.computeVolts(_ads.readADC_SingleEnded(_ads_channel));
    ang_position = (u * _ang_position_max) / _u_max;
    displacement = (ang_position / 360.0) * 3.14159 * _measurement_gear_diameter;
    return displacement;
}

bool AdaptiveValve::controller(float displacement_tolerance)
{
    // update dispcacement tolerance
    _displacement_tolerance = displacement_tolerance;
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
        // update object variable
        in_position = false;
        // return value as well
        return false;
    }
    else
    {
        // turn off the motor
        _tle.set_pwm_dir(0, 0);
        // update object variable
        in_position = true;
        // return value as well
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
