#ifndef __AdaptiveValve__
#define __Adaptivevalve__

#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include <TLE9201.h>

class AdaptiveValve
{
private:
    TLE9201 motor_driver; // H-bridge motor driver object
    int motor_channel; // motor driver channel
    Adafruit_ADS1115 ads;
    int ads_i2c_address;
    uint8_t ads_analog_channel;
    // valve position set point
    float valve_pos_set; 
    // angular positions limits
    float ang_pos_valve_min;
    float ang_pos_valve_max;
    // voltage for adc calculations
    const float u_min = 0.0;
    const float u_max = 3.3;
    // angular position variables [deg] - entire range of potentiometer 0 - 3600 deg
    const float pot_pos_min = 0.0;
    const float pot_pos_max = 3600.0;
    // measurement gear nominal diameter
    const float gear_diameter = 22.0;
    // valve tolerance
    const float valve_pos_tolerance = 10.0;

public:
    // adc initialization status
    bool adc_init_status;
    // potentiometer output voltage [V]
    float u;
    // gear angular position [deg]
    float ang_pos;
    // valve displacement [mm]
    float distance;

    AdaptiveValve(int motor_channel,
                  int ads_i2c_address, uint8_t ads_analog_channel,
                  float ang_pos_valve_min, float ang_pos_valve_max, float valve_pos_set)
    {
    }

    void begin();
    void set_position(float set_position);
    void compute_position();
    void print_set_position();
    void print_actual_position();
    void controller();
};

#endif