#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <DHT.h>
#include <DHT_U.h>

//Definir el tipo de sensor
#define DHTTYPE           DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

//Definir el pin digital del sensor
#define DHTPIN                  14

//Objeto de la libreria- clase del sensor
DHT dht(DHTPIN, DHTTYPE);

#define WIFI_TIMEOUT_CONNECTION           10000UL   //Timeout de conexion WiFi en 'ms'

#define SSID                              "ROSENFELD-2.4G"
#define PASSWORD                          "A8s1L2d3jZcKg96"

//Objeto WiFi Client
WiFiClient client;

//Objeto HTTPCliente
HTTPClient http;


void setup() {

  //Iniciar puerto serie
  Serial.begin(9600);

  //Inicializar sensor DHT
  dht.begin();

  //Configurar WiFi como STA: Unirse al AP
  setup_wifi();

}

void loop() {
  // put your main code here, to run REPETITIVA:

  //Tomar muestras de temperatura y humedad
  float t=dht.readTemperature();
  float h=dht.readHumidity();

  //Imprimir data en puerto CONSOLA
  Serial.print("\r\nTemperature [°C]: "),
  Serial.print(t, 2);
  Serial.println("");
  Serial.print("\r\nHumidity [%RH]: "),
  Serial.print(h, 2);
  Serial.println("");

  //Crear el documento en formato JSON
  StaticJsonDocument<256> doc;

  //Añadir valores al JSON
  doc["temperature"]=t;
  doc["humidity"]=h;

  //Generar la Trama JSON
  String payloadUPLINK;
  serializeJson(doc, payloadUPLINK); //Serializar documento JSON en dato 'String'
  Serial.println("\r\nJSON payload UPLINK [TX]: ");
  Serial.println(payloadUPLINK);
  Serial.println("");

  //Hasta aqui se imprime..
  /*

  */

 //ARMAR una PETICION HTTP desde el cliente
  http.begin(client, "http://industrial.api.ubidots.com/api/v1.6/devices/nodemcu-esp8266-ubidots/");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-Auth-Token", "BBUS-FCNCDjvXGpmc8nQ8vlgTqNgFvdcRqj");

  //ENVIAR LA PETICION + PAYLOAD DE UPLINK
  int httpCode=http.POST(payloadUPLINK);

  //Evaluar el codigo de respuesta desde el servidor (debe ser 200 OK)
  Serial.print("\r\nHTTP Response Code(from server): ");
  Serial.println(httpCode, DEC);
  Serial.println("");

  delay(300);

  if(httpCode>0){
    String payloadDOWNLINK=http.getString();
    Serial.println("\r\nJSON payload DOWNLINK [RX]: ");
    Serial.println(payloadDOWNLINK);
  }

  delay(10000);

}


void setup_wifi(void){
    //Conectarse a la red WiFi
    WiFi.begin(SSID, PASSWORD);

    //Imprimir info al usuario
    Serial.print("\r\nConnecting to WiFi");

    unsigned long start=millis();

    while( (WiFi.status()!=WL_CONNECTED) && (millis() - start)<=WIFI_TIMEOUT_CONNECTION  )
    {
      delay(300);
      Serial.print(".");

    }

    if(WiFi.status()!=WL_CONNECTED)
    {
      Serial.println("\r\nCould not connect to AP, try again !!");
      return;
    }

    Serial.print("\r\nConnected to Access Point: ");
    Serial.println(SSID);
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC Address(esp8266): ");
    Serial.println(WiFi.macAddress());
    Serial.println("");
}