#include "mqtt.h"
#include <Arduino_JSON.h>

// bool ping = false;
// int count = 0;

unsigned long previousMillis = 0;    
const long interval = 50000;

//oftwareSerial mySerial(10,11);
//SoftwareSerial mySerial(14, 12);
Mqtt::Mqtt(bool displayMsg) {

}

void Mqtt::begin(int baudRate) {
 
 // Serial.begin(baudRate);  


 initAt();

http(); 
 initTCP();
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
    serverr =server;
    portt =port;
    clientId = s;
    withauth = auth;
    userr = user;
    pwdd = pswd;

   Serial.println("AT+CIPSHUT\r");
  delay(2000);
  byte co[] = {0x00, 0x04, 0x4d, 0x51, 0x54,
               0x54, 0x04, 0xc2, 0x00, 0x3c, 0x00};
  byte pwd[] = {0x00};

  int length = s.length()+user.length()+pswd.length()+16;
  if(!auth){
    co[7] = 0x02;
    length-= 4;

  }
  Serial.print("AT+CIPSTART=\"TCP\",\"");
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

  Serial.print("AT+CIPSEND\r");
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
  Serial.print("AT+CIPSEND=");
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
  Serial.println("AT+SAPBR=2,1\r");
  delay(1000);
  rr();
}

void Mqtt::initAt() {
  Serial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r");

  delay(1000);
   rr();
  Serial.println("AT+CIPSHUT\r");

  delay(1000);
   rr();
  Serial.println("AT+CIPSTATUS\r");
  delay(1000);
   rr();
  Serial.println("AT+CIPMUX=0\r");
  delay(1000);
   rr();
  Serial.println("AT+CSTT=\"internet.ooredoo.tn\"\r");
  delay(1000);
   rr();
  Serial.println("AT+CIPSPRT=0\r");
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
  Serial.println("AT+HTTPTERM\r");
  delay(1000);
  rr();
  Serial.println("AT+HTTPINIT\r");
  delay(1000);
  rr();
  Serial.println("AT+HTTPPARA=\"URL\",\"752fd59633b5.ngrok.io/espitems/4561\"\r");
  delay(1000);
  rr();
  Serial.println("AT+HTTPPARA=\"CID\",1\r");
  delay(1000);
  rr();
  Serial.println("AT+HTTPACTION=0\r");
  delay(2000);
  rr();
  while (Serial.available()<=0) {
  //Serial.print(".");
  delay(1000);
  }
  rr();
  Serial.println("AT+HTTPREAD\r");

  String s = Serial.readString();
  s= s.substring(s.indexOf("["),s.indexOf("]")+1);
  //Serial.print("response : => ");
  //Serial.println(s);
  Serial.println("AT+HTTPTERM\r");
  delay(1000);
  rr();

  JSONVar myObject = JSON.parse(s);
  if (JSON.typeof(myObject) == "undefined") {
  //Serial.println("Parsing input failed!");
  return;
  }
  for (int i=0; i<myObject.length(); i++) {
  if((String)((const char*)myObject[i]["action"])=="D"){
    pinMode((int)myObject[i]["pin"],OUTPUT);
    digitalWrite((int)myObject[i]["pin"],!(int)myObject[i]["value"]);
  }

  }
  Serial.println("AT+SAPBR=0,1\r");
  delay(2000);
  rr();


}
void Mqtt::getData(){
 while (true) {
      if (available()) {
         line[ lineIndex ] = '\0';                   
        lineIndex = 0;
        String s(line);
    //    Serial.println(line);
        if(s.indexOf("ok")>=0|| s.indexOf("ERROR")>=0|| s.indexOf("FAIL")>=0){
          break;
        }
      }
  }
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
            line[ lineIndex++ ] = c-'a'+'A';
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













