#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_ADS1X15.h>
#include <TLE9201.h>
#include <LoRa.h>
#include <cmath>

class AdaptiveValve
{
private:
  // motor driver channel
  int motor_channel;
  // H-bridge motor driver object
  // TLE9201 motor_driver;
  // ADC and ADC channel variables
  Adafruit_ADS1115 adc_ic;
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

  AdaptiveValve(int motor_channel,
      int ads_i2c_address, uint8_t ads_analog_channel,
      float ang_pos_valve_min = 1120.0, float ang_pos_valve_max = 1490.0,
      float valve_pos_set = 1140)
  {
    //motor_driver(motor_channel);
    ads_i2c_address = ads_i2c_address;
    ads_analog_channel = ads_analog_channel;
    ang_pos_valve_min = ang_pos_valve_min;
    ang_pos_valve_max = ang_pos_valve_max;
    valve_pos_set = constrain(valve_pos_set, ang_pos_valve_min, ang_pos_valve_max);
  }

  // start tle H-bridge motor driver
  //void tle_begin(){
  // motor_driver.begin();
  //}

  // start ads ADC
  bool ads_begin()
  {
    return adc_ic.begin(ads_i2c_address);
  }

  // set valve position
  void set_valve_position(float set_position)
  {
    valve_pos_set = constrain(set_position, ang_pos_valve_min, ang_pos_valve_max);
  }

  // Compute position function declaration
  void compute_position()
  {
    // convert counts into voltage
    u = adc_ic.computeVolts(adc_ic.readADC_SingleEnded(ads_analog_channel)); // replace ads_2 with class instance
    // calculate angular position;
    ang_pos = (u - u_min) * (ang_pos_max - ang_pos_min) / (u_max - u_min) + ang_pos_min;
    // compute absolute distance
    distance = (ang_pos / 360.0) * M_PI * gear_diameter;
  }

  // print position
  void print_actual_position()
  {
    compute_position();
    Serial.print("position: ");
    Serial.print(ang_pos);
    Serial.println(" [deg]");
  }

  void print_set_position()
  {
    Serial.print("Set position: ");
    Serial.print(valve_pos_set);
    Serial.print(" [deg], ");
  }
  /*
    void controller()
    {
      // update position on each call
      compute_position();
      // check if valve position is within tolerance
      if (abs(valve_pos_set - ang_pos) >= valve_pos_tolerance)
      {
        // control motor direction
        if (valve_pos_set >= ang_pos)
        {
          // turn motor on in forward direction
          motor_driver.set_pwm_dir(1, 1);
          Serial.println("Motor: ON, direction: UP");
        }
        else
        {
          // turn motor on in reverse direction
          motor_driver.set_pwm_dir(1, 0);
          Serial.println("Motor: ON, direction: DOWN");
        }
      }
      else
      {
        // turn off the motor
        motor_driver.set_pwm_dir(0, 0);
        Serial.println("Valve in position : )");
      }

    }
    */
};

// H-bridge object instance
// TLE9201 h8(7);
// Declare adaptive valve object
AdaptiveValve ad_valve_1(7, 0b1001001, 3);

/* MAIN functions declaration */
// void MAIN_print_hbridge_status(void);

void setup()
{
  // start serial comunication
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Setup started!");
  // initialization of H-bridge
  // ad_valve_1.tle_begin();
  // initialize external ADC
  if (!ad_valve_1.ads_begin())
  {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
  Serial.println("Setup successfully completedd!");
}

void loop()
{
  ad_valve_1.set_valve_position(1140);
  ad_valve_1.print_set_position();
  ad_valve_1.print_actual_position();
  // ad_valve_1.controller();

  // wait for next loop iter
  delay(50);
}