#include "MPU6050.h"

//Inicializar el sensor MPU050
extern void MPU6050_Init(void){
  //Power Cycle MPU6050, if SLEEP=0

  //Set Clock Source to X-gyrosocope reference
  //If CLKSEL=1, PLL with X axis gyroscope reference

  /*START Condition, point to SLAVE ADDR  */
  Wire.beginTransmission(MPU_6050_SLAVE_ADDR);

  /*Send RPA pointer to write*/
  Wire.write(MPU_6050_REG_MAP_PWR_MGMT_1);
  /*Send custom byte to RPA(DATA) */
  Wire.write(0x01);

  /*STOP Condition(true),  RE-start(false) */
  Wire.endTransmission(true);

//=======================================================================

//Set the sample data rate : SMPLRT_DIV
/*
  Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
  where Gyroscope Output Rate = 8kHz when the DLPF is disabled (DLPF=0)

  So if Sample Rate=1kHz is desired, SMPLRT_DIV=7
*/

  /*START Condition, point to SLAVE ADDR  */
  Wire.beginTransmission(MPU_6050_SLAVE_ADDR);

  /*Send RPA pointer to write*/
  Wire.write(MPU_6050_REG_MAP_SMPLRT_DIV);
  /*Send custom byte to RPA(DATA) */
  Wire.write(0x07);

  /*STOP Condition(true),  RE-start(false) */
  Wire.endTransmission(true);

//----------------------------------------------------------------------

//Set BandWidth Configuration of accelerometer and gyro
// If DLPF_CFG=0, AccelBW=260Hz, delay=0ms, Fs=1kHz(always); 
//                GyroBW=256Hz, delay=0.98ms, Fs=8kHz(selected); 

/*START Condition, point to SLAVE ADDR  */
  Wire.beginTransmission(MPU_6050_SLAVE_ADDR);

  /*Send RPA pointer to write*/
  Wire.write(MPU_6050_REG_MAP_CONFIG);
  /*Send custom byte to RPA(DATA) */
  Wire.write(0x00);

  /*STOP Condition(true),  RE-start(false) */
  Wire.endTransmission(true);

//---------------------------------------------------------------------

//Set FSR(Full Scale Range) of Gyrometer
//If FS_SEL =1, Gyro Range0 ± 500 °/s

/*START Condition, point to SLAVE ADDR  */
  Wire.beginTransmission(MPU_6050_SLAVE_ADDR);

 /*Send RPA pointer to write*/
  Wire.write(MPU_6050_REG_MAP_GYRO_CONFIG);
  /*Send custom byte to RPA(DATA) */
  Wire.write(MPU_6050_GYRO_CONFIG_FSEL_500_DEG_SEC);

  /*STOP Condition(true),  RE-start(false) */
  Wire.endTransmission(true);

//---------------------------------------------------------------------

//Set FSR(Full Scale Range) of Accelerometer
//If ACCEL_CONFIG=0, Accel Range=+-2g

/*START Condition, point to SLAVE ADDR  */
  Wire.beginTransmission(MPU_6050_SLAVE_ADDR);

 /*Send RPA pointer to write*/
  Wire.write(MPU_6050_REG_MAP_ACCEL_CONFIG);
  /*Send custom byte to RPA(DATA) */
  Wire.write(MPU_6050_ACCEL_CONFIG_AFS_SEL_2G);

  /*STOP Condition(true),  RE-start(false) */
  Wire.endTransmission(true);

}

extern void MPU6050_ReadScaled(float *AX, float *AY, float *AZ, float *TEMP, float *GX, float *GY, float *GZ){
    int16_t Ax,Ay,Az,T,Gx,Gy,Gz;

  //Get Raw Data from MPU6050
  MPU6050_ReadRaw(&Ax, &Ay, &Az, &T, &Gx, &Gy, &Gz);

 //Acceleration Data in format 'g'
  *AX=((float) Ax)/(16384.0f); //16384.0 if AFS_SEL=0 
  *AY=((float) Ay)/(16384.0f);
  *AZ=((float) Az)/(16384.0f);

//Temperature in format °C
  *TEMP=(((float) T)/(340.0f) + 36.53f);
  
  //Gyroscope Data in format '°/sec'
  *GX=((float) Gx)/(65.5f); //65.5 if FS_SEL=1
  *GY=((float) Gy)/(65.5f); //65.5 if FS_SEL=1
  *GZ=((float) Gz)/(65.5f); //65.5 if FS_SEL=1
}

extern void MPU6050_ReadRaw(int16_t *Ax, int16_t *Ay, int16_t *Az, int16_t *Temp, int16_t *Gx, int16_t *Gy, int16_t *Gz){

//Actualizar el registro puntero(RPA) desde donde se va leer

  /*START Condition, point to SLAVE ADDR  */
  Wire.beginTransmission(MPU_6050_SLAVE_ADDR);

   /*Send RPA pointer to write*/
  Wire.write(MPU_6050_REG_MAP_ACCEL_XOUT_H);

  /*STOP Condition(true),  RE-start(false) */
  Wire.endTransmission(false); //RE-START

  /*Request # bytes from device
   STOP Condition(true),  RE-start(false)
  */
  Wire.requestFrom(MPU_6050_SLAVE_ADDR, MPU6050_DATA_RX_BUFER_SIZE, true);

  int i=0;

  //Mientras hayan bytes por recibir, acumular en el arreglo o array
  while(Wire.available()){
    mpu6050DataBuffer[i++]=(uint8_t) Wire.read();
  }

  //Copiar array hacia la variable tipo estrucutura 'mpu6050MeasurementsData'
  memcpy(&mpu6050MeasurementsData, mpu6050DataBuffer, MPU6050_DATA_RX_BUFER_SIZE);

  //Covertir data RAW en data que se pueda leer: word DATA_H:DATA_L
  *Ax=(int16_t) ( (((uint16_t) mpu6050MeasurementsData.ACCEL_XOUT_H)<<8)&0xFF00 | (((uint16_t) mpu6050MeasurementsData.ACCEL_XOUT_L)<<0)&0x00FF ) ;
  *Ay=(int16_t) ( (((uint16_t) mpu6050MeasurementsData.ACCEL_YOUT_H)<<8)&0xFF00 | (((uint16_t) mpu6050MeasurementsData.ACCEL_YOUT_L)<<0)&0x00FF ) ;
  *Az=(int16_t) ( (((uint16_t) mpu6050MeasurementsData.ACCEL_ZOUT_H)<<8)&0xFF00 | (((uint16_t) mpu6050MeasurementsData.ACCEL_ZOUT_L)<<0)&0x00FF ) ;

  *Temp=(int16_t) ( (((uint16_t) mpu6050MeasurementsData.TEMP_OUT_H)<<8)&0xFF00 | (((uint16_t) mpu6050MeasurementsData.TEMP_OUT_L)<<0)&0x00FF ) ;

  *Gx=(int16_t) ( (((uint16_t) mpu6050MeasurementsData.GYRO_XOUT_H)<<8)&0xFF00 | (((uint16_t) mpu6050MeasurementsData.GYRO_XOUT_L)<<0)&0x00FF ) ;
  *Gy=(int16_t) ( (((uint16_t) mpu6050MeasurementsData.GYRO_YOUT_H)<<8)&0xFF00 | (((uint16_t) mpu6050MeasurementsData.GYRO_YOUT_L)<<0)&0x00FF ) ;
  *Gz=(int16_t) ( (((uint16_t) mpu6050MeasurementsData.GYRO_ZOUT_H)<<8)&0xFF00 | (((uint16_t) mpu6050MeasurementsData.GYRO_ZOUT_L)<<0)&0x00FF ) ;


}

extern bool MPU_6050_Test_If_Present(void){
  uint8_t slaveADDR;  //Variable que guardara la direccion del esclavo

  
  //Actualizar el registro RPA hacia donde se va leer
  //Registro a leer: "WHO_AM_I"

/*START Condition, point to SLAVE ADDR  */
  Wire.beginTransmission(MPU_6050_SLAVE_ADDR);

 /*Send RPA pointer to write*/
  Wire.write(MPU_6050_REG_MAP_WHO_AM_I);

  /*STOP Condition(true),  RE-start(false) */
  Wire.endTransmission(false);

  /*Solicitar un # de bytes al esclavo, (Solo 1 byte)
    STOP Condition(true),  RE-start(false)
  */
  Wire.requestFrom(MPU_6050_SLAVE_ADDR, 1, true);

   //Esperar el byte que viene desde el esclavo este disponible
    while(Wire.available()) {
      slaveADDR=(uint8_t) Wire.read();
    }

  if(slaveADDR==MPU_6050_SLAVE_ADDR)  return true; //Verificacion de direccion Esclaco I2C correcta
  else                                return false; //No se pudo verificar la direccion : Esclacvo I2C

}
