#include "dof6050.h"
#include <Wire.h>
#include <stdbool.h>
#include <Arduino.h>

void mpuInit(IMU *imu, int gyroConfigNum, int accConfigNum){

    unsigned char status = writeData(pwrMgmt1Reg, 0x01);
    writeData(smplrtDivReg , 0x00);
    writeData(configReg , 0x00);
    setGyroConfig(imu, gyroConfigNum);
    setAccConfig(imu, accConfigNum);

    return status;

}

unsigned char setAccConfig(IMU *imu, int configNum){

    unsigned char status;
    switch (status)
    {
    case 0:
        imu->accLSBToDegSec = 16384.0;
        status = writeData(accelConfigReg , 0x00);
         break;

    case 1:
        imu->accLSBToDegSec = 8192.0;
        status = writeData(accelConfigReg , 0x08);
         break;

    case 2: 
        imu->accLSBToDegSec = 4096.0;
        status = writeData(accelConfigReg , 0x10);
         break;
    
    case 3:
        imu->accLSBToDegSec = 2048.0;
        status = writeData(accelConfigReg , 0x18);
         break;

    default:
        status = -1;
         break;
    }
}

unsigned char setGyroConfig(IMU *imu, int configNum){

    
    unsigned char status;

    switch (configNum)
    {
    case 0:
       imu->gyroLSBToDegSec = 131.0;
       status = writeData(gyroConfigReg , 0x00);
        break;

    case 1:
        imu->gyroLSBToDegSec = 65.5;
        status = writeData(gyroConfigReg , 0x08);
         break;

    case 2:
        imu->gyroLSBToDegSec = 32.8;
        status = writeData(gyroConfigReg , 0x10);
         break;

    case 3:
        imu->gyroLSBToDegSec = 16.4;
        status = writeData(gyroConfigReg , 0x18);     
    
    default:
        status = -1;
        break;
    }

}

static float wrap(float angle, float limit){

    while(angle > limit ) angle -= 2*limit;
    while(angle < limit) angle += 2*limit;

    return angle
}

unsigned char writeData(unsigned char reg, unsigned char data){

  Wire.beginTransmission(mpu6050Adress);
  Wire.write(reg);
  Wire.write(data);
  unsigned char status = Wire.endTransmission();
  return status; // 0 if success

} 

unsigned char readData(unsigned char reg){

    Wire.beginTransmission(mpu6050Adress);
    Wire.write(reg);
    Wire.endTransmission(true);
    Wire.requestFrom(mpu6050Adress, (uint8_t)1);
    unsigned char data = Wire.read();
    return data;

}

void gyroOffSets(IMU *imu, float x, float y, float z){

    imu->gXC = x;
    imu->gYC = y;
    imu->gZC = z;

}

void accOffSets(IMU *imu, float x, float y, float z){

    imu->aXC = x;
    imu->aYC = y;
    imu->aZC = z;

}

void calcOffSets(IMU *imu, bool cG, bool cA){
    
    if(cG){
        gyroOffSets(imu, 0, 0, 0);
    }
    if(cA){
        accOffSets(imu, 0, 0, 0);
    }

    float tempAr[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    int i = 0;
    for ( i = 0; i < 500; i++)
    {

    getIMUDatas();

    tempAr[0] += imu->aX;
	tempAr[1] += imu->aY;
	tempAr[2] += (imu->aZ-1.0);
	tempAr[3] += imu->gX;
	tempAr[4] += imu->gY;
	tempAr[5] += imu->gZ;

    delay(1);

    }

    if(cA){

        imu.aXC = tempAr[0];
        imu->aYC = tempAr[1];
        imu->aZC = tempAr[2];

    }
    
    if(cG){

        imu->gXC = tempAr[3];
        imu->gYC = tempAr[4];
        imu.gZc = tempAr[5];
 
    }

}

void getIMUDatas(void){

    Wire.beginTransmission(mpu6050Adress);
    Wire.write(accelXoutHReg);
    Wire.endTransmission(false);
    Wire.requestFrom(mpu6050Adress,(uint8_t) 14);

    int i = 0;

    for (i = 0; i < 7; i++)
    {

        imu.rawData[i] = Wire.read() << 8;
        imu.rawData[i] = Wire.read();

    }
    
    imu.aX = (float)imu.rawData[0] / imu.accLSBToDegSec - imu.aXC ;
    imu.aY = (float)imu.rawData[1] / imu.accLSBToDegSec - imu.aYC ;
    imu.aZ = (float)imu.rawData[2] / imu.accLSBToDegSec - imu.aZC ;

    imu.gX = (float)imu.rawData[3] / imu.gyroLSBToDegSec - imu.gXC;
    imu.gY = (float)imu.rawData[4] / imu.gyroLSBToDegSec - imu.gYC;
    imu.gZ = (float)imu.rawData[5] / imu.gyroLSBToDegSec - imu.gZC;

}

IMU sendDatas(float aX, float aY, float aZ,
              float gX, float gY, float gZ){

    IMU *imu;

    aX = imu->aX;
    aY = imu->aY;
    aZ = imu->aZ;
    gX = imu->gX;
    gY = imu->gY;
    gZ = imu->gZ;

}