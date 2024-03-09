/*****************************************************************************/
//  HighLevelExample.ino
//  Hardware:      Grove - 6-Axis Accelerometer&Gyroscope
//	Arduino IDE:   Arduino-1.65
//	Author:	       Lambor
//	Date: 	       Oct,2015
//	Version:       v1.0
//
//  Modified by:
//  Data:
//  Description:
//
//	by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
/*******************************************************************************/

#include "LSM6DS3.h"
#include "Wire.h"
#include <SPI.h>
#include <SD.h>

//#define DEBUG
#define RELEASE
#define DATA_FILENAME "data3.csv"
#define FS 100

File myFile;

//Create a instance of class LSM6DS3
LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
#ifdef DEBUG
    while (!Serial);
#endif
    //Call .begin() to configure the IMUs
    if (myIMU.begin() != 0) {
        Serial.println("Device error");
    } else {
        Serial.println("Device OK!");
    }

    while(!SD.begin(D2));
}

void loop() {
    //Accelerometer
    double acc[10][3];
    double gyro[10][3];
    double temp[10];
    for(int i = 0; i < 10; i++){
      delay(50);
      acc[i][0] = myIMU.readFloatAccelX();
      acc[i][1] = myIMU.readFloatAccelY();
      acc[i][2] = myIMU.readFloatAccelZ();
#ifdef DEBUG
      Serial.print("\nAccelerometer:\n");
      Serial.print(" X1 = ");
      Serial.println(acc[i][0], 4);
      Serial.print(" Y1 = ");
      Serial.println(acc[i][1], 4);
      Serial.print(" Z1 = ");
      Serial.println(acc[i][2], 4);
#endif
    //Gyroscope
      gyro[i][0] = myIMU.readFloatGyroX();
      gyro[i][1] = myIMU.readFloatGyroY();
      gyro[i][2] = myIMU.readFloatGyroZ();
#ifdef DEBUG
      Serial.print("\nGyroscope:\n");
      Serial.print(" X1 = ");
      Serial.println(gyro[i][0], 4);
      Serial.print(" Y1 = ");
      Serial.println(gyro[i][1], 4);
      Serial.print(" Z1 = ");
      Serial.println(gyro[i][2], 4);
#endif
    //Thermometer
      temp[i] = myIMU.readTempC();
#ifdef DEBUG
      Serial.print("\nThermometer:\n");
      Serial.print(" Degrees C1 = ");
      Serial.println(temp[i], 4);
#endif 
    }
    unsigned long start_SD = millis();
    myFile = SD.open(DATA_FILENAME,FILE_WRITE);
    unsigned long start_saving = millis();
    for(int i = 0;i<10;i++){
      char buff[256];
      memset(buff,0,256);
      sprintf(buff,"%0.4f,%0.4f,%0.4f,%0.4f,%0.4f,%0.4f,%0.4f",acc[i][0],acc[i][1],acc[i][2],gyro[i][0],gyro[i][1],gyro[i][2],temp[i]);
      if(myFile){
        myFile.println(buff);
      } else {
        Serial.println("ERROR writing to file");
      } 
    }
    unsigned long start_closing = millis();
    myFile.close();
    unsigned long stop_SD = millis();
#ifdef DEBUG
    Serial.print("saving duration ");
    Serial.println(stop_SD-start_SD);
    Serial.print("opening duration ");
    Serial.println(start_saving-start_SD);
    Serial.print("printing duration ");
    Serial.println(start_closing-start_saving);
    Serial.print("closing duration ");
    Serial.println(stop_SD-start_closing);
#endif
#ifdef DEBUG
    Serial.println(buff);
#endif
}
