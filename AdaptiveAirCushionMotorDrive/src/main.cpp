#include <Arduino.h>
#include <ArduinoJson.h> 
#include <Adafruit_ADS1X15.h>
#include <TLE9201.h>
#include <LoRa.h>

// H-bridge object instance
TLE9201 h1(0);
// Analog to digital conventer instance
Adafruit_ADS1115 ads_1;

/* MAIN functions declaration */
void MAIN_print_hbridge_status(void);
/* Compute position function declaration */
float compute_position(int16_t);

void setup() {
  // start serial comunication
  Serial.begin(115200);
  while(!Serial);

  Serial.println("Setup started!");
  // initialize external ADC
  if (!ads_1.begin(0b1001000)) {
    Serial.println("Failed to initialize ADS.");
  while (1);
  Serial.println("Setup successfully completedd!");
  }
}

void loop() {
  // variables for storing ADC values
  int16_t adc1_ch0;
  // h1.update_status(); /* No need to call that if set_pwm_dir() is used */  
  // MAIN_print_hbridge_status();
  
  // read data from ADC2
  adc1_ch0 = ads_1.readADC_SingleEnded(0);
  Serial.print("ADC1_CH0: ");
  Serial.println(adc1_ch0);
  // start H-bridge 
  h1.set_pwm_dir(1, 0);
  delay(1500);
  h1.set_pwm_dir(1, 1);
  delay(1500);

}

void MAIN_print_hbridge_status(void)
{
  /* Print DIA register */
  Serial.print("EN = ");
  Serial.println(h1.dia_reg_bits.en);

  Serial.print("OT = ");
  Serial.println(h1.dia_reg_bits.ot);

  Serial.print("TV = ");
  Serial.println(h1.dia_reg_bits.tv);

  Serial.print("CL = ");
  Serial.println(h1.dia_reg_bits.cl);

  Serial.print("DIA4 = ");
  Serial.println(h1.dia_reg_bits.dia4);

  Serial.print("DIA3 = ");
  Serial.println(h1.dia_reg_bits.dia3);

  Serial.print("DIA2 = ");
  Serial.println(h1.dia_reg_bits.dia2);

  Serial.print("DIA1 = ");
  Serial.println(h1.dia_reg_bits.dia1);

  /* Print CTRL register */
  Serial.print("CMD = ");
  Serial.println(h1.ctrl_reg_bits.cmd);

  Serial.print("OLDIS = ");
  Serial.println(h1.ctrl_reg_bits.oldis);

  Serial.print("SIN = ");
  Serial.println(h1.ctrl_reg_bits.sin);

  Serial.print("SEN = ");
  Serial.println(h1.ctrl_reg_bits.sen);

  Serial.print("SDIR = ");
  Serial.println(h1.ctrl_reg_bits.sdir);

  Serial.print("SPWM = ");
  Serial.println(h1.ctrl_reg_bits.spwm);

  /* Print REV register */
  Serial.print("Revision = ");
  Serial.println(h1.read_reg(TLE9201_REV_REG));

  Serial.println();
}

float compute_position(int16_t)
{
  return 1.0;
}
