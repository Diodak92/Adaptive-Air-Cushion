/**
 * @file AdaptiveValve.h
 * @author Tomasz Marcin Kowalski
 * @brief Function/Class prototypes for controling adaptive valve
 * @version 1.0
 * @date 2022-12-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __AdaptiveValve__
#define __Adaptivevalve__

#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include <TLE9201.h>

/**
 * @brief This class implements all functions needed to opearate adaptive vale 
 */

class AdaptiveValve
{
private:
    Adafruit_ADS1015 _ads; // analog to digital conventer IC object
    uint8_t _ads_i2c_addr; // I2C addres of analog to digital conventer
    uint8_t _ads_channel; //analog to digital conventer channel (0-3)
    TLE9201 _tle; // motor driver object
    uint8_t _tle_cs_pin; // chip select pin for motor driver (TLE)
    float _set_position; // set position for valve [mm]
    float _displacement_min, _displacement_max; // absolute minimum and maximum 
    float _displacement_tolerance; // displacement tolerance for set position in [mm]
    const int _ang_position_max = 3600; // angular range of measurement potentiometer [deg]
    const float _u_max = 3.3; // ADC supply voltage - adjust if needed [V]
    const int _measurement_gear_diameter = 25; // diameter of measurement gear [mm]
    const int _remote_combinations = 9; // number of remote controler combinations 
    int _remote_code; // remote code 

public:
    bool in_position; // if valve is within set range or not
    float u; // voltage reading from analog to digital conventer [V]
    float ang_position; // measured angular position [deg]
    float displacement; // measured displacement [mm]

    AdaptiveValve(u_int8_t ads_i2c_addr, uint8_t ads_channel,
                  uint8_t tle_cs_pin,
                  float set_position = 0,
                  float displacement_min = 0, float displacement_max = 70);

    bool begin();
    float decode_position(int remote_setting);
    void set_position(float new_position);
    float get_position();
    bool controller(float displacement_tolerance = 2.5);
    void print_position();
};

#endif