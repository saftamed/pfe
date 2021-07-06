#include "mqtt.h"
#include <Arduino_JSON.h>


// bool ping = false;
// int count = 0;
int ii =0;

input record[8];
unsigned long previousMillis = 0;    
const long interval = 50000;

//oftwareSerial mySerial(10,11);
//SoftwareSerial mySerial(14, 12);
Mqtt::Mqtt(bool displayMsg) {

}

void Mqtt::begin(int baudRate) {
 
 // Serial.begin(baudRate);  


 initAt();

//http(); 
 //initTCP();
  /*
  cli();                     
  TCCR1A = 0;               
  TCCR1B = 0;                
  TCCR1B |= B00000101;  
  TIMSK1 |= B00000010;       
  OCR1A = 15624;            
  sei();   */
}
/*
ISR(TIMER1_COMPA_vect){
  TCNT1  = 0; 
  count++;
  if(count >= 50){
    ping = true;
    count = 0;
  }

}*/

void Mqtt::connect(String server,String server1,String port,String s,bool auth,String user,String pswd) {
   initTCP();
    serverr =server;
    portt =port;
    clientId = s;
    withauth = auth;
    userr = user;
    pwdd = pswd;

   Serial.println(F("AT+CIPSHUT\r"));
  delay(2000);
  byte co[] = {0x00, 0x04, 0x4d, 0x51, 0x54,
               0x54, 0x04, 0xc2, 0x00, 0x3c, 0x00};
  byte pwd[] = {0x00};

  int length = s.length()+user.length()+pswd.length()+16;
  if(!auth){
    co[7] = 0x02;
    length-= 4;

  }
  Serial.print(F("AT+CIPSTART=\"TCP\",\""));
  delay(1000);

  Serial.print(server);
  delay(1000);
  rr();
    Serial.print(server1);
  delay(1000);
  rr();
   Serial.print("\",\"");
  delay(1000);
 rr();
  Serial.print(port);
  delay(1000);
 rr();
  Serial.println("\"\r");

  delay(5000);
 rr();

  Serial.print(F("AT+CIPSEND\r"));
  delay(3000);

 rr();
  Serial.write(0x10);
  Serial.write(length);
  Serial.write(co, 11);

  Serial.write(s.length());
  Serial.print(s);
 
  if(auth){
    Serial.write(pwd, 1);
    Serial.write(user.length());
    Serial.print(user);
    Serial.write(pwd, 1);  

    Serial.write(pswd.length());
    Serial.print(pswd);
  }


  delay(1000);
 rr();
  // mySerial.write(0x1a);
  Serial.println("\r");
  delay(1000);
 rr();
}
void Mqtt::publish(String topic,  String msg) {
  int length = 5 + topic.length() + msg.length();
  byte pu[] = {0x00};
  Serial.print(F("AT+CIPSEND="));
  Serial.print(length-1);
  Serial.print("\r");
  delay(3000);
   rr();
  Serial.write(0x31);
  Serial.write(length - 3);
  Serial.write(pu, 1);
  Serial.write(topic.length());
  Serial.print(topic);
  Serial.print(msg);
  delay(1000);
   rr();
  Serial.write(0x1a);
  delay(1000);
   rr();
}
void Mqtt::subscribe(String topic) {
  topicc =topic;
  int length = 7 + topic.length();
  byte su[] = {0x9b, 0x9c, 0x00};
  byte s[] = {0x00};
  Serial.print("AT+CIPSEND=");
  Serial.print(length);
  Serial.print("\r");
  delay(1000);
   rr();
  Serial.write(0x82);
  Serial.write(length - 2);
  Serial.write(su, 3);
  Serial.write(topic.length());
  Serial.print(topic);
  Serial.write(s, 1);
  Serial.write(0X1a);
  delay(3000);
   rr();
   connected = true;
}
void Mqtt::initTCP(){
  sendCmd("AT+CIICR\r",10);
  // mySerial.println("AT+CIICR\r");
  // delay(5000);
  // rr();
  Serial.println("AT+CIFSR\r");
  delay(1000);
  rr();  
}
bool Mqtt::sendCmd(String cmd,int timeout){
  Serial.println(cmd);
  timeOut = 0;
  while (Serial.available()<=0) {
    timeOut++;
    if(timeOut>=timeout){
      return false;
    }
    delay(1000);
    Serial.print(".");
  }
  String s = Serial.readString();
  //Serial.println(s);
 if(s.indexOf("ok")>=0){
   return true;
  }
  return false;
}
void Mqtt::initHTTP(){
  sendCmd("AT+SAPBR=1,1\r",10);
  // mySerial.println("AT+SAPBR=1,1\r");
  // delay(5000);
  // rr();
  Serial.println(F("AT+SAPBR=2,1\r"));
  delay(1000);
  rr();
}

