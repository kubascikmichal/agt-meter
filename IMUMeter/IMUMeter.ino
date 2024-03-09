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

#define DEBUG
//#define RELEASE
#define DATA_FILENAME "data.csv"
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
    double acc[3] = {myIMU.readFloatAccelX(), myIMU.readFloatAccelY(),myIMU.readFloatAccelZ()};
#ifdef DEBUG
    Serial.print("\nAccelerometer:\n");
    Serial.print(" X1 = ");
    Serial.println(acc[0], 4);
    Serial.print(" Y1 = ");
    Serial.println(acc[1], 4);
    Serial.print(" Z1 = ");
    Serial.println(acc[2], 4);
#endif
    //Gyroscope
    double gyro[3] = {myIMU.readFloatGyroX(), myIMU.readFloatGyroY(),myIMU.readFloatGyroZ()};
#ifdef DEBUG
    Serial.print("\nGyroscope:\n");
    Serial.print(" X1 = ");
    Serial.println(gyro[0], 4);
    Serial.print(" Y1 = ");
    Serial.println(gyro[1], 4);
    Serial.print(" Z1 = ");
    Serial.println(gyro[2], 4);
#endif
    //Thermometer
    double temp = myIMU.readTempC();
#ifdef DEBUG
    Serial.print("\nThermometer:\n");
    Serial.print(" Degrees C1 = ");
    Serial.println(temp, 4);
#endif
    char buff[256];
    memset(buff,0,256);
    sprintf(buff,"%0.4f,%0.4f,%0.4f,%0.4f,%0.4f,%0.4f,%0.4f,",acc[0],acc[1],acc[2],gyro[0],gyro[1],gyro[2],temp);
    myFile = SD.open("data.csv",FILE_WRITE);
    if(myFile){
      
    } else {
      Serial.println("ERROR writing to file");
    }
#ifdef DEBUG
    Serial.println(buff);
#endif
    delay(1000);
}
