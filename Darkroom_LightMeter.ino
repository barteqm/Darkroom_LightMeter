
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
//#include <BH1750FVI.h>
//BH1750FVI eye;

#include <SparkFunTSL2561.h>
#include <Wire.h>
// Create an SFE_TSL2561 object, here called "light":
SFE_TSL2561 light;

#include "progmem.h"
#include "config.h" // variables and constants declarations
#include "http.h"
#include "lightmeter.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  // eye.begin(BH_Conth, BH_AddrL, 254);
  // eye.begin();
  Serial.println("Configuring Sensor TSL...");
  configureSensor();
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/readVals", readVals);
  server.on("/setParams", setParams);
  server.on("/red", setRed);
  server.on("/white", setWhite);
  server.on("/timerLock", setTimerLock);
  server.on("/timerStart", setTimerStart);
  server.begin();
  Serial.println("HTTP server started");
  // input config
  pinMode(RED_pin, OUTPUT);
  pinMode(GREEN_pin, OUTPUT);
  pinMode(BLUE_pin, OUTPUT);
  pinMode(TIMER_pin, OUTPUT);
  digitalWrite(RED_pin, 0);
  digitalWrite(GREEN_pin, 0);
  digitalWrite(BLUE_pin, 0);
  digitalWrite(TIMER_pin, 0);
}

void loop() {
  unsigned long currentMillis = millis();
  // interval loop
  if (currentMillis - previousMillis >= interval & !Timer) {
    previousMillis = currentMillis;
    if (false) {
        Serial.println("Measurement Start..");
   //   light.manualStart();
    
    } else {
      // if the LED is off turn it on and vice-versa:
   //   light.manualStop();
      
      lux = getLux();
      EV = lux2ev(lux);
      if (!TimerLock or Timer) {
        T = (100 * pow(A, 2) / iso / pow(2, EV + EVcorr));
        TimerCounter = T * 1000; //counter in milliseconds
      };
      //T = exposure time, in &nbsp;seconds
      //T = 100 * pow(A, 2) / iso / pow(2, EV);
             Serial.println("Measurement Stop - Values:");
      Serial.print("LUX: ");
      Serial.print(lux);
      Serial.print(" EV: ");
      Serial.print(EV);
      Serial.print("  T: ");
      Serial.print(T);
      Serial.print("  A: ");
      Serial.print(A);
      Serial.print("  ISO: ");
      Serial.print(iso);
      Serial.print("  EVCorr ");
      Serial.println(EVcorr);

      Serial.println(measure);
    }
    if (RED_light) {
      digitalWrite(RED_pin, 1);
      digitalWrite(GREEN_pin, 0);
      digitalWrite(BLUE_pin, 0);
    }
    if (WHITE_light) {
      digitalWrite(RED_pin, 1);
      digitalWrite(GREEN_pin, 1);
      digitalWrite(BLUE_pin, 1);

    }
    measure = !measure;
  } else if (currentMillis - previousMillis >= timer_interval & Timer)
  {
    previousMillis = currentMillis;
    TimerCounter = TimerCounter - timer_interval;
    Serial.println(TimerCounter);
    if (TimerCounter <= 0) {
      Timer = false;
      TimerCounter = T * 1000;
    };
    if (Timer) {
      digitalWrite(TIMER_pin, true);
      //  digitalWrite(RED_pin, 0);
      digitalWrite(GREEN_pin, 0);
      digitalWrite(BLUE_pin, 0);
    } else {
      digitalWrite(TIMER_pin, false);
    };
  }

  server.handleClient();

}