void Mqtt::initAt() {
  Serial.println(F("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r"));

  delay(1000);
   rr();
  Serial.println(F("AT+CIPSHUT\r"));

  delay(1000);
   rr();
  Serial.println(F("AT+CIPSTATUS\r"));
  delay(1000);
   rr();
  Serial.println(F("AT+CIPMUX=0\r"));
  delay(1000);
   rr();
  Serial.println(F("AT+CSTT=\"internet.ooredoo.tn\"\r"));
  delay(1000);
   rr();
  Serial.println(F("AT+CIPSPRT=0\r"));
  delay(1000);
   rr();
}
void Mqtt::rr() {
  while (Serial.available() >= 1) {
    Serial.read();
  }
}
void Mqtt::http(){
  initHTTP();
  Serial.println(F("AT+HTTPTERM\r"));
  delay(1000);
  rr();
  Serial.println(F("AT+HTTPINIT\r"));
  delay(1000);
  rr();
  Serial.println(F("AT+HTTPPARA=\"URL\",\"752fd59633b5.ngrok.io/espitems/4561\"\r"));
  delay(1000);
  rr();
  Serial.println(F("AT+HTTPPARA=\"CID\",1\r"));
  delay(1000);
  rr();
  Serial.println(F("AT+HTTPACTION=0\r"));
  delay(2000);
  rr();
  while (Serial.available()<=0) {
  //Serial.print(".");
  delay(1000);
  }
  rr();
  Serial.println("AT+HTTPREAD\r");

  setAll();
  //Serial.print("response : => ");
  //Serial.println(s);
  Serial.println("AT+HTTPTERM\r");
  delay(1000);
  rr();

 
  Serial.println("AT+SAPBR=0,1\r");
  delay(2000);
  rr();

 


}
void Mqtt::getData(){

}
void Mqtt::sendPing(){

  if(connected){
  byte co[] = {0xc0,0x00,0x1a};
  Serial.print("AT+CIPSEND\r");
  delay(2000);
  Serial.write(co,3);
  //Serial.println("ping");
  }
}
bool Mqtt::available(){
      // if(ping){
      //   ping = false;
      //   sendPing();
      //   count = 0;
      // }
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    sendPing();
    previousMillis = currentMillis;


  }


     while ( Serial.available()>0 ) {
     char  c = Serial.read();
     //Serial.print(c);
      if (/*( c == 0x0d) ||*/ ( c == 0x0a)) {  
        if ( lineIndex > 2 ) {    
          return true; 
        }else {
            line[ 0 ] = '\0';              
            lineIndex = 0;
         return false;
        }      

          //checkResponse();
        }else if ( c == '}'){
          line[ lineIndex++ ] = c;      
          return true;                                   
          //checkResponse(true);

        } else {
          if ( lineIndex >= LINE_BUFFER_LENGTH-1 ) {
            //Serial.println("overflow");
             lineIndex = 0;
          } 
          else if ( c >= 'a' && c <= 'z' ) {        
            line[ lineIndex++ ] = c;
          } 
          else if ( (c >= '0' && c <= '9') ||(c >= 'A' && c <= 'Z') || c==':'|| c=='{'|| c==0x22|| c==',') {
            line[ lineIndex++ ] = c;
          }
        return false; 
      }
    }
    return false; 
}



String Mqtt::readString(){
    if ( lineIndex > 0 ) {                        
        line[ lineIndex ] = '\0';                   
        lineIndex = 0;
        String s(line);
        if(s.indexOf("IOT")>=0){
          return s.substring(s.indexOf("{"));
        }else if(s.indexOf("OK")>=0){
          return "OK";
        }else if(s.indexOf("CLOSED")>=0 || s.indexOf("ERROR")>=0|| s.indexOf("FAIL")>=0){
              //reconnect
                 connected = false;
              connect(serverr, "ernetofthings.ibmcloud.com",portt,clientId,withauth,userr,pwdd);
              delay(2000);
              subscribe(topicc);
              line[ 0 ] = '\0';
              return "ERROR";
        }
    }else {
        line[ 0 ] = '\0';                   
        lineIndex = 0;
    }
  return "";
}

void Mqtt::sendData(input r){
 // Serial.println("send");
  String payload = "{\"action\":\"";
  payload += r.action;
  payload += "\",\"pin\":";
  payload += String(r.pin, DEC);
  payload += ",\"value\":\"";
  payload += String(r.value, DEC);
  payload += "\"}";



  if (isWifi) {
    Serial.println(payload);
    //digitalWrite(2,HIGH);
  } else {
     publish("iot-2/4561", payload);
  }
}
void Mqtt::CheckData() {
  // read the input on analog pin 0:

  for(int i=0;i<ii;i++){
    int v = 0;
    if(record[i].action == "A"){
      v= map(analogRead(record[i].pin),0,1023,0,100);
      if(v >record[i].value + 5 || v<record[i].value -5){
        record[i].value = v;
        sendData(record[i]);
      }
    }else if(record[i].action == "I"){

      v= digitalRead(record[i].pin);

           
      if(v != record[i].value){
        record[i] = {  record[i].pin,v,"I"};
        sendData(record[i]);
         Serial.println(record[i].value);
      }

    }
  }
 
}
void Mqtt::setAll(){
       while (Serial.available()<=0) {
       }

  char  c = Serial.read();

  if(!isWifi){
    Serial.readStringUntil('[');
  }
  while ( true ) {
      String s = Serial.readStringUntil('}');
        while (Serial.available()<=0) {
        }
          char  c = Serial.read();
          s+="}";

        //Serial.println(s);
        setActions(s);
          if(c == ']')
          {
            break;
          }
      

  }


}



void Mqtt::setActions(String line){
  JSONVar obj = JSON.parse(line);
  if (JSON.typeof(obj) == "undefined") {
    return;
  }
    String ss = (String)((const char*)obj["action"]);
    Serial.println(ss);

    if(ss=="D"){
      pinMode((int)obj["pin"],OUTPUT);
      String s =(const char*) obj["value"];
      digitalWrite((int)obj["pin"],s.toInt());
    }else if(ss=="P"){
      String s =(const char*) obj["value"];
    //s.trim();
      analogWrite((int)obj["pin"],map( s.toInt(), 0, 100, 0, 1023));
    }else if(ss=="A"){
        int ss =getIndex((int)obj["pin"]);
        record[ss]={ (int)obj["pin"],record[ss].value, "A"};            
    }else if(ss=="I"){
        pinMode( (int)obj["pin"],INPUT);
        int ss =getIndex( (int)obj["pin"]);
        record[ss]={  (int)obj["pin"],record[ss].value, "I"};
    }else{
      
    }
  
  

 
}
int Mqtt::getIndex(int pin){
   for(int i=0;i<ii;i++){
     if(pin == record[i].pin)
      return i;
   }
   return ii++;
}









