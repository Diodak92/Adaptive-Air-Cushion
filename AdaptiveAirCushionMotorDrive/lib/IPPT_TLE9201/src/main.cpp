#include <Arduino.h>
#include "TLE9201.h"

/* H-bridge object instances creation */
TLE9201 h1(0);
TLE9201 h2(1);
TLE9201 h3(2);
TLE9201 h4(3);
TLE9201 h5(4);
TLE9201 h6(5);
TLE9201 h7(6);
TLE9201 h8(7);

/* MAIN functions declarations */
void MAIN_print_hbridge_status(void);



void setup()
{
  Serial.begin(115200);
  delay(2000);
  
  /* H-bridge object instances initialization */
  h1.begin();
  h2.begin();
  h3.begin();
  h4.begin();
  h5.begin();
  h6.begin();
  h7.begin();
  h8.begin();
}

void loop() 
{ 
  h1.update_status(); /* No need to call that if set_pwm_dir() is used */
  h2.update_status(); /* No need to call that if set_pwm_dir() is used */
  h8.update_status(); /* No need to call that if set_pwm_dir() is used */
  
  MAIN_print_hbridge_status();
  
  h1.set_pwm_dir(1, 0);
  h2.set_pwm_dir(1, 0);
  h8.set_pwm_dir(1, 0);
  
  delay(1500);
  
  h1.set_pwm_dir(1, 1);
  h2.set_pwm_dir(1, 1);
  h8.set_pwm_dir(1, 1);

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
