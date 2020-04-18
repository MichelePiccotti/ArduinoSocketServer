#include "ESP8266WiFi.h"
#include "DHTesp.h"
#include <math.h> 
#include <stdio.h> 

const char* ssid = "SintecWiFi";
const char* password =  "sintecsintec15";
const int dhtPin = 16; //Pin connessione DHT22 - GPIO17


DHTesp dht;
WiFiServer wifiServer(80);

void setup() {

  Serial.begin(115200);

  delay(1000);

  IPAddress ip(192, 168, 0, 50);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);


  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }

  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());

  wifiServer.begin();

  dht.setup(dhtPin, DHTesp::DHT22); // Connect DHT sensor to GPIO 17
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

  WiFiClient client = wifiServer.available();

  if (client) {

    while (client.connected()) {

      while (client.available() > 0) {
        char c = client.read();
        Serial.write(c);
        char RESULT;
        RESULT = c;
        if ( RESULT == '1') {
          digitalWrite(LED_BUILTIN, LOW);
        }
        else if ( RESULT == '0') {
          digitalWrite(LED_BUILTIN, HIGH);
        }
        else if ( RESULT == '5') {
          delay(dht.getMinimumSamplingPeriod());

          float humidity = dht.getHumidity();
          float temperature = dht.getTemperature();
          Serial.print(humidity);
          //Serial.print(temperature);
          char hum[6];
          ftoa(humidity, hum, 6);
          
          //sprintf(hum, "%d", humidity);
          char tem[5];
          ftoa(temperature, tem, 5);
          client.write(hum, 6);
          client.write(";", 1);
          client.write(tem, 5);
          client.write(";", 1);
          client.write(";", 1);
          //client.write(13);
        }

      }

      delay(10);
    }
    client.stop();
    Serial.println("Client disconnected");

  }
}
void ftoa(float n, char* res, int afterpoint) 
{ 
    // Extract integer part 
    int ipart = (int)n; 
  
    // Extract floating part 
    float fpart = n - (float)ipart; 
  
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 
  
    // check for display option after point 
    if (afterpoint != 0) { 
        res[i] = '.'; // add dot 
  
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter  
        // is needed to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
} 
int intToStr(int x, char str[], int d) 
{ 
    int i = 0; 
    while (x) { 
        str[i++] = (x % 10) + '0'; 
        x = x / 10; 
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 
void reverse(char* str, int len) 
{ 
    int i = 0, j = len - 1, temp; 
    while (i < j) { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; 
        j--; 
    } 
} 
