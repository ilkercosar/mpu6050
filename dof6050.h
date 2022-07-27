#ifndef __dof6050__
#define __dof6050__

typedef enum{

    mpu6050Adress = 0x68,
    smplrtDivReg = 0x19,
    accelXoutHReg = 0x3B,
    accelConfigReg = 0x1C,
    gyroConfigReg = 0x1B,
    tempOutHReg = 0x41,
    gyroXoutHReg = 0x43,
    pwrMgmt1Reg = 0x6B,
    whoAmIReg = 0x75,
    configReg = 0x1A,

}registerOfMpu;

typedef struct{

    float aX,aY,aZ;
    float gX,gY,gZ;
    float gyroLSBToDegSec;
    float accLSBToDegSec;
    float aXC,aYC,aZC;
    float gXC,gYC,gZC;

    int16_t rawData[7];

}IMU;

void mpuInit(IMU *imu, int gyroConfigNum, int accConfigNum);
unsigned char setAccConfig(IMU *imu, int configNum);
unsigned char setGyroConfig(IMU *imu, int configNum);
static float wrap(float angle, float limit);
unsigned char writeData(unsigned char reg, unsigned char data);
unsigned char readData(unsigned char reg);
void gyroOffSets(IMU *imu, float x, float y, float z);
void accOffSets(IMU *imu, float x, float y, float z);
void calcOffSets(IMU *imu, bool cG, bool cA);
void getIMUDatas(void);
IMU sendDatas(float aX, float aY, float aZ, float gX, float gY, float gZ);

#endif