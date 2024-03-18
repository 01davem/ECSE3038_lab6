#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid ;
const char* password; 
const char* apiKey; 

const char* host = "ecse3038-temp-light-lab.onrender.com";

int ledPin = 2; // Define the pin for the LED

void setup() {
  Serial.begin(921600);
  pinMode(ledPin, OUTPUT);
  
  connectToWiFi();
}

void loop() {
  makeGetRequest();
  delay(5000); // Delay between requests
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  
  Serial.println("Connected to WiFi");
}

void makeGetRequest() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    Serial.print("Sending GET request...");
    String endpoint = "/api/light";
    String url = "http://" + String(host) + endpoint;
    
    http.begin(url);
    http.addHeader("api-key", apiKey);
    
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response:");
      Serial.println(response);
      
      DynamicJsonDocument doc(256);
      deserializeJson(doc, response);
      
      bool lightState = doc["light"];
      
      if (lightState) {
        digitalWrite(ledPin, HIGH); // Turn on LED
      } else {
        digitalWrite(ledPin, LOW); // Turn off LED
      }
    } else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

