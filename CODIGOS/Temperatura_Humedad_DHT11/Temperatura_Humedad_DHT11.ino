#include <DHT.h>
#include <DHT_U.h>

#include "Estadisticas.h"

//Definir el tipo de sensor
#define DHTTYPE           DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

//Definir el pin digital del sensor
#define DHTPIN                  4

//Objeto de la libreria- clase del sensor
DHT dht(DHTPIN, DHTTYPE);

//Definir pines digitales I/O
#define AC_LOAD_PIN             7 //Pin para controlar alguna carga externa tipo salida
#define CTRL_BUTTON             8  //pulsador de entrada conectado en D8

#define MAX_SAMPLES             10 //Tamaño del array de muestras a tomar promedio

//Definir las constantes de trabajo
const float tempHighLIMIT=27.0f;
const float sensibilidadTemp=1.0f;

//Declaracion de variables de trabajo
static float temperature[MAX_SAMPLES]={0.0f}; //ARRAY de muestras
static int i=0;                               //Iterador de elementos del ARRAY
static bool stopReading=false;              //Para deshabilotar el sistema

static uint8_t ledState=LOW;                //Variable para conmutar el LED
static uint8_t buttonState,lastButtonState=HIGH;  //Variable para leer-actualizar el estado del boton

#define PRINT_BUFFER_SIZE       64 //Tamaño del texto en el buffer de impresion (cantidad de letras)

//Definir variables de IMPRESION(texto) en CONSOLA
static char serialBUFFER[PRINT_BUFFER_SIZE]="";
static char tempF[6]="";
static char humF[6]="";
static char tempPromF[6]="";

#define TASK1_POLLRATE_MS       2000UL  //Tarea de leer sensor DHT11 (en milisegundos)
#define TASK2_POLLRATE_MS       200UL  //Tarea de encendido rapido de LED en placa (en milisegundos)
#define TASK3_POLLRATE_MS       50UL  //Tarea de sensado del estado de boton-pulsador

#define NUMBER_OF_TASKS         4     //Cantidad de tareas del sistema

//Definir variables de temporizacion de las tareas
static uint32_t timePrev[NUMBER_OF_TASKS]={0};


void setup() {
  // Iniciar pines I/O
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(CTRL_BUTTON, INPUT_PULLUP);

  pinMode(AC_LOAD_PIN, OUTPUT);
  digitalWrite(AC_LOAD_PIN, LOW);

  //Inicar comunicacion serie consola
  Serial.begin(9600);

  //Inicializar sensor DHT
  dht.begin();

  //Filear o llenar los numeros del arreglo con un mismo valor inicial
  float t=dht.readTemperature();
  for(int i=0; i<MAX_SAMPLES; i++){
    temperature[i]=t;
    
  }

  Serial.println("System Test Initializating...!!!");

  //Iniciar tiempo en milisegundos de ejecucion de tareas
  Task_Init();


}

void loop() {
  float t,h;

  //Tarea TASK que se ejecuta cada 'TASK3_POLLRATE_MS'
  if((millis() - timePrev[2])>=TASK3_POLLRATE_MS){
    timePrev[2]=millis();

    //Segmento para leer el sensor
    buttonState=digitalRead(CTRL_BUTTON);

    //Retardo de debouncing
    delay(10);

    if(buttonState!=lastButtonState){
      if(buttonState==LOW){             //Hay una transicion de HIGH a LOW en el boton ?: https://i.sstatic.net/UW8v9.png 
        //Par DESHABILITAR /RE-Habilitar Termostato
        stopReading=!stopReading;

        //Realizar alguna accion
        Serial.println("");
        Serial.println("Ud presiono el boton !!!");
        Serial.println(stopReading? "TERMOSTATO OFF":"TERMOSTATO ON");
        Serial.println("");
      
      }

    }

    //Salvar el estado del boton
    lastButtonState=buttonState;

  }

  //Tarea TASK que se ejecuta cada 'TASK1_POLLRATE_MS'
  if((millis() - timePrev[0])>=TASK1_POLLRATE_MS){
    timePrev[0]=millis(); //Actualizar cuenta de temorizador

    if(!stopReading)    //EVALUAR SI SE HA DESHABILITADO LA LECTURA
    {

    //EL SENSOR DHT11 no se puede leer a menos de 2 segundos..
    //Sino Falla 
    t=dht.readTemperature();
    h=dht.readHumidity();

    //Acumular al arreglo desde donde se tomaran promedios
    temperature[i++]=t;
    if(i>=MAX_SAMPLES)    i=0; //poner a cero el iterador

    //Dar formato TEXTO a los valores
    dtostrf(t,2,2,tempF);
    dtostrf(h,2,2,humF);
    //Calcular el promedio de la temperatura y convertir a "##.##"
    dtostrf(promediar(temperature, MAX_SAMPLES),2,2,tempPromF);

    sprintf(serialBUFFER, "Temp=%s °C, Temp_promedio=%s °C, Hum=%s rH, T_millis=",
            tempF,
            tempPromF,
            humF
    );

    Serial.print(serialBUFFER);
    Serial.print(millis(), DEC);
    Serial.println("ms");

    }

  }

  //Tarea TASK que se ejecuta cada 'TASK2_POLLRATE_MS'
  if((millis() - timePrev[1])>=TASK2_POLLRATE_MS){
    timePrev[1]=millis(); //Actualizar cuenta de temorizador

    //VALOR DE CONTROL DE TEMPERATURA:  Limite TH  y TL
    if((t - tempHighLIMIT)>=sensibilidadTemp && !stopReading){

      //Efecto de conmutacion rapida del LED
      ledState=!ledState;
      digitalWrite(LED_BUILTIN, ledState);

      //Encender carga AC externa con modulo rele
      digitalWrite(AC_LOAD_PIN, HIGH);

    }else{

      //Led incorporado en la placa ya no parpadea
      digitalWrite(LED_BUILTIN, LOW);

      //Apagar carga AC externa
      digitalWrite(AC_LOAD_PIN, LOW);

    }

  }
  

}


void Task_Init(void){
    //Iniciar el arreglo de temporizacion de tareas
    for(int i=0; i<NUMBER_OF_TASKS; i++){
      timePrev[i]=millis();
    }
}
