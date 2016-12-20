#include <ESP8266WiFi.h>
#include "HallSense.h"

const char* ssid = "FiOS-KNRXW";
const char* password = "flux2383gap9255hug";

#define LED_DIGITAL_PIN   2
#define minimumRotationalSpeed 8000

WiFiClient client;
HallSense auxD(16, "AuxDoor"), mainD(0, "MainDoor");

void setup()
{
  pinMode(LED_DIGITAL_PIN, OUTPUT);

  // connect to WIFI
  
  Serial.begin(115200);
  delay(10);
  
  WiFi.begin(ssid, password);
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
   digitalWrite(LED_DIGITAL_PIN, 1);
   delay(500);
   digitalWrite(LED_DIGITAL_PIN, 0);
   Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop()
{
  auxD.checkState(&client);
  mainD.checkState(&client);
}


