#include "mqtt.h"
#include <Arduino_JSON.h>

Mqtt mqtt(false);
bool isWifi= false;

void setup() {
   Serial.begin(9600);  
   delay(1000);
   Serial.println("at");
   while(!getModule(3));

   if(!isWifi){
      mqtt.begin(9600);
      mqtt.connect("6.tcp.ngrok.", "io", "16806",
      "cc",false/*, true, "use-token-auth", "g+fjh-hUsMx8lqw!2i"*/);
      delay(6000);
      mqtt.subscribe("iot-2/4561");
      delay(4000);
      mqtt.publish("iot-2/4561", "{\"action\":\"D\",\"pin\":13,\"value\":1,\"options\":50}");


   }else{
       while (Serial.available()<=0) {
       }
       String s= Serial.readStringUntil('\n');
       Serial.print(s);
   }
}

void loop() {
  if (mqtt.available()) {
    String line = mqtt.readString();
    if (line == "ERROR") {
      //Serial.println(line);
    } else {
     // Serial.println(line);
     setActions(line);
    }
  }
}

void setActions(String line){
  JSONVar myObject = JSON.parse(line);
  if (JSON.typeof(myObject) == "undefined") {
    //Serial.println("Parsing input failed!");
    return;
  }

  if((String)((const char*)myObject["ACTION"])=="D"){
    pinMode((int)myObject["PIN"],OUTPUT);
    digitalWrite((int)myObject["PIN"],!(int)myObject["VALUE"]);
  }
}

bool getModule(int timeout){
   int timeOut = 0;
  while (Serial.available()<=0) {
    timeOut++;
    if(timeOut>=timeout){
      return false;
    }
    delay(1000);
  }
   String s= Serial.readStringUntil('\n');
  if(s.indexOf("OK")>=0){
    isWifi = false;
    return true;
  }else if(s.indexOf("wifi")>=0){
    isWifi = true;
    return true;
  }
  return false;
}


