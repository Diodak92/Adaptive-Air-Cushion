#include "AdaptiveValve.h"

/*
AdaptiveValve::AdaptiveValve( // TLE9201 motor,
    Adafruit_ADS1115 ads1115, uint8_t channel,
    float pos_min = 1120.0, float pos_max = 1490.0,
    float set_position = 1140)
{
    // motor_driver() = motor;
    adc_ic = ads1115;
    adc_channel = channel;
    ang_pos_valve_min = pos_min;
    ang_pos_valve_max = pos_max;
    valve_pos_set = constrain(set_position, ang_pos_valve_min, ang_pos_valve_max);
}

// set valve position
AdaptiveValve::set_valve_position(float set_position)
{
    valve_pos_set = constrain(set_position, ang_pos_valve_min, ang_pos_valve_max);
}

// Compute position function declaration
AdaptiveValve::compute_position()
{
    // convert counts into voltage
    u = adc_ic.computeVolts(adc_ic.readADC_SingleEnded(adc_channel)); // replace ads_2 with class instance
    // calculate angular position;
    ang_pos = (u - u_min) * (ang_pos_max - ang_pos_min) / (u_max - u_min) + ang_pos_min;
    // compute absolute distance
    distance = (ang_pos / 360.0) * M_PI * gear_diameter;
}

// print position
AdaptiveValve::print_actual_position()
{
    compute_position();
    Serial.print("position: ");
    Serial.print(ang_pos);
    Serial.println(" [deg]");
}

AdaptiveValve::print_set_position()
{
    Serial.print("Set position: ");
    Serial.print(valve_pos_set);
    Serial.print(" [deg], ");
}

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