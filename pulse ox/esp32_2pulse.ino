/*!
 * @file SPO2.ino
 * @brief Display heart-rate and SPO2 on serial in real-time, normal SPO2 is within 95~100
 * @n Try to fix the sensor on your finger in using to avoid the effect of pressure change on data output.
 * @n This library supports mainboards: ESP8266, FireBeetle-M0, UNO, ESP32, Leonardo, Mega2560
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [YeHangYu](hangyu.ye@dfrobot.com)
 * @version  V0.1
 * @date  2020-05-29
 * @url https://github.com/DFRobot/DFRobot_MAX30102
 */

#include <DFRobot_MAX30102.h>
#include <Wire.h>
#include "head_ped.h"

#define SDA_PIN_1 21  // custom SDA pin
#define SCL_PIN_1 33  // custom SCL pin

#define SDA_PIN_2 17
#define SCL_PIN_2 18

//TwoWire Wire1 = TwoWire(0);
TwoWire Wire2 = TwoWire(1);

DFRobot_MAX30102 particleSensor1;
DFRobot_MAX30102 particleSensor2;

struct PulseVals {
  int HR;
  int SPO2;
  bool LowFlag;
};

void setup() {
  Serial.begin(115200);
  //Init serial
  Wire.begin(SDA_PIN_1, SCL_PIN_1);
  while (!particleSensor1.begin(&Wire)) {
    Serial.println("MAX30102-1 was not found");
    delay(1000);
  }

  particleSensor1.sensorConfiguration(/*ledBrightness=*/50, /*sampleAverage=*/SAMPLEAVG_4,
                                      /*ledMode=*/MODE_MULTILED, /*sampleRate=*/SAMPLERATE_100,
                                      /*pulseWidth=*/PULSEWIDTH_411, /*adcRange=*/ADCRANGE_16384);


  delay(100);
  Wire2.begin(SDA_PIN_2, SCL_PIN_2);
  while (!particleSensor2.begin(&Wire2)) {
    Serial.println("MAX30102-2 was not found");
    delay(1000);
  }

  particleSensor2.sensorConfiguration(/*ledBrightness=*/50, /*sampleAverage=*/SAMPLEAVG_4,
                                      /*ledMode=*/MODE_MULTILED, /*sampleRate=*/SAMPLERATE_100,
                                      /*pulseWidth=*/PULSEWIDTH_411, /*adcRange=*/ADCRANGE_16384);
}

int32_t SPO2_1, SPO2_2;                     //SPO2
int8_t SPO2Valid_1, SPO2Valid_2;            //Flag to display if SPO2 calculation is valid
int32_t heartRate_1, heartRate_2;           //Heart-rate
int8_t heartRateValid_1, heartRateValid_2;  //Flag to display if heart-rate calculation is valid

void loop() {
  PulseVals result;
  result = getPulse();
  Serial.print(F("Avg HR = "));
  Serial.println(result.HR);

  Serial.print(F("Avg SPO2 = "));
  Serial.println(result.SPO2);

  Serial.print(F("Low Flag = "));
  Serial.println(result.LowFlag);

  delay(1000);
}

PulseVals getPulse() {
  // Serial.print(F("SENSOR 1 -->"));
  // Serial.print(F("heartRate="));
  // Serial.print(heartRate_1, DEC);
  // int HR1 = (int)(heartRate_1);
  // Serial.print(F("; SPO2="));
  // Serial.println(SPO2_1, DEC);
  // int SPO1 = (int)(SPO2_1);

  // //particleSensor2.heartrateAndOxygenSaturation(/**SPO2=*/&SPO2_2, /**SPO2Valid=*/&SPO2Valid_2, /**heartRate=*/&heartRate_2, /**heartRateValid=*/&heartRateValid_2);
  // Serial.print(F("SENSOR 2 -->"));
  // Serial.print(F("heartRate="));
  // Serial.print(heartRate_2, DEC);
  // int HR2 = (int)(heartRate_2);
  // Serial.print(F("; SPO2="));
  // Serial.println(SPO2_2, DEC);
  // int SPO2 = (int)(SPO2_2);

  // int HRT = ((HR1 + HR2) / 2);
  // int SPO_T = ((SPO1 + SPO2) / 2);

  int HRT = 0;
  int SPO_T = 0;
  int hr_cnt = 0;
  int spo_cnt = 0;
  int low_cnt = 0;

  // 4-6 seconds loop for measuring
  for (int i = 0; i < 2; i++) {

    // Pulse Ox 1

    PowerOn_1();
    // Serial.println("Power ON - 1");
    particleSensor1.heartrateAndOxygenSaturation(/**SPO2=*/&SPO2_1, /**SPO2Valid=*/&SPO2Valid_1, /**heartRate=*/&heartRate_1, /**heartRateValid=*/&heartRateValid_1);
    // Serial.print("HR_1: ");
    // Serial.print(heartRate_1);
    if ((heartRate_1 > 60) && (heartRate_1 < 150)) {
      HRT += heartRate_1;
      hr_cnt += 1;
    }
    // Serial.print("  SPO2_1: ");
    // Serial.println(SPO2_1);
    if ((SPO2_1 > 93) && (SPO2_1 < 101)) {
      SPO_T += SPO2_1;
      spo_cnt += 1;
    }
    // Serial.println("Power OFF - 1");
    PowerOff_1();
    delay(200);

    // Pulse Ox 2

    PowerOn_2();
    // Serial.println("Power ON - 2");
    particleSensor2.heartrateAndOxygenSaturation(/**SPO2=*/&SPO2_2, /**SPO2Valid=*/&SPO2Valid_2, /**heartRate=*/&heartRate_2, /**heartRateValid=*/&heartRateValid_2);
    // Serial.print("HR_2: ");
    // Serial.print(heartRate_2);
    if ((heartRate_2 > 60) && (heartRate_2 < 150)) {
      HRT += heartRate_2;
      hr_cnt += 1;
    }
    // Serial.print("  SPO2_2: ");
    // Serial.println(SPO2_2);
    if ((SPO2_2 > 93) && (SPO2_2 < 101)) {
      SPO_T += SPO2_2;
      spo_cnt += 1;
    }
    // Serial.println("Power OFF - 1");
    PowerOff_2();

    if (((SPO2_1 < 90) && (SPO2_2 < 90)) || ((heartRate_1 < 40) && (heartRate_2 < 40))) {
      low_cnt += 1;
    }
    delay(200);  // Delay between measurements
  }

  PulseVals values;  // define struct

  if (hr_cnt != 0) {
    HRT = (HRT / hr_cnt);
  } else {
    HRT = NULL;
  }
  values.HR = HRT;

  if (spo_cnt != 0) {
    SPO_T = (SPO_T / spo_cnt);
  } else {
    SPO_T = NULL;
  }
  values.SPO2 = SPO_T;

  if (low_cnt > 1) {
    values.LowFlag = true;
  } else {
    values.LowFlag = false;
  }

  return values;
}

void PowerOn_1() {
  particleSensor1.wakeUp();
}

void PowerOn_2() {
  particleSensor2.wakeUp();
}

void PowerOff_1() {
  particleSensor1.shutDown();
}

void PowerOff_2() {
  particleSensor2.shutDown();
}

PulseVals getResting() {
  PulseVals rest;
  if (moving() == false) {
    rest = getPulse();
  }
  return rest;
}
