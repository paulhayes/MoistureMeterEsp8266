#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <ESP8266WiFi.h>

#include <Phant.h>
#include <stdio.h>
#include "mathFuncs.h"
#include <EEPROM.h>

/*
 * {"title":"Soil Moisture Sensor","outputUrl":"http://data.sparkfun.com/output/q5X6GMEqJZUwE6KZjXK3","inputUrl":"http://data.sparkfun.com/input/q5X6GMEqJZUwE6KZjXK3","manageUrl":"http://data.sparkfun.com/streams/q5X6GMEqJZUwE6KZjXK3","publicKey":"q5X6GMEqJZUwE6KZjXK3","privateKey":"BVA2qdkYB1I9vNZRGMZn","deleteKey":"Rv2LgjAqyEs3KGe1NjeY"}
 */

//const char* host = "192.168.0.103";
//const char* ssid     = "dlink-7AE0";
//const char* password = "eeews24530";

const char* ssid     = "dlink-7AE0";
const char* password = "eeews24530";

const char* host = "data.sparkfun.com";
const char* streamId   = "q5X6GMEqJZUwE6KZjXK3";
const char* privateKey = "BVA2qdkYB1I9vNZRGMZn";

float MoistureMax = 0.2;
float MoistureMin = 1.0;
float reportThreshold = 0.05;

float lastSentMoisture;

int sleepTimeS = 60;

#define ledPin 5
#define indicatorPin 4
#define sensorPowerPin 0

Phant phant(host, streamId, privateKey);

void setup() {

  // put your setup code here, to run once:
  pinMode(ledPin,OUTPUT);
  pinMode(indicatorPin,OUTPUT);
  pinMode(sensorPowerPin,OUTPUT);

  digitalWrite(ledPin, LOW);
  digitalWrite(indicatorPin, LOW);
  digitalWrite(sensorPowerPin, HIGH);

  EEPROM.begin(32);
  EEPROM_readFloat(0, lastSentMoisture);
  if( isnan( lastSentMoisture ) ){
    lastSentMoisture = 0.0;
  }
  
}

void loop() {
  digitalWrite(ledPin, HIGH );
  digitalWrite(sensorPowerPin, LOW);

  delay(10);
  long avg = 0;
  const int numSamples = 200;
  for(int sample=0;sample<numSamples;sample++){
    avg += analogRead(A0);
    delay(10);
  }
  avg /= numSamples;
  float* samples;
    
  digitalWrite(sensorPowerPin, HIGH);

  float moisture = iLerp( MoistureMin, MoistureMax, 1.0 * avg / 1024.0 );
  float diff = abs( moisture - lastSentMoisture );
    
  if( diff > reportThreshold ){

    // CONNECT
  
    digitalWrite(ledPin, LOW);
    digitalWrite(indicatorPin, LOW);
    meter( moisture );
  
    int flashingCounter = 0;
    int ledOn = 0;
    
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED ) {
      delay(100);
      if( ++flashingCounter > 2 ){
        digitalWrite(ledPin, ledOn=!ledOn );
        flashingCounter = 0;
      }
    }
  
    //END CONNECT
    
    digitalWrite(ledPin, HIGH);

    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      digitalWrite(ledPin, LOW);
      return;
    }
    phant.add("moisture", moisture );
    phant.add("diff", diff );
  
    client.print(phant.post());
    
    client.print(String());
  
    while(client.available()){
      String line = client.readStringUntil('\r');
    }

    lastSentMoisture = moisture;
    digitalWrite(indicatorPin, LOW);
    EEPROM_writeFloat(0, moisture);
    EEPROM.commit();  

    //WiFi.disconnect();
    
  }
  else {
    meter( moisture );
    delay(1000);
  }

  digitalWrite(ledPin, LOW);
  digitalWrite(indicatorPin, LOW);
  ESP.deepSleep(sleepTimeS * 1000000);
  //delay(sleepTimeS*1000);
}

void meter(float measure){
    analogWrite(indicatorPin,floor( 196 * clamp(0,1,1-measure)) );
}

int EEPROM_writeFloat(int ee, const float& value)
{
   const byte* p = (const byte*)(const void*)&value;
   int i;
   const byte valueSize = sizeof(value);
   for (i = 0; i < valueSize; i++)
       EEPROM.write(ee++, *p++);
   return i;
}

int EEPROM_readFloat(int ee, float& value)
{
   byte* p = (byte*)(void*)&value;
   int i;
   const byte valueSize = sizeof(value);
   for (i = 0; i < valueSize; i++)
       *p++ = EEPROM.read(ee++);
   return i;
}
