#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_ADS1X15.h>
#include <TLE9201.h>
#include <LoRa.h>
#include <cmath>

class AdaptiveValve
{
private:
  // ADC and ADC channel variables
  Adafruit_ADS1115 adc_ic;
  uint8_t adc_channel;
  // voltage variables [V]
  float u;
  const float u_min = 0.0;
  const float u_max = 3.3;
  // angular position variables [deg] - entire range of potentiometer 0 - 3600 deg
  float ang_pos_valve_min;
  float ang_pos_valve_max;
  const float ang_pos_min = 0.0;
  const float ang_pos_max = 3600.0;
  const float gear_diameter = 22.0;

public:
  // measure grear angular position
  float ang_pos;
  // valve displacement
  float distance;
  AdaptiveValve(Adafruit_ADS1115 ads1115, uint8_t channel, float pos_min = 1140.0, float pos_max = 1470.0)
  {
    adc_ic = ads1115;
    adc_channel = channel;
    ang_pos_valve_min = pos_min;
    ang_pos_valve_max = pos_max;
  }

  // Compute position function declaration
  float compute_position()
  {
    // convert counts into voltage
    u = adc_ic.computeVolts(adc_ic.readADC_SingleEnded(adc_channel)); // replace ads_2 with class instance
    // calculate angular position;
    ang_pos = (u - u_min) * (ang_pos_max - ang_pos_min) / (u_max - u_min) + ang_pos_min;
    // compute absolute distance
    distance = (ang_pos / 360.0) * M_PI * gear_diameter;
  }

  // print position
  void print_position()
  {
    Serial.print("position: ");
    Serial.print(ang_pos);
    Serial.println(" [deg]");
  }
};

// H-bridge object instance
TLE9201 h8(7);
// Analog to digital conventer instance
Adafruit_ADS1115 ads_2;
// Declare adaptive valve object
AdaptiveValve ad_valve_1(ads_2, 3);

/* MAIN functions declaration */
void MAIN_print_hbridge_status(void);

void setup()
{
  // start serial comunication
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Setup started!");
  // initialization of H-bridge
  h8.begin();
  // initialize external ADC
  if (!ads_2.begin(0b1001001))
  {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
  Serial.println("Setup successfully completedd!");
}

void loop()
{
  // init control variables
  float valve_pos, valve_pos_set, valve_pos_set_raw;
  // set valve tolerance
  const float valve_pos_tolerance = 10.0; // 1.5;
  // get valve set position from reomote controler
  // can be temporarily replaced by input from the terminal
  valve_pos_set_raw = 1140;
  // set min and max range
  valve_pos_set = constrain(valve_pos_set_raw, 1120.0, 1490.0);
  Serial.print("Set position: ");
  Serial.print(valve_pos_set);
  Serial.print(" [mm], ");
  // get valve displacement
  valve_pos = ad_valve_1.compute_position();
  ad_valve_1.print_position();

  // check if valve position is within tolerance
  if (abs(valve_pos_set - valve_pos) >= valve_pos_tolerance)
  {
    // control motor direction
    if (valve_pos_set >= valve_pos)
    {
      // turn motor on in forward direction
      h8.set_pwm_dir(1, 1);
      Serial.println("Motor: ON, direction: UP");
    }
    else
    {
      // turn motor on in reverse direction
      h8.set_pwm_dir(1, 0);
      Serial.println("Motor: ON, direction: DOWN");
    }
  }
  else
  {
    // turn off the motor
    h8.set_pwm_dir(0, 0);
    Serial.println("Valve in position : )");
  }
  // wait for next loop iter
  delay(50);
}

void MAIN_print_hbridge_status(void)
{
  /* Print DIA register */
  Serial.print("EN = ");
  Serial.println(h8.dia_reg_bits.en);

  Serial.print("OT = ");
  Serial.println(h8.dia_reg_bits.ot);

  Serial.print("TV = ");
  Serial.println(h8.dia_reg_bits.tv);

  Serial.print("CL = ");
  Serial.println(h8.dia_reg_bits.cl);

  Serial.print("DIA4 = ");
  Serial.println(h8.dia_reg_bits.dia4);

  Serial.print("DIA3 = ");
  Serial.println(h8.dia_reg_bits.dia3);

  Serial.print("DIA2 = ");
  Serial.println(h8.dia_reg_bits.dia2);

  Serial.print("DIA1 = ");
  Serial.println(h8.dia_reg_bits.dia1);

  /* Print CTRL register */
  Serial.print("CMD = ");
  Serial.println(h8.ctrl_reg_bits.cmd);

  Serial.print("OLDIS = ");
  Serial.println(h8.ctrl_reg_bits.oldis);

  Serial.print("SIN = ");
  Serial.println(h8.ctrl_reg_bits.sin);

  Serial.print("SEN = ");
  Serial.println(h8.ctrl_reg_bits.sen);

  Serial.print("SDIR = ");
  Serial.println(h8.ctrl_reg_bits.sdir);

  Serial.print("SPWM = ");
  Serial.println(h8.ctrl_reg_bits.spwm);

  /* Print REV register */
  Serial.print("Revision = ");
  Serial.println(h8.read_reg(TLE9201_REV_REG));

  Serial.println();
}
