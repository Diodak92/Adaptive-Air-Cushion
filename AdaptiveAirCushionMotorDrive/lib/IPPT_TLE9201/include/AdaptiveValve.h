/*

#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include <TLE9201.h>

class AdaptiveValve
{
private:
  // motor driver channel
  int motor_channel; 
  // H-bridge motor driver object
  TLE9201 motor_driver(int motor_channel);
  // ADC and ADC channel variables
  Adafruit_ADS1115 adc_ic;
  uint8_t adc_channel;
  // valve position set point
  float valve_pos_set;
  // angular positions limits
  float ang_pos_valve_min;
  float ang_pos_valve_max;
  // voltage for adc calculations
  const float u_min = 0.0;
  const float u_max = 3.3;
  // angular position variables [deg] - entire range of potentiometer 0 - 3600 deg
  const float ang_pos_min = 0.0;
  const float ang_pos_max = 3600.0;
  // measurement gear nominal diameter
  const float gear_diameter = 22.0;
  // valve tolerance
  const float valve_pos_tolerance = 10.0;

public:
  // potentiometer output voltage [V]
  float u;
  // gear angular position [deg]
  float ang_pos;
  // valve displacement [mm]
  float distance;

  AdaptiveValve(TLE9201 motor,  
                Adafruit_ADS1115 ads1115, uint8_t channel,
                float pos_min, float pos_max,
                float set_position);

  void set_valve_position(float set_position);
  void compute_position();
  void controller();
  void print_actual_position();
  void print_set_position();
};

*/