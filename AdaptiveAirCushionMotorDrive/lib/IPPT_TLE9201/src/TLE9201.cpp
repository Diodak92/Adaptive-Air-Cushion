/**
 * @file TLE9201.cpp
 * @author Julian Zyromski
 * @brief Function/ definitions for TLE9201 H-bridge driver.
 * @version 0.1
 * @date 2022-08-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "TLE9201.h"

/**
 * @brief Construct a new TLE9201::TLE9201 object
 * 
 * @param cs_pin - your designated Chip Selcet pin for this particular device
 */
TLE9201::TLE9201(uint8_t cs_pin)
{
    spi_cs_pin = cs_pin;
}

/**
 * @brief Initializes TL9201 object by configuring CS GPIO and SPI parameters.
 *        SPI CLK frequency could be increased if needed (refer to the datasheet).
 * 
 */
void TLE9201::begin()
{
    pinMode(spi_cs_pin, OUTPUT);
    digitalWrite(spi_cs_pin, HIGH);
    SPI.begin();
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
}

/**
 * @brief This function mergers lower level SPI manipulation into a larger function allowing for less
 *        messy register read/write.
 * 
 * @param data_out 
 * @return uint8_t - last shifted register value
 */
uint8_t TLE9201::SPI_transfer(uint8_t data_out)
{
  uint8_t data_in = 0;
  digitalWrite(spi_cs_pin, LOW);
  data_in = SPI.transfer(data_out);
  digitalWrite(spi_cs_pin, HIGH);
  return data_in;
}

/**
 * @brief Reads TL9201 register - weird implementation due to TL9201 specific SPI data exchange
 * 
 * @param reg - register to be read
 * @return uint8_t - register value readout
 */
uint8_t TLE9201::read_reg(uint8_t reg)
{
    uint8_t result = 0;
    if(reg == TLE9201_DIA_REG)
    {
        SPI_transfer(_TLE9201_DIA_REG_READOUT);
        result = SPI_transfer(_TLE9201_DIA_REG_READOUT);
    }
    else if(reg == TLE9201_CTRL_REG)
    {
        SPI_transfer(_TLE9201_CTRL_REG_READOUT);
        result = SPI_transfer(_TLE9201_CTRL_REG_READOUT);
    }
    else if(TLE9201_REV_REG)
    {
        SPI_transfer(_TLE9201_REV_REG_READOUT);
        result = SPI_transfer(_TLE9201_REV_REG_READOUT);
    }
    else
    {
        return 0U;
    }
    return result;
}

/**
 * @brief Writes TL9201 register
 * 
 * @param reg - register to be written address
 * @param val - value to be written to the provided address
 * @return uint8_t - last shifted register value
 */
uint8_t TLE9201::write_reg(uint8_t reg, uint8_t val)
{
    uint8_t result = 0;
    SPI_transfer(reg);
    result = SPI_transfer(val);
    return result;
}

/**
 * @brief Reads DIA and CTRL register of TL9201 and deserializes them into human-readable bitfields.
 *        Register bitfields can be found in apropriate structs in .h file.
 * @note There is no need to call this function when using set_pwm_dir() periodically as it
 *       will automatically call this function.
 * 
 */
void TLE9201::update_status(void)
{
    uint8_t dia_readout = 0;
    dia_readout = read_reg(TLE9201_DIA_REG);
    memcpy(&dia_reg_bits, &dia_readout, 1);

    uint8_t ctrl_readout = 0;
    ctrl_readout = read_reg(TLE9201_CTRL_REG);
    memcpy(&ctrl_reg_bits, &ctrl_readout, 1);
}

/**
 * @brief Sets output state of H-bridge On/Off as well as polarity (direction)
 * @note This function has PWM in it's name to comply with TL9201 datasheet while in fact 
 *       the PWM functionality has to be implemented by the user (in a non-blocking loop or an ISR routine).
 * 
 * @param pwm_on_off - turns the current flow ON when set to 1 and turns it OFF when set to 0
 * @param direction - changes the motor spin direction (possible values are 0 and 1)
 */
void TLE9201::set_pwm_dir(uint8_t pwm_on_off, uint8_t direction)
{
    uint8_t new_ctrl_reg_val = 0;
    update_status();
    ctrl_reg_bits.cmd = 7;
    ctrl_reg_bits.sin = 1;
    ctrl_reg_bits.sen = 1;
    ctrl_reg_bits.spwm = pwm_on_off;
    ctrl_reg_bits.sdir = direction;
    memcpy(&new_ctrl_reg_val, &ctrl_reg_bits, 1);
    write_reg(TLE9201_CTRL_REG, new_ctrl_reg_val);
}

/**
 * @brief Resets the fault codes in DIA register.
 *        Should be called after resolving a failure (like short or open-load condition).
 * 
 */
void TLE9201::reset_dia_reg(void)
{
    SPI_transfer(_TLE9201_RES_DIA);
}

