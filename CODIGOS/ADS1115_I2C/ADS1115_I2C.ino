#include <Wire.h>
#include <Adafruit_ADS1X15.h>

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

//Objetos de Librerias
Adafruit_ADS1115 ads;

#define TASK1_POLLRATE_MS       300UL  //Tarea de lectura -promediado de sensor

#define NUMBER_OF_TASKS         1     //Cantidad de tareas del sistema

//Definir variables de temporizacion de las tareas
static uint32_t timePrev[NUMBER_OF_TASKS]={0};

void setup() {

   //Power up, delay &I2C , Iniciar Periferico I2C como Maestro
  //      at ....kHz
  delay(100);
  Wire.begin(I2C_SDA, I2C_SCL); // Use GPIO4 (D2) for SDA, GPIO5 (D1) for SCL
  Wire.setClock(I2C_BIT_RATE_100KHZ);

  //Iniciar puerto serie
  Serial.begin(9600);

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS1115.");
    return;
  }

  // Set gain to ±6.144V (the largest range, suited for 0–5V)
  ads.setGain(GAIN_TWOTHIRDS);

  // Set sample rate to 250 samples per second
  ads.setDataRate(RATE_ADS1115_250SPS);

  Serial.println("ADS1115 initialized with GAIN_TWOTHIRDS and sample rate: 250SPS.");

  //Iniciar tiempo en milisegundos de ejecucion de tareas
  Task_Init();
 
}

void loop() {

  if((millis() - timePrev[0])>=TASK1_POLLRATE_MS){
      timePrev[0]=millis();

      // Read from A0 (single-ended)
    int16_t adc0 = ads.readADC_SingleEnded(0);

    // Convert to voltage
    float volts0 = ads.computeVolts(adc0);

   //Imprimir en pantalla
    Serial.print("A0 ADC: ");  //Valor crudo
    Serial.print(adc0, DEC);
    Serial.print(" => Voltage: "); //Valor convertido
    Serial.print(volts0, 4);
    Serial.println(" V");

  }


}


void Task_Init(void){
    //Iniciar el arreglo de temporizacion de tareas
    for(int i=0; i<NUMBER_OF_TASKS; i++){
      timePrev[i]=millis();
    }
}
