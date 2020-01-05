#include <Arduino.h>
#include "Thing.h"
#include "WebThingAdapter.h"

const char *ssid = "WIFI Name";
const char *password = "WIFI PASSWORD";

const int button = 23;
const int led = 2;

WebThingAdapter *adapter;

const char *PushButtonTypes[] = {"PushButton",nullptr}; // define a type of device referenc to context https://iot.mozilla.org/schemas
ThingDevice Pushbt("PushButton","Pushbutton0",PushButtonTypes); //configure device id, name and type
ThingProperty PushBtOn("push","button is pressed",BOOLEAN,"PushedProperty"); // device property of device, name, text, boolen type


bool state = false;

void detectpush()
{
  if(digitalRead(button) == HIGH)
  {
    state = true;
  }
  else
  {
    state = false;
  }
  //Serial.println(digitalRead(button));
}

void setup() {
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  Serial.println("");
  Serial.print("Connection to \"");
  Serial.print(ssid);
  Serial.println("\"");

  #if defined(ESP8266) || defined(ESP32)
    WiFi.mode(WIFI_STA);
  #endif
    WiFi.begin(ssid, password);
    Serial.println("");
  
  // Wait for connection
  bool blink = true;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(led, blink ? LOW : HIGH); // active low led
    blink = !blink;
  }
  digitalWrite(led, HIGH); // active low led

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  adapter = new WebThingAdapter("pushbutton", WiFi.localIP());

  Pushbt.addProperty(&PushBtOn);
  adapter->addDevice(&Pushbt);
  adapter->begin();
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(Pushbt.id);
}

void loop()  {
  detectpush();
  ThingPropertyValue value;
  
  value.boolean = state;
  PushBtOn.setValue(value);
  adapter->update();
  delay(300);

}
