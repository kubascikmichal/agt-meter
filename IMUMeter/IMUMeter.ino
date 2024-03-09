/*****************************************************************************/
//  IMUmeter.ino
//  Hardware:      XIAO BLE Sense
//	Author:	       Michal Kubascikk
//	Date: 	       March,2024
//	Version:       v1.0
//
/*******************************************************************************/

#include "LSM6DS3.h"
#include "Wire.h"
#include <SPI.h>
#include <SD.h>

//#define DEBUG
#define RELEASE
//#define TIMING_ANALYSIS

#define DATA_FILENAME "data_final.csv"
//#define FS 100 //not used

File myFile;
LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A

void setup() {
  Serial.begin(9600);
#ifdef DEBUG
  while (!Serial);
#endif
  if (myIMU.begin() != 0) {
      Serial.println("Device error");
  } else {
      Serial.println("Device OK!");
  }

  while(!SD.begin(D2));
}

void loop() {
    
  double acc[10][3];
  double gyro[10][3];
  double temp[10];
  
  for(int i = 0; i < 10; i++){
    if(i>0)
      delay(50);
    
    //Accelerometer
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
#ifdef TIMING_ANALYSIS
  unsigned long start_SD = millis();
#endif
  myFile = SD.open(DATA_FILENAME,FILE_WRITE);
#ifdef TIMING_ANALYSIS
  unsigned long start_saving = millis();
#endif
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
#ifdef TIMING_ANALYSIS
  unsigned long start_closing = millis();
#endif
  myFile.close();
#ifdef TIMING_ANALYSIS
  unsigned long stop_SD = millis();
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
