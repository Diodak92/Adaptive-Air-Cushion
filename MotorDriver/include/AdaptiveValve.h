#ifndef __AdaptiveValve__
#define __Adaptivevalve__

#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include <TLE9201.h>

class AdaptiveValve
{
private:
    Adafruit_ADS1015 _ads;  
    uint8_t _ads_i2c_addr;
    uint8_t _ads_channel;
    float _set_position;

public:
   AdaptiveValve(u_int8_t ads_i2c_addr, uint8_t ads_channel, float set_position = 1000)
    {
        _ads_i2c_addr = ads_i2c_addr;
        _ads_channel = ads_channel;
        _set_position = set_position;
    }

    bool begin()
    {
        return _ads.begin(_ads_i2c_addr);
    }

    void print_position(){
        Serial.print("Set position: ");
        Serial.print(_set_position);
        Serial.print(" [mm] \n");
    }

    void set_position(float new_position)
    {
        Serial.print("Old position: ");
        Serial.print(_set_position);
        Serial.print(" [mm] \n");
        _set_position = new_position;
        Serial.print("New position: ");
        Serial.print(_set_position);
        Serial.print(" [mm] \n");
    }

    float get_position()
    {
        return _ads.computeVolts(_ads.readADC_SingleEnded(_ads_channel));
    }
};

#endif