#include <Wire.h>
#include "MPU6050.h"

//Definir las velocidades I2C
#define I2C_BIT_RATE_400KHZ             400000
#define I2C_BIT_RATE_100KHZ             100000


void setup() {

  //Power up, delay &I2C , Iniciar Periferico I2C como Maestro
  //      at 400 kHz
  delay(100);
  Wire.begin();
  Wire.setClock(I2C_BIT_RATE_400KHZ);

  //Start UART serial communication
  Serial.begin(9600);

  //Test if MPU6050 is present??
  if(!MPU_6050_Test_If_Present()){
    Serial.println("MPU6050 not connected, Check ADDR or wiring and try again !!!");
    while(1);
  }

  Serial.println("Sensor MPU6050 OK !!");
  Serial.println("");

  //Inicializar sensor MPU6050
  //Seleccinar Rango de Acelerometro: +-2g, +-4g, +-8g, +-16g
  //Seleccinar Rango de Gyrometro: +-250, +-500, +-1000, +-2000 °/sec
  //Frecuencia de muestreo: ADC 1kHz 
  //Ancho de banda del acelerometro: 260Hz, Delay=0 ms
  //Ancho de banda del Gyroscopo:    256Hz, Delay=0.98 ms
  MPU6050_Init();

}

void loop() {  //El void loop se ejecuta de forma repetitiva

  float AX, AY, AZ, temp, GX, GY, GZ;

  MPU6050_ReadScaled(&AX, &AY, &AZ, &temp, &GX, &GY, &GZ);

  //Print Acceleration Data in format 0.00f
  Serial.print("Ax ="); //16384.0 if AFS_SEL=0 
  Serial.print(AX, 2);

  Serial.print(" Ay ="); 
  Serial.print( AY, 2);

  Serial.print(" Az ="); 
  Serial.print(   AZ, 2);

  Serial.print(" T ="); 
  Serial.print( temp, 2);
  Serial.print("°C");

//Print Gyroscope Data in format 0.00f
  Serial.print(" Gx ="); 
  Serial.print(   GX, 2);

  Serial.print(" Gy ="); 
  Serial.print(   GY, 2);

  Serial.print(" Gz ="); 
  Serial.println(  GZ, 2);

  delay(500);

}
