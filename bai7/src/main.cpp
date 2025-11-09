#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <string.h>
const char* ssid = "TP-Link_9430";
const char* password = "51563103" ;
 
String UrlThingspeak = "https://api.thingspeak.com/update?api_key=XD9PYBRVMOWTH9V2";
String httpGETRequest(const char* Url);
void setup() {
  Serial.begin(115200);
  Serial.println(F("Status to Thingspeak!")); 
  //setup wifi
  WiFi.begin(ssid,password);
  Serial.println("conecting");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
} 
void loop() {
    // Wait a few seconds between measurements.
    delay(1000);
    char para[50];
    int So = random(1,5);
    sprintf(para,"&field1=%d",So);
    Serial.println(So);
    String Url = UrlThingspeak + String(para);
    httpGETRequest(Url.c_str());
} 
String httpGETRequest(const char* Url)
{
  HTTPClient http;
  http.begin(Url);
  int responseCode = http.GET();
  String responseBody = "{}";
  if(responseCode > 0)
  {
    Serial.print("responseCode:");
    Serial.println(responseCode);
    responseBody = http.getString();
  }
  else
  {
    Serial.print("Error Code: ");
    Serial.println(responseCode);
  }
  http.end();
  return responseBody;
}