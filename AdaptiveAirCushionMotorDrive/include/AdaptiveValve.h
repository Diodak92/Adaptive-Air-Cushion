#ifndef __AdaptiveValve__
#define __Adaptivevalve__

#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include <TLE9201.h>

class AdaptiveValve
{
private:
    float _set_position;

public:
    AdaptiveValve(float set_position = 1000)
    {
        _set_position = set_position;
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
};

#endif