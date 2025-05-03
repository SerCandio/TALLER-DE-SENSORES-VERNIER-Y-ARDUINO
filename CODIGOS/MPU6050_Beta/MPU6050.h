#ifndef MPU6050_h
#define MPU6050_h

#include "Arduino.h"
#include "Wire.h"

//MPU6050 Slave Address
#define MPU_6050_SLAVE_ADDR             0x68


/*
  MPU6050 Register Map Addresses
*/
#define MPU_6050_REG_MAP_SMPLRT_DIV     0x19
#define MPU_6050_REG_MAP_CONFIG         0x1A
#define MPU_6050_REG_MAP_GYRO_CONFIG    0x1B
#define MPU_6050_REG_MAP_ACCEL_CONFIG   0x1C  

//Power Managment Register: SLEEP 
#define MPU_6050_REG_MAP_PWR_MGMT_1     0x6B   

//WHO-AM-I Register to Test connection
#define MPU_6050_REG_MAP_WHO_AM_I       0x75

//MEASUREMENTS pointer register valur¿e: RPA
#define MPU_6050_REG_MAP_ACCEL_XOUT_H   0x3B

/*
  MPU6050 Register CUSTOM VALUES
*/
#define MPU_6050_GYRO_CONFIG_FSEL_250_DEG_SEC    0x00
#define MPU_6050_GYRO_CONFIG_FSEL_500_DEG_SEC    (0x01)<<3
#define MPU_6050_GYRO_CONFIG_FSEL_1000_DEG_SEC   (0x02)<<3
#define MPU_6050_GYRO_CONFIG_FSEL_2000_DEG_SEC   (0x03)<<3

#define MPU_6050_ACCEL_CONFIG_AFS_SEL_2G          (0x00)
#define MPU_6050_ACCEL_CONFIG_AFS_SEL_4G          (0x01)<<3
#define MPU_6050_ACCEL_CONFIG_AFS_SEL_8G          (0x02)<<3
#define MPU_6050_ACCEL_CONFIG_AFS_SEL_16G         (0x03)<<3

/*
  Estructuras de datos
*/
typedef struct{
  uint8_t ACCEL_XOUT_H; //Medidas de la Aceleracion Eje X
  uint8_t ACCEL_XOUT_L;
  uint8_t ACCEL_YOUT_H;//Medidas de la Aceleracion Eje Y
  uint8_t ACCEL_YOUT_L;
  uint8_t ACCEL_ZOUT_H; //Medidas de la Aceleracion Eje Z
  uint8_t ACCEL_ZOUT_L;

  uint8_t TEMP_OUT_H; //Medidas de Temperatura
  uint8_t TEMP_OUT_L;

  uint8_t GYRO_XOUT_H; //Medidas del Gyrometro Eje X
  uint8_t GYRO_XOUT_L;
  uint8_t GYRO_YOUT_H;//Medidas del Gyrometro Eje Y
  uint8_t GYRO_YOUT_L;
  uint8_t GYRO_ZOUT_H; //Medidas del Gyrometro Eje Z
  uint8_t GYRO_ZOUT_L;

}mpu6050MeasurementsData_t;

#define MPU6050_DATA_RX_BUFER_SIZE        14

/*
  Variables Globales
*/
//Para el formateo de datos a leer
static mpu6050MeasurementsData_t mpu6050MeasurementsData; 
//Para Lecturas crudas (Raw Data)
static uint8_t mpu6050DataBuffer[MPU6050_DATA_RX_BUFER_SIZE]={0};

/*
  Funciones Prototipo
  Acceso: Publico

*/
extern void MPU6050_Init(void);
extern void MPU6050_ReadScaled(float *AX, float *AY, float *AZ, float *TEMP, float *GX, float *GY, float *GZ);
extern void MPU6050_ReadRaw(int16_t *Ax, int16_t *Ay, int16_t *Az, int16_t *Temp, int16_t *Gx, int16_t *Gy, int16_t *Gz);
extern bool MPU_6050_Test_If_Present(void);



#endif