#include <Arduino.h>
#include <ArduinoJson.h> 
#include <Adafruit_ADS1X15.h>
#include <TLE9201.h>
#include <LoRa.h>
#include <cmath>

// H-bridge object instance
TLE9201 h8(7);
// Analog to digital conventer instance
Adafruit_ADS1115 ads_2;

/* MAIN functions declaration */
void MAIN_print_hbridge_status(void);
/* Compute position function declaration */
float compute_position(int16_t);

void setup() {
  // start serial comunication
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Setup started!");
  // initialization of H-bridge
  h8.begin();
  // initialize external ADC
  if (!ads_2.begin(0b1001001)) {
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
  valve_pos = compute_position(ads_2.readADC_SingleEnded(3));
  Serial.print("position: ");
  Serial.print(valve_pos);
  Serial.println(" [mm]");
  //Serial.print(ads_2.readADC_SingleEnded(3));

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

float compute_position(int16_t adc_counts)
{ 
  // voltage variables [V]
  float u, u_min = 0.0, u_max = 3.3;
  // angular position variables [deg] - entire range of potentiometer 0 - 3600 deg
  // valve extreme positions: 1140 deg (open), 1470 deg (closed)
  float ang_pos, ang_pos_min = 0.0, ang_pos_max = 3600.0, ang_pos_valve_min = 1140.0, ang_pos_valve_max = 1470.0;
  // distance variables in [mm]
  float distance, gear_diameter = 22.0;
  // convert counts into voltage
  u = ads_2.computeVolts(adc_counts); // replace ads_2 with class instance
  // calculate angular position;
  ang_pos = (u-u_min) *  (ang_pos_max-ang_pos_min) / (u_max-u_min) + ang_pos_min;
  //Serial.print(ang_pos);
  // compute absolute distance
  distance = (ang_pos / 360.0) * M_PI * gear_diameter; 
  return ang_pos; //distance;
}
