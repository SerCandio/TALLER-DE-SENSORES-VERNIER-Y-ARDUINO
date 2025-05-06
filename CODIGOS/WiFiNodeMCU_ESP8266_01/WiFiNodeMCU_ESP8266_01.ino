#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

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

    /*
      Hasta aqui se imprime:

      {
  "coord": {
    "lon": -77.0579,
    "lat": -12.0562
  },
  "list": [
    {
      "main": {
        "aqi": 2
      },
      "components": {
        "co": 72.65,
        "no": 0,
        "no2": 0.05,
        "o3": 55.94,
        "so2": 0.33,
        "pm2_5": 9.3,
        "pm10": 20,
        "nh3": 0
      },
      "dt": 1746500374
    }
  ]
}
    
    */
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
