#ifndef __AdaptiveValve__
#define __Adaptivevalve__

#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include <TLE9201.h>

class AdaptiveValve
{
private:
    Adafruit_ADS1015 _ads;
    uint8_t _ads_i2c_addr;
    uint8_t _ads_channel;
    float _set_position;
    float _displacement;
    const int _displacement_min = 0, _displacement_max = 70;
    float _ang_position;
    const int _ang_position_max = 270;
    float _u;
    const float _u_max = 3.3;
    const int _measurement_gear_diameter = 25;

public:
    AdaptiveValve(u_int8_t ads_i2c_addr, uint8_t ads_channel, float set_position = 0)
    {
        _ads_i2c_addr = ads_i2c_addr;
        _ads_channel = ads_channel;
        _set_position = constrain(set_position, _displacement_min, _displacement_max);
    }

    bool begin()
    {
        return _ads.begin(_ads_i2c_addr);
    }

    void set_position(float new_position)
    {
        Serial.print("Old position: ");
        Serial.print(_set_position);
        Serial.print(" [mm] \t");
        _set_position = constrain(new_position, _displacement_min, _displacement_max);
        Serial.print("New position: ");
        Serial.print(_set_position);
        Serial.print(" [mm] \n");
    }

    float get_position()
    {
        _u = _ads.computeVolts(_ads.readADC_SingleEnded(_ads_channel));
        _ang_position = (_u * _ang_position_max) / _u_max;
        _displacement = (_ang_position / 360.0) * 3.14159 * _measurement_gear_diameter;
        return _displacement;
    }

    void print_position()
    {
        get_position();
        Serial.print("Set position: ");
        Serial.print(_set_position);
        Serial.print(" [mm] \t");
        Serial.print(" Measured position: ");
        Serial.print(_displacement);
        Serial.print(" [mm] \n");
    }
};

#endif