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

void loop() {
  // variables for storing ADC values
  //int16_t adc2_ch3;
  // read data from ADC2
  //adc2_ch3 = ads_2.readADC_SingleEnded(3); // min 0, max 17607
  //Serial.print("ADC2_CH3: ");
  //Serial.println(adc2_ch3);
  // displacement
  float displacement;
  displacement = compute_position(ads_2.readADC_SingleEnded(3));
  Serial.print("Displacement: ");
  Serial.print(displacement);
  Serial.println(" [mm]");

  // start H-bridge -- check status when battery is unpluged
  //h8.set_pwm_dir(1, 0); 
  //delay(1500);
  //h8.set_pwm_dir(0, 0);
  delay(1500); 
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
  // angular position variables [deg]
  float ang_pos, ang_pos_min = 0.0, ang_pos_max = 270.0;
  // distance variables in [mm]
  float distance, gear_diameter = 22.0;
  // convert counts into voltage
  //u = ads_2.computeVolts(adc_counts); // replace ads_2 with class instance
  u = ads_2.computeVolts(adc_counts); // replace ads_2 with class instance
  // map voltage to angular position
  ang_pos = map(u, u_min, u_max, ang_pos_min, ang_pos_max);
  // compute absolute distance
  distance = (ang_pos / ang_pos_max) * M_PI * gear_diameter; 
  return distance;
}
