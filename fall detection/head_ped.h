// head_ped.h

#ifndef HEAD_PED_H
#define HEAD_PED_H

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "C:/Users/14384/Documents/Arduino/esp32_falldetect/SensorLib/src/SensorQMI8658.hpp"

extern SensorQMI8658 qmi;


// Function declarations
bool moving();
int numSteps(); 
void fall_setup(); 

#endif
