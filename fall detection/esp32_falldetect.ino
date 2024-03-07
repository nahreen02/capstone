/* 
Fall Detection Algorithm - Nahreen Tahmid

*/
#include "head_ped.h"

#define USE_WIRE

#ifndef SENSOR_SDA
#define SENSOR_SDA 6
#endif

#ifndef SENSOR_SCL
#define SENSOR_SCL 7
#endif

#ifndef SENSOR_IRQ
#define SENSOR_IRQ -1
#endif

#define IMU_CS 5

//SensorQMI8658 qmi;
IMUdata acc;
IMUdata gyr;

float maxVal = 0.0;

void setup() {
  fall_setup();
  Serial.println("Setup done ------------");
//   Serial.begin(115200);
//   while (!Serial)
//     ;
// #ifdef USE_WIRE
//   //Using WIRE !!
//   if (!qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, SENSOR_SDA, SENSOR_SCL)) {
//     Serial.println("Failed to find QMI8658 - check your wiring!");
//     while (1) {
//       delay(1000);
//     }
//   }
// #else
//   if (!qmi.begin(IMU_CS)) {
//     Serial.println("Failed to find QMI8658 - check your wiring!");
//     while (1) {
//       delay(1000);
//     }
//   }
// #endif
//   /* Get chip id*/
//   Serial.print("Device ID:");
//   Serial.println(qmi.getChipID(), HEX);

//   qmi.configAccelerometer(SensorQMI8658::ACC_RANGE_16G,SensorQMI8658::ACC_ODR_1000Hz,SensorQMI8658::LPF_MODE_0,true);
//   qmi.configGyroscope( SensorQMI8658::GYR_RANGE_256DPS, SensorQMI8658::GYR_ODR_896_8Hz, SensorQMI8658::LPF_MODE_3,true);

//   qmi.enableGyroscope();
//   qmi.enableAccelerometer();

//   qmi.configPedometer(0x007D, 0x00CC, 0x0066, 0x00C8, 0x14, 0x03, 0, 0x01); //insert pedometer params here

//   qmi.enablePedometer();
//   qmi.dumpCtrlRegister();
//   Serial.println("Read data now...");
}

void loop() {
  while (1) {
    falldect();
  }
}

void falldect() {
  float G;
  int threshold = 9; 
  // read raw data from IMU
  if (qmi.getDataReady()) {
    qmi.getAccelerometer(acc.x, acc.y, acc.z);
    qmi.getGyroscope(gyr.x, gyr.y, gyr.z);

    G = sqrt(sq(acc.x) + sq(acc.y) + sq(acc.z));

    if (G > threshold ) { 
      //Serial.println(G);
      if (moving() == false) {
        Serial.println("Fall Detected");
      }
    } 
    return;
  }
}
