/**
 * @file TLE9201.h
 * @author Julian Zyromski
 * @brief Function/Class prototypes for TLE9201 H-bridge driver.
 * @version 0.1
 * @date 2022-08-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __TLE9201_H__
#define __TLE9201_H__

#include <Arduino.h>
#include <SPI.h>

#define TLE9201_CTRL_REG          0x01
#define _TLE9201_CTRL_REG_READOUT 0x60

#define TLE9201_DIA_REG           0x00
#define _TLE9201_DIA_REG_READOUT  0x00
#define _TLE9201_RES_DIA          0x80

#define TLE9201_REV_REG           0x20
#define _TLE9201_REV_REG_READOUT  0x20

/**
 * @brief this struct holds DIA_REG value represented as a single bits (check TLE datasheet)
 *        it has to be in reverse order compared to datasheet (a matter of correct endiannes).
 *        Bitfields are used so that memcpy() can be utilized to populate the struct.
 * 
 */
struct dia_register
{
    uint8_t dia1 : 1;
    uint8_t dia2 : 1;
    uint8_t dia3 : 1;
    uint8_t dia4 : 1;
    uint8_t cl : 1;
    uint8_t tv : 1;
    uint8_t ot : 1;
    uint8_t en : 1;
}__attribute__((packed));

/**
 * @brief this struct holds CTRL_REG value represented as a single bits (check TLE datasheet)
 *        it has to be in reverse order compared to datasheet (a matter of correct endiannes).
 *        Bitfields are used so that memcpy() can be utilized to populate the struct.
 * 
 */
struct ctrl_register
{
    uint8_t spwm : 1;
    uint8_t sdir : 1;
    uint8_t sen : 1;
    uint8_t sin : 1;
    uint8_t oldis : 1;
    uint8_t cmd : 3;
}__attribute__((packed));


/**
 * @brief This class implements all functions needed to opearate TL9201 H-bridge and allows for 
 *        easy SPI bus sharing between multiple devices. 
 * 
 */
class TLE9201
{
  public:
    dia_register dia_reg_bits;
    ctrl_register ctrl_reg_bits;
    uint8_t spi_cs_pin;
    TLE9201();
    void begin(uint8_t cs_pin);
    uint8_t read_reg(uint8_t reg);
    uint8_t write_reg(uint8_t reg, uint8_t val);
    void update_status(void);
    void set_pwm_dir(uint8_t pwm_on_off, uint8_t direction);
    void reset_dia_reg(void);

  private:
    uint8_t SPI_transfer(uint8_t data_out);
	
};

#endif