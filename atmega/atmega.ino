#include "mqtt.h"


Mqtt mqtt(false);


int publishInterval = 1500; 
long lastPublishMillis=0;

void setup() {
   Serial.begin(9600);  


      
   while(!getModule(3));

   if(!mqtt.isWifi){
      mqtt.begin(9600);
      mqtt.http();
      mqtt.connect(F("6.tcp.ngrok."), "io", "16806",
      "cc",false/*, true, "use-token-auth", "g+fjh-hUsMx8lqw!2i"*/);
      delay(6000);
      mqtt.subscribe("iot-2/4561");
      delay(4000);
      mqtt.publish("iot-2/4561", F("{\"action\":\"D\",\"pin\":13,\"value\":1,\"options\":50}"));


   }else{

      //  String s = Serial.readString();
      //  Serial.println(s);
        mqtt.setAll();
    
   }
}

void loop() {
  if (mqtt.available()) {
    String line = mqtt.readString();
    if (line == "ERROR") {
      //Serial.println(line);
    } else {
     Serial.println(line);
     mqtt.setActions(line);
    }
  }
    if (millis() - lastPublishMillis > publishInterval) {
    mqtt.CheckData();
    lastPublishMillis = millis();
  }
}


bool getModule(int timeout){
       Serial.println("at");
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
    mqtt.isWifi = false;
    return true;
  }else if(s.indexOf("wifi")>=0){
    mqtt.isWifi = true;
    return true;
  }
  return false;
}



