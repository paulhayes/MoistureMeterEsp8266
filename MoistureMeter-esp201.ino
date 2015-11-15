//#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <Phant.h>
#include <stdio.h>
#include "colours.h"

/*
 * {"title":"Soil Moisture Sensor","outputUrl":"http://data.sparkfun.com/output/q5X6GMEqJZUwE6KZjXK3","inputUrl":"http://data.sparkfun.com/input/q5X6GMEqJZUwE6KZjXK3","manageUrl":"http://data.sparkfun.com/streams/q5X6GMEqJZUwE6KZjXK3","publicKey":"q5X6GMEqJZUwE6KZjXK3","privateKey":"BVA2qdkYB1I9vNZRGMZn","deleteKey":"Rv2LgjAqyEs3KGe1NjeY"}
 */

//const char* ssid     = "dlink-7AE0";
//const char* password = "eeews24530";

const char* ssid     = "dlink-7AE0";
const char* password = "eeews24530";

const char* host = "data.sparkfun.com";
const char* streamId   = "q5X6GMEqJZUwE6KZjXK3";
const char* privateKey = "BVA2qdkYB1I9vNZRGMZn";

float MoistureMax = 0.2;
float MoistureMin = 1.0;

float lastSentMoisture;

unsigned long lastSendMoistureTime;

#define ledPin 5
#define neoPixPin 5
#define pixelsNum 24

//Adafruit_NeoPixel ring = Adafruit_NeoPixel(pixelsNum, neoPixPin, NEO_GRB + NEO_KHZ800);

char buffer[25];

Phant phant(host, streamId, privateKey);

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin,OUTPUT);

  Serial.begin(9600);
  digitalWrite(ledPin, LOW);

  //while(!Serial);
  
  digitalWrite(ledPin, LOW);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(WiFi.localIP());

  digitalWrite(ledPin, HIGH);
  //ring.begin();
  //ring.setBrightness(64);
  meter(0.0);
  
}

void loop() {
  float moisture = iLerp( MoistureMin, MoistureMax, 1.0 * analogRead(A0) / 1024.0 );
  float diff = abs( moisture - lastSentMoisture );

  meter( moisture );

  Serial.print("Moisture=");
  Serial.print(analogRead(A0));
  Serial.print(" Moisture=");
  Serial.println( dtostrf( moisture, 4, 2, buffer) );
    
  unsigned long elapsed = millis() - lastSendMoistureTime;
  if( diff > 0.01 && elapsed > 30000 ){
    digitalWrite(ledPin, HIGH);
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      digitalWrite(ledPin, LOW);
      return;
    }
    Serial.println("sent moisture");
    phant.add("moisture", moisture );
  
    client.print(phant.post());
  
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    lastSentMoisture = moisture;
    lastSendMoistureTime = millis();
    delay(500);
  }
  
  delay(500);

}

void meter(float measure){
  /*
  measure = 1 - measure;
  for(int i=0;i<pixelsNum;i++){
    uint32_t c = 0;
    float p = 1.0 * i / pixelsNum ;
    if( p <= measure ){
      c = getColor(p);
    }
    ring.setPixelColor(i,c);
  }
  ring.show(); 
  */
}


