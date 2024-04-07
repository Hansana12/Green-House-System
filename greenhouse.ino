#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#include <DHT.h>
#include <Wire.h>
#include <BH1750.h> // SCL:22 & SDA:21

#include "relayupdater.h"
#include "config.h"
#include "dipconf.h"

FirebaseAuth auth;
FirebaseConfig config;

float humidity;
float temperature;
float lightAP;
int lightA;
int soilHA;
int waterD;
int waterStatus;

// Variables to store previous sensor readings
float prevHumidity = -1.0;
float prevTemperature = -1.0;
float prevLightAP = -1.0;
float prevSoilHAP = -1.0;
int prevWaterStatus = -1;
int prevwaterStatus = 2;

int count = 1;

String sensorDataPath;
FirebaseJson json;
BH1750 lightMeter;

char dipStateBinary[3];
bool signupOK = false;

void setup()
{
  Wire.begin();
  lightMeter.begin();
  pinMode(relaySW1, OUTPUT);
  pinMode(relaySW2, OUTPUT);
  pinMode(dip1, INPUT_PULLUP);
  pinMode(dip2, INPUT_PULLUP);
  digitalWrite(relaySW1, HIGH);
  digitalWrite(relaySW2, HIGH);

  delay(2000);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }

  dipSet();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("ok");
    signupOK = true;
  }
  else
  {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  if (dip1state == 0)
  {
    dht.begin();
    Serial.println(dip1state);
  }
  if (dip1state == 1)
  {
    Serial.println(dip1state);
    dht11.begin();
  }
}

void loop()
{
  if (Firebase.ready() && signupOK)
  {
    if (dip1state == 0)
    {
      humidity = dht.readHumidity();
      Serial.println(humidity);
      temperature = dht.readTemperature();
      Serial.println(temperature);
    }
    if (dip1state == 1)
    {
      humidity = dht11.readHumidity();
      Serial.println(humidity);
      temperature = dht11.readTemperature();
      Serial.println(temperature);
    }
    lightA = analogRead(lightAread);
    soilHA = analogRead(soilHAread);
    waterD = digitalRead(waterSensor);
    if (dip1state == 0)
    {
      lightAP = lightMeter.readLightLevel();
    }

    if (dip1state == 1)
    {
      lightAP = map(lightA, 4095, 0, 0, 100);
    }
    float soilHAP = map(soilHA, 4095, 0, 0, 100);
    if (waterD == HIGH)
    {
      waterStatus = 1;
    }
    else if (waterD == LOW)
    {
      waterStatus = 0;
    }
    // Check if sensor readings have changed

    json.set(hum.c_str(), String(humidity));
    json.set(temp.c_str(), String(temperature));
    json.set(light.c_str(), String(lightAP));
    json.set(soilH.c_str(), String(soilHAP));
    json.set(water.c_str(), String(waterStatus));

    if (dip2state == 1)
    {
      
      sensorDataPath = "TestData/" + String(count);
      Serial.printf("[Test_Mode] : Updating firebase... %s\n", Firebase.RTDB.setJSON(&fbdo, sensorDataPath.c_str(), &json) ? "Success" : fbdo.errorReason().c_str());
      count++;
      delay(1000);
    }

    if (dip2state == 0)
    {
      relayStatus();
      if (humidity != prevHumidity || temperature != prevTemperature ||
          lightAP != prevLightAP || soilHAP != prevSoilHAP || waterStatus != prevWaterStatus)
      {

        prevHumidity = humidity;
        prevTemperature = temperature;
        prevLightAP = lightAP;
        prevSoilHAP = soilHAP;
        prevWaterStatus = waterStatus;
        sensorDataPath = "LiveData/SensorData/";
        Serial.printf("[Operation_Mode] : Updating firebase]... %s\n", Firebase.RTDB.setJSON(&fbdo, sensorDataPath.c_str(), &json) ? "Success" : fbdo.errorReason().c_str());
        delay(200);
      }
      else
      {
        Serial.println("Values are same. Not updating firebase.");
      }
    }
  }
}
