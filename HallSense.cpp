#define LED_DIGITAL_PIN   2
#define minimumRotationalSpeed 8000

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "HallSense.h"

String apiKey = "D1ORP8ET4HSSGJLC";
const char* server = "api.thingspeak.com";

HallSense::HallSense(int p, String n)
{
  pin = p;
  sname = n;
  this->Reset();

  pinMode(this->pin, INPUT);
  this->currentState = false;
  this->priorState = false;
}

void HallSense::Reset()
{
  count = 0;
  currentTime = 0;
  priorTime = 0;
  currentState = false;
  priorState = false;
  moving = false;
}

void HallSense::checkState(WiFiClient *c)
{
  currentState = digitalRead(pin);
  currentTime = millis();

  long elapsedTime = currentTime - priorTime;
      
  // if different from prior then send a message to the World!
  if (currentState != priorState)
  {  
      // display the current state of sensor1 on LED of ESP8266
      
      digitalWrite(LED_DIGITAL_PIN, currentState);

      Serial.print(sname);
      Serial.print(": state changed - value is:");
      Serial.print(currentState);
      Serial.print(" and elapsed time between states is:");
      Serial.print(elapsedTime);
      Serial.println(" ms");
      
      // store the elapsed time for this reading
      // this will help us decipher the movement of the door - which we'll do in ThingSpeak
      
      timeVec[count] = elapsedTime;
      
      priorState = currentState;
      priorTime = currentTime;
      count++;
      moving = true;
  }
  else
    if (elapsedTime > minimumRotationalSpeed && moving)
      {
        Serial.print(sname);
        Serial.print(": door state has changed - it has been ");
        Serial.print(elapsedTime/1000);
        Serial.println(" seconds since last state");
        
        this->tellWorld(elapsedTime, c);
        
        // the door state has changed so reset the counter and reclaim the storage array
        
        count = 0;
        moving = false;
      }
}

void HallSense::tellWorld(long elapsedTime, WiFiClient *client)
{
  if (client->connect(server,80))
  {
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(elapsedTime);
    postStr += "&field2=";
    postStr += String(count);
    postStr += "&field3=";
    postStr += sname;
    postStr += "&field4=";
    postStr += String(currentState);
    postStr += "&field5=";
    
    for (int l = 0; l < count; l++)
      postStr += String(timeVec[l]) + ",";
    
    postStr += "\r\n\r\n";
    
    Serial.println(postStr);
    
    client->print("POST /update HTTP/1.1\n");
    client->print("Host: api.thingspeak.com\n");
    client->print("Connection: close\n");
    client->print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client->print("Content-Type: application/x-www-form-urlencoded\n");
    client->print("Content-Length: ");
    client->print(postStr.length());
    client->print("\n\n");
    client->print(postStr);
  }
  else Serial.println("Error sending to thingSpeak");
  client->stop();
}
