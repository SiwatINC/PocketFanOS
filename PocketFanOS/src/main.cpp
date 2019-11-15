#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266_SSL.h>
char auth[] = "YourAuthToken";
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";
int page = 1;
int fan1speed=0;
int fan2speed=0;
long oldPosition  = -999;
void ICACHE_RAM_ATTR rotR(){
  switch (page)
  {
  case 1:
    if(fan1speed<=255)fan1speed+=15;
    
    break;
  case 2:
    if(fan2speed<=255)fan2speed+=15;
  default:
    break;
  }
}
void ICACHE_RAM_ATTR rotL(){
  switch (page)
  {
  case 1:
    if(fan1speed>=0)fan1speed-=15;
    break;
  case 2:
    if(fan2speed>=0)fan2speed-=15;
  default:
    break;
  }
}
void ICACHE_RAM_ATTR btn(){
switch (page)
  {
  case 1:
    page=2;
    break;
  case 2:
    page=1;
    break;
  default:
    page=1;
    break;
  }
}
void setup() {
  Serial.begin(115200);
  pinMode(12,INPUT);
  pinMode(12,INPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  Blynk.begin(auth, ssid, pass);
  attachInterrupt(digitalPinToInterrupt(12),rotR,RISING);
  attachInterrupt(digitalPinToInterrupt(13),rotL,RISING);
}

void loop() {
  Blynk.run();
}