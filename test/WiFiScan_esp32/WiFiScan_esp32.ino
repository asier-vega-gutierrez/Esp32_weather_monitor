/*
 *  Wifi scan para esp32
 *  Realiza un escan de las redes que pueda detectar
 */

  // El esp32 puede usarse de :
  //   - Estacion wifi (STA), este es otro cliente de una red. El esp32 coje su ip del DHCP del router al que se conecta
  //   - Punto de acceso (SAP), es el centro de la red que provee servicio al los dispositivos conectados. El esp32 asigna una ip a los clientes (maximo 5) que se conecten
  // *El esp32 no tiene conexion directa a internet, solo sirve como punto de acceso por eso se considera punto de acceso libgero (soft acces point) 

#include "WiFi.h"

void setup()
{
    //Inportante que los baud del monitor serie coincidan con los de este parametro
    Serial.begin(115200);
    //En este caso se necesita establecerlo como estacion
    WiFi.mode(WIFI_STA); 
    //Esto impide que se conecta, ya que solo uqeremos hacer el escaner
    WiFi.disconnect();
    delay(100);
    Serial.println("Setup done");
}

void loop()
{
    Serial.println("Scan start");
    // Esto devuleve el numero de redes que se han encontrado
    int n = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
        //Se itera por todas las redes encontradas
        for (int i = 0; i < n; ++i) {
            Serial.printf("%2d",i + 1);
            Serial.print(" | ");
            Serial.printf("%-32.32s", WiFi.SSID(i).c_str()); //Se pinta el nombre de la red 
            Serial.print(" | ");
            Serial.printf("%4d", WiFi.RSSI(i)); //Se pinta la fuerza de la señal
            Serial.print(" | ");
            Serial.printf("%2d", WiFi.channel(i)); //Se pinta el canal
            Serial.print(" | ");
            switch (WiFi.encryptionType(i)) //Si o no es un red abierta
            {
            case WIFI_AUTH_OPEN:
                Serial.print("open");
                break;
            case WIFI_AUTH_WEP:
                Serial.print("WEP");
                break;
            case WIFI_AUTH_WPA_PSK:
                Serial.print("WPA");
                break;
            case WIFI_AUTH_WPA2_PSK:
                Serial.print("WPA2");
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                Serial.print("WPA+WPA2");
                break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
                Serial.print("WPA2-EAP");
                break;
            case WIFI_AUTH_WPA3_PSK:
                Serial.print("WPA3");
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                Serial.print("WPA2+WPA3");
                break;
            case WIFI_AUTH_WAPI_PSK:
                Serial.print("WAPI");
                break;
            default:
                Serial.print("unknown");
            }
            Serial.println();
            delay(10);
        }
    }
    Serial.println("");
    // Borrar el contenido apra liberar espacio en memoria
    WiFi.scanDelete();
    delay(5000);
}
