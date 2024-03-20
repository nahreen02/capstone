/**
 *
 * @license MIT License
 *
 * Copyright (c) 2022 lewis he
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file      QMI8658_GetDataExample.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @date      2022-10-16
 *
 */

#include "head_ped.h"


#define USE_WIRE

#ifndef SENSOR_SDA
#define SENSOR_SDA  6
#endif

#ifndef SENSOR_SCL
#define SENSOR_SCL  7
#endif

#ifndef SENSOR_IRQ
#define SENSOR_IRQ  -1
#endif

#define IMU_CS                      5

SensorQMI8658 qmi;

void fall_setup()
{
    Serial.begin(115200);
    while (!Serial);
#ifdef USE_WIRE
    //Using WIRE !!
    if (!qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, SENSOR_SDA, SENSOR_SCL)) {
        Serial.println("Failed to find QMI8658 - check your wiring!");
        while (1) {
            delay(1000);
        }
    }
#else
    if (!qmi.begin(IMU_CS)) {
        Serial.println("Failed to find QMI8658 - check your wiring!");
        while (1) {
            delay(1000);
        }
    }
#endif
    /* Get chip id*/
    Serial.print("Device ID:");
    Serial.println(qmi.getChipID(), HEX);
    // FALL CONFIG
    qmi.configAccelerometer(SensorQMI8658::ACC_RANGE_16G,SensorQMI8658::ACC_ODR_1000Hz,SensorQMI8658::LPF_MODE_0,true);
    qmi.configGyroscope(SensorQMI8658::GYR_RANGE_256DPS, SensorQMI8658::GYR_ODR_896_8Hz, SensorQMI8658::LPF_MODE_3,true);

    qmi.enableGyroscope();
    qmi.enableAccelerometer();

    qmi.configPedometer(0x007D, 0x00CC, 0x0066, 0x00C8, 0x14, 0x03, 0, 0x01); //insert pedometer params here

    qmi.enablePedometer();
    qmi.dumpCtrlRegister();
    Serial.println("Read data now...");
}


void pedo_loop()
{
    // Get IMU status
    uint8_t status =  qmi.getStatusRegister();
    Serial.printf("STATUS:0x%x BIN:", status);
    Serial.println(status, BIN);

    if (status & SensorQMI8658::EVENT_TAP_MOTION) {
        Serial.println("Detected TAP event");
        qmi.getTapStatus();
    }
    if (status & SensorQMI8658::EVENT_WOM_MOTION) {
        Serial.println("Detected Wom event");
    }
    if (status & SensorQMI8658::EVENT_PEDOMETER_MOTION) {
        Serial.println("Detected Pedometer event");
        // uint32_t val = qmi.getPedometerCounter();
        // Serial.println(val);
    }
    if (status & SensorQMI8658::EVENT_ANY_MOTION) {
        Serial.println("Detected Any Motion event");
    }
    if (status & SensorQMI8658::EVENT_NO_MOTION) {
        Serial.println("Detected No Motion event");
    }
    if (status & SensorQMI8658::EVENT_SIGNIFICANT_MOTION) {
        Serial.println("Detected Significant Motion event");
    }

    delay(500);
}


bool moving() {
    uint8_t status =  qmi.getStatusRegister();
    if ((status & SensorQMI8658::EVENT_PEDOMETER_MOTION)) {
        uint32_t prev = qmi.getPedometerCounter();
        //Serial.println(prev);
        delay(10000); // wait for movement 
        uint32_t val = qmi.getPedometerCounter();
        //Serial.println(val);

        if (val > prev) {
            // Serial.print(val);
            // Serial.print(" ");
            // Serial.println(prev);
            return true;
        } else {
            return false;
        }

    }
}

int numSteps() {
  uint8_t status =  qmi.getStatusRegister();
  if (status & SensorQMI8658::EVENT_PEDOMETER_MOTION) {
    Serial.println("Detected Pedometer event");
    int val = qmi.getPedometerCounter();
    return val; 
  }

}

