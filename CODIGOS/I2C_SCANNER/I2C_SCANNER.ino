#include <Wire.h> 

//Definir las velocidades I2C
#define I2C_BIT_RATE_400KHZ             400000
#define I2C_BIT_RATE_100KHZ             100000

/*
  En la placa ESP8266:
  SDA D2 (GPIO4)
  SCL D1 (GPIO5)
*/
#define I2C_SDA         4
#define I2C_SCL         5


void setup()
{
    //Power up, delay &I2C , Iniciar Periferico I2C como Maestro
  //      at ....kHz
  delay(100);
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(I2C_BIT_RATE_100KHZ);
 
  Serial.begin(9600);
  Serial.println("\nI2C Scanner");
  delay(500);
}
 
 
void loop()
{
  uint8_t error, address;
  int nDevices;
 
  Serial.println("Scanning...");
  delay(100);
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.

    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address:  0x");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      Serial.print(address,HEX);
      Serial.println("  !");
    }  

    delay(10);  //Wait until next scan request  
  }

//////////////////////////////////////////////////////////////////////////

//If no devices found....
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
 
 
 //Finalizar programa
  delay(60000);

}