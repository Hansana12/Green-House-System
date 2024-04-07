#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define API_KEY ""
#define DATABASE_URL ""

String relay1 = "LiveData/relaySW/rel1";
String relay2 = "LiveData/relaySW/rel2";
int relaySW1 = 0;
int relaySW2 = 5;

int dip1 = 13;
int dip2 = 16;

String hum = "/air_humidity";
String temp = "/temperature";
String light = "/light";
String soilH = "/soil_humidity";
String water = "/water";

const int lightAread = 12;  // Light sensor analog pin
const int soilHAread = 35;  // Soil humidity sensor analog pin
const int waterSensor = 12; // Water sensor digital pin
DHT dht(4, DHT21); 
DHT dht11(17, DHT11);        // DHT21 sensor pin


#endif // CONFIG_H
