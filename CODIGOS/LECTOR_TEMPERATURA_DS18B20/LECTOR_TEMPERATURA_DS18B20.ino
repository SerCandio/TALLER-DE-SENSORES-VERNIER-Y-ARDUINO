// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

//Definir en que pin se va conectar el sensor
#define ONE_WIRE_BUS          5

//Importar Objetos de libreria
OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature ds18b20(&oneWire);

#define TASK1_POLLRATE_MS       1000UL  //Tarea de leer sensor DS18B20 (en milisegundos)
#define TASK2_POLLRATE_MS       200UL  //Tarea de parpadear LED (en milisegundos)

#define NUMBER_OF_TASKS         2     //Cantidad de tareas del sistema

//Variables de trabajo
static uint8_t ledState=LOW;                //Variable para conmutar el LED

//Definir variables de temporizacion de las tareas
static uint32_t timePrev[NUMBER_OF_TASKS]={0};

void setup() {
    // Iniciar pines I/O
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //Iniciar consola serie
  Serial.begin(9600);

  //Iniciar el sensor
  ds18b20.begin();

  //Iniciar la temorizacion de las tareas
  Task_Init();


}

void loop() {
  // put your main code here, to run repeatedly:

  //TASK QUE SE EJECUTA CADA 'TASK1_POLLRATE_MS' : ENTRADA
  if((millis() - timePrev[0])>=TASK1_POLLRATE_MS){
    timePrev[0]=millis();

    ds18b20.requestTemperatures();

    float tempC = ds18b20.getTempCByIndex(0);
  
    // Verificar si todo OK con el sensor
    if(tempC != DEVICE_DISCONNECTED_C) 
    {
        Serial.print("Temperature [°C]: ");
        Serial.println(tempC);
    } 
    else
    {
      Serial.println("ERROR");
      digitalWrite(LED_BUILTIN, HIGH);
      while(1);
    }

  }

//TASK QUE SE EJECUTA CADA 'TASK2_POLLRATE_MS' : SALIDA
  if((millis() - timePrev[1])>=TASK2_POLLRATE_MS){
      timePrev[1]=millis();

       //Efecto de conmutacion rapida del LED
      ledState=!ledState;
      digitalWrite(LED_BUILTIN, ledState);
  }





}

void Task_Init(void){
    //Iniciar el arreglo de temporizacion de tareas
    for(int i=0; i<NUMBER_OF_TASKS; i++){
      timePrev[i]=millis();
    }
}