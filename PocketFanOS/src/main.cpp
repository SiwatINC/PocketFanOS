#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266_SSL.h>
char auth[] = "YourAuthToken";
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";
int thermistorconstant = 13;
int page = 1;
int fan1speed=0;
int fan2speed=0;
String serialmessage;
void ICACHE_RAM_ATTR rotR(){
  switch (page)
  {
  case 1:
    if(fan1speed<=255)fan1speed+=15;
    analogWrite(4,fan1speed);
    break;
  case 2:
    if(fan2speed<=255)fan2speed+=15;
    analogWrite(5,fan2speed);
  default:
    break;
  }
}
void ICACHE_RAM_ATTR rotL(){
  switch (page)
  {
  case 1:
    if(fan1speed>=0)fan1speed-=15;
    analogWrite(4,fan1speed);
    break;
  case 2:
    if(fan2speed>=0)fan2speed-=15;
    analogWrite(5,fan2speed);
  default:
    break;
  }
}
void ICACHE_RAM_ATTR btn(){
switch (page)
  {
  case 1:
    page=2;
    analogWrite(4,fan1speed);
    analogWrite(5,fan2speed);
    break;
  case 2:
    page=3;
    analogWrite(4,fan1speed);
    analogWrite(5,fan2speed);
    break;
  case 3:
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
  Serial.printf("Siwat INC PocketFanOS");
  Serial.printf("https://github.com/SiwatINC/PocketFanOS");
  Serial.printf("------ Commands ------");
  Serial.printf("[F1/F2].setspeed [SPEED FROM 0-255] | Set Fan Rotational Speed");
  Serial.printf("mode.set [MODE FROM 1-3] | Set the device's mode, 1 or 2 is Manual, Speed is control via setspeed 3 is Automatic, Speed is controlled via PID and Thermistor");
  Blynk.begin(auth, ssid, pass);
  attachInterrupt(digitalPinToInterrupt(12),rotR,RISING);
  attachInterrupt(digitalPinToInterrupt(13),rotL,RISING);
  attachInterrupt(digitalPinToInterrupt(14),btn,RISING);
}

void loop() {
  Blynk.run();
  if(page==3){
    analogWrite(4,thermistorconstant*analogRead(A0));
    analogWrite(5,thermistorconstant*analogRead(A0));
  }
  if(Serial.available()){
    serialmessage = Serial.readStringUntil('\n');
    if (serialmessage.startsWith("F1.setspeed ")){
      serialmessage.remove(0,12);
      fan1speed=serialmessage.toInt();
      analogWrite(4,fan1speed);
    }
    else if (serialmessage.startsWith("F2.setspeed ")){
      serialmessage.remove(0,12);
      fan2speed=serialmessage.toInt();
      analogWrite(5,fan2speed);
    }
    else if (serialmessage.startsWith("mode.set ")){
      serialmessage.remove(0,9);
      page=serialmessage.toInt();
    } else if (serialmessage.startsWith("F1.getspeed")){
      Serial.print(fan1speed);
    }else if (serialmessage.startsWith("F2.getspeed"))
    {
      Serial.print(fan2speed);
    } else if (serialmessage.startsWith("mode.get")){
      Serial.print(page);
    }
  }
}