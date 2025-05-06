#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

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

  //Configurar WiFi como STA: Unirse al AP
  setup_wifi();

  //Hacer una peticion HTTP desde el cliente
  http.begin(client, "http://api.openweathermap.org/data/2.5/air_pollution?lat=-12.056165&lon=-77.057880&appid=abcf4542035084fa5ca8e4697b6bccc6");

  //Llamada al metodo GET
  int httpCode=http.GET();

  //Evaluar el codigo de respuesta desde el servidor
  Serial.print("HTTP Response Code(from server): ");
  Serial.println(httpCode, DEC);
  Serial.println("");

  delay(1000);
  
  //Imprimir el contenido 
  if(httpCode>0){
    String payload=http.getString();
    Serial.println(payload);

    //Crear el documento en formato JSON
    StaticJsonDocument<256> doc;

    //Descomponer/deserializar el payload en el documento JSON: 'doc'
    deserializeJson(doc, payload);

    //Extraer valores y guardar en variables
    float co=doc["list"][0]["components"]["co"];
    float no=doc["list"][0]["components"]["no"];
    float no2=doc["list"][0]["components"]["no2"];
    float o3=doc["list"][0]["components"]["o3"];
    float so2=doc["list"][0]["components"]["so2"];
    float pm2_5=doc["list"][0]["components"]["pm2_5"];
    float pm10=doc["list"][0]["components"]["pm10"];
    float nh3=doc["list"][0]["components"]["nh3"];

    Serial.println("");
    Serial.print("co= ");
    Serial.println(co, 2);
    Serial.print("no= ");
    Serial.println(no, 2);
    Serial.print("no2= ");
    Serial.println(no2, 2);
    Serial.print("o3= ");
    Serial.println(o3, 2);
    Serial.print("so2= ");
    Serial.println(so2, 2);
    Serial.print("pm2_5= ");
    Serial.println(pm2_5, 2);
    Serial.print("pm10= ");
    Serial.println(pm10, 2);
    Serial.print("nh3= ");
    Serial.println(nh3, 2);

  }

  delay(60000);

}

void loop() {
  // put your main code here, to run repeatedly:

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
