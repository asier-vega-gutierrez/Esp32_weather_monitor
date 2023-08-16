//Librerias
#include <WiFi.h>
#include "time.h"
#include "sntp.h"
#include "DHT.h"

//variables
/*const int pulseDebounce = 250; //50
const int flowRateTimeout = 20000;
const float volPerPulse = 0.5;
const float flowRatePerMs = 60000;
volatile unsigned long secondLastPulse = 0;
volatile unsigned long lastPulse = 0;*/
volatile unsigned int pulses = 0;
int lastResetDate = 0;
int newLineCount = 0;

//Creates a server that listens for incoming connections on the specified port
WiFiServer promSrv(80);

//Led de comprovacion de estado, si esta apaga esta fuera de servicio
int pinLed = 22;

//Constantes para el temea de la sincronizacion del la hora
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
const char* time_zone = "CET-1CEST,M3.5.0,M10.5.0/3"; 

//dht
#define DHTPIN 4 
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
float humedad = 0;
float temperatura = 0;


void setup() {

  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, LOW);

  //Velocidad del puerto serie
  Serial.begin(9600);
  
  //Conexion a la red wifi
  Serial.print("Connecting to WiFi");
  WiFi.begin("", "");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("Connected as ");
  Serial.println(WiFi.localIP());

  //ORIGINAL
  //Fecha y hora del esp32
  /*configTzTime("AEST-10:00:00AEDT-11:00:00,M10.1.0/02:00:00,M4.1.0/03:00:00", "au.pool.ntp.org");
  struct tm localTime;
  getLocalTime(&localTime);
  lastResetDate = localTime.tm_mday;
  Serial.println(&localTime, "It's currently %A %d %B %Y %H:%M:%S %Z");*/

  //COMENTAR
  sntp_set_time_sync_notification_cb(timeavailable);
  sntp_servermode_dhcp(1);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  //Inicio del servidor
  promSrv.begin();
  //Inicio del sensor dht
  dht.begin();

  //ORIGINAL
  //pinMode(D7, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(D7), pulse, RISING);

  //Realizamos una primera leida de los sensores
  leerSensores();

  //Si toto de ha preparado corectemtn se enciende la led
  Serial.println("Ready!");
  digitalWrite(pinLed, HIGH);

}

void loop() {

  //Actualizamos los valores de los sensores
  leerSensores();

  //Si se pierde la conexion intentamos conectarnos de nuevo
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi");
    WiFi.begin();
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
    Serial.print("Connected as ");
    Serial.println(WiFi.localIP());
  }
  
  //Pintamos el html con los datos leidos
  WiFiClient client = promSrv.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          newLineCount++;
          if (newLineCount == 2) {

            //Sincornizar la hora
            struct tm localTime;
            getLocalTime(&localTime);
            if (localTime.tm_mday != lastResetDate) {
              pulses = 0;
              lastResetDate = localTime.tm_mday;
            }

            //Respuesta al cliente
            client.print("HTTP/1.1 200 OK\n");
            client.print("Content-Type: text/plain; charset=UTF-8; version=0.0.4\n");
            client.print("Access-Control-Allow-Origin: *\n");
            client.print(&localTime, "X-DateTime: %A %d %B %Y %H:%M:%S %Z\n");
            client.print("X-WiFi: ");
            client.print(WiFi.SSID());
            client.print(" (");
            client.print(WiFi.BSSIDstr());
            client.print(") at ");
            client.print(WiFi.RSSI());
            client.print("dBm\n");
            client.print("Connection: close\n\n");

            //Texto que se vera en pantalla
            client.print("# HELP humidity_1 Percentage of Humidity\n");
            client.print("# TYPE humidity_1 gauge\n");
            client.print("humidity_1 ");
            client.print(humedad);
            client.print("\n\n");
            client.print("# HELP temperature_in_celsius_1 Temperature in Celsius\n");
            client.print("# TYPE temperature_in_celsius_1 gauge\n");
            client.print("temperature_in_celsius_1 ");
            client.print(temperatura);
            client.print("\n\n");

            client.print("\n");
            client.stop();
          }
        } else if (c != '\r') {
          newLineCount = 0;
        }
      }
    }
  }
}

/*void pulse() {
  if (millis() - lastPulse > pulseDebounce) {
    pulses++;
    secondLastPulse = lastPulse;
    lastPulse = millis();
  }
}*/

//Para pintar la hora actual
void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("No time available (yet)");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

//Si se recive un ajuste de la hora se notifica
void timeavailable(struct timeval *t){
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}

//Funcion para leer todos los sensores de una vez
void leerSensores(){
  //dht
  humedad = dht.readHumidity();
  temperatura = dht.readTemperature();
  if (isnan(humedad) || isnan(temperatura) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
}
