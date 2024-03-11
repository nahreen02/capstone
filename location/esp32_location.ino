#include <Arduino.h>
#if defined ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#error Wrong platform
#endif 

#include <WifiLocation.h>

#if __has_include("wificonfig.h")
#include "wificonfig.h"
#else
const char* googleApiKey = "";
const char* ssid = "";
const char* passwd = "";
#endif

WifiLocation location (googleApiKey);
float lon, lat;
location_t loc;

String getClock () {
    configTime (0, 0, "pool.ntp.org", "time.nist.gov");
    time_t now = time (nullptr);
    while (now < 8 * 3600 * 2) {
        delay (500);
        now = time (nullptr);
    }
    struct tm timeinfo;
    gmtime_r (&now, &timeinfo);
    return (asctime (&timeinfo));
}

void setup() {
    Serial.begin(115200);
    // Connect to WPA/WPA2 network
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, passwd);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);
        // wait 5 seconds for connection:
        Serial.print("Status = ");
        Serial.println(WiFi.status());
        delay(500);
    }
    Serial.println ("Connected");
    Serial.println(getClock());
    
    //location_t loc = location.getGeoFromWiFi();

    // Serial.println("Location request data");
    // Serial.println(location.getSurroundingWiFiJson()+"\n");
    // Serial.println ("Location: " + String (loc.lat, 7) + "," + String (loc.lon, 7));
    // //Serial.println("Longitude: " + String(loc.lon, 7));
    // Serial.println ("Accuracy: " + String (loc.accuracy));
    // Serial.println ("Result: " + location.wlStatusStr (location.getStatus ()));

}

void loop() {
  updateValues();
  Serial.println(getLon(), 6); 
  Serial.println(getLat(), 6); 
  delay(60000);
}

void updateValues(){
  loc = location.getGeoFromWiFi();
}

float getLon() {
  float lon = loc.lon; 
  return lon; 
}

float getLat() {
  float lat = loc.lat; 
  return lat; 
}
