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
    TLE9201 _tle;
    uint8_t _tle_cs_pin;
    float _set_position;
    float _displacement_min, _displacement_max;
    const float _displacement_tolerance = 2.5;
    const int _ang_position_max = 3600;
    const float _u_max = 3.3;
    const int _measurement_gear_diameter = 25;
    const int _remote_combinations = 9; 

public:
    bool in_position;
    float u;
    float ang_position;
    float displacement;

    AdaptiveValve(u_int8_t ads_i2c_addr, uint8_t ads_channel,
                  uint8_t tle_cs_pin,
                  float set_position = 0,
                  float displacement_min = 0, float displacement_max = 70);

    bool begin();
    float decode_position(int remote_setting);
    void set_position(float new_position);
    float get_position();
    void controller();
    void print_position();
};

#endif