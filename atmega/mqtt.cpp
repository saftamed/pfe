#include "mqtt.h"

#include "Wire.h"
#include "LiquidCrystal_I2C.h"

#include <Servo.h>



LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo ser[3];

#define DS3231_I2C_ADDRESS 0x68

const char AT_CMD_SEND[] = "AT+CIPSEND=";
const char SERVER[] = "0.tcp.ngrok.io";
const char PORT[] = "12396";
const char TOPIC[] = "iot-2/";
const char TOKEN[] = "4561";
// bool ping = false;
// int count = 0;
int ii = 0;

input record[10];

unsigned long previousMillis = 0;
const long interval = 50000;

//oftwareSerial mySerial(10,11);
//SoftwareSerial mySerial(14, 12);
Mqtt::Mqtt(bool displayMsg) {
  beginDebug();
 // print("hi");
}

void Mqtt::beginDebug() {
  lcd.init();  // initialize
  lcd.backlight();
}
void Mqtt::print(String msg, int l = 1) {
  if (!debug) return;
  lcd.setCursor(l, 0);  //(C,R)
  lcd.print(msg);
  delay(100);
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

void Mqtt::connect() {
  initTCP();
  print("Try To Coonect");
  Serial.println(F("AT+CIPSHUT\r"));
  delay(2000);
  const byte co[] = { 0x00, 0x04, 0x4d, 0x51, 0x54,
                      0x54, 0x04, 0xc2, 0x00, 0x3c, 0x00 };
  const byte pwd[] = { 0x00 };

  int length = /*+user.length()+pswd.length()*/ 20;
  // if(!auth){
  //   co[7] = 0x02;
  //   length-= 4;

  // }
  Serial.print(F("AT+CIPSTART=\"TCP\",\""));
  delay(1000);

  Serial.print(SERVER);
  delay(1000);
  rr();
  //   Serial.print(server1);
  // delay(1000);
  // rr();
  Serial.print("\",\"");
  delay(1000);
  rr();
  Serial.print(PORT);
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

  Serial.write(4);
  Serial.print(TOKEN);

  // if(auth){
  //   Serial.write(pwd, 1);
  //   Serial.write(user.length());
  //   Serial.print(user);
  //   Serial.write(pwd, 1);

  //   Serial.write(pswd.length());
  //   Serial.print(pswd);
  // }


  delay(1000);
  rr();
  // mySerial.write(0x1a);
  Serial.println("\r");
  delay(1000);
  rr();
}
void Mqtt::publish(String msg) {
  int length = 15 + msg.length();
  const byte pu[] = { 0x00 };
  Serial.print(AT_CMD_SEND);
  Serial.print(length - 1);
  Serial.print("\r");
  delay(3000);
  rr();
  Serial.write(0x31);
  Serial.write(length - 3);
  Serial.write(pu, 1);
  Serial.write(10);
  Serial.print(TOPIC);
  Serial.print(TOKEN);
  Serial.print(msg);
  delay(1000);
  rr();
  Serial.write(0x1a);
  delay(1000);
  rr();
}
void Mqtt::subscribe() {
  int length = 17;
  const byte su[] = { 0x9b, 0x9c, 0x00 };
  const byte s[] = { 0x00 };
  Serial.print(AT_CMD_SEND);
  Serial.print(length);
  Serial.print("\r");
  delay(1000);
  rr();
  Serial.write(0x82);
  Serial.write(length - 2);
  Serial.write(su, 3);
  Serial.write(10);
  Serial.print(TOPIC);
  Serial.print(TOKEN);
  Serial.write(s, 1);
  Serial.write(0X1a);
  delay(3000);
  rr();
  connected = true;
}
void Mqtt::initTCP() {
  sendCmd("AT+CIICR\r", 10);
  // mySerial.println("AT+CIICR\r");
  // delay(5000);
  // rr();
  sendCmd("AT+CIFSR\r", 1);
}
bool Mqtt::sendCmd(String cmd, int timeout) {
  Serial.println(cmd);
  timeOut = 0;
  while (Serial.available() <= 0) {
    timeOut++;
    if ((timeOut / 10) > timeout) {
      return false;
    }
    delay(100);
    //Serial.print(".");
  }
  String s = Serial.readString();
  //Serial.println(s);
  if (s.indexOf("ok") >= 0) {
    return true;
  }
  return false;
}
void Mqtt::initHTTP() {
  sendCmd("AT+SAPBR=1,1\r", 10);
  // mySerial.println("AT+SAPBR=1,1\r");
  // delay(5000);
  // rr();
  sendCmd(F("AT+SAPBR=2,1\r"), 1);
}

void Mqtt::initAt() {
  sendCmd(F("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r"), 1);
  sendCmd(F("AT+CIPSHUT\r"), 1);
  sendCmd(F("AT+CIPSTATUS\r"), 1);
  sendCmd(F("AT+CIPMUX=0\r"), 1);
  sendCmd(F("AT+CSTT=\"internet.ooredoo.tn\"\r"), 1);
  sendCmd(F("AT+CIPSPRT=0\r"), 1);
}
void Mqtt::rr() {
  while (Serial.available() >= 1) {
    Serial.read();
  }
}
void Mqtt::http() {
  initHTTP();
  sendCmd(F("AT+HTTPTERM\r"), 1);
  sendCmd(F("AT+HTTPINIT\r"), 1);
  sendCmd(F("AT+HTTPPARA=\"URL\",\"5c64275f4ca2.ngrok.io/espitems/4561\"\r"), 1);
  sendCmd(F("AT+HTTPPARA=\"CID\",1\r"), 1);
  sendCmd(F("AT+HTTPACTION=0\r"), 10);
  Serial.println("AT+HTTPREAD\r");

  setAll();
  sendCmd("AT+HTTPTERM\r", 1);
  sendCmd("AT+SAPBR=0,1\r", 2);
}
void Mqtt::getData() {
}
void Mqtt::sendPing() {

  if (connected) {
    byte co[] = { 0xc0, 0x00, 0x1a };
    Serial.print("AT+CIPSEND\r");
    delay(2000);
    Serial.write(co, 3);
    //Serial.println("ping");
  }
}
bool Mqtt::available() {
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


  while (Serial.available() > 0) {
    char c = Serial.read();
    //Serial.print(c);
    if (/*( c == 0x0d) ||*/ (c == 0x0a)) {
      if (lineIndex > 2) {
        return true;
      } else {
        line[0] = '\0';
        lineIndex = 0;
        return false;
      }

      //checkResponse();
    } else if (c == '}') {
      line[lineIndex++] = c;
      return true;
      //checkResponse(true);

    } else {
      if (lineIndex >= LINE_BUFFER_LENGTH - 1) {
        //Serial.println("overflow");
        lineIndex = 0;
      } else if (c >= 'a' && c <= 'z') {
        line[lineIndex++] = c;
      } else if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || c == ':' || c == '{' || c == 0x22 || c == ',') {
        line[lineIndex++] = c;
      }
      return false;
    }
  }
  return false;
}



String Mqtt::readString() {
  if (lineIndex > 0) {
    line[lineIndex] = '\0';
    lineIndex = 0;
    String s(line);
    if (s.indexOf("IOT") >= 0 || s.indexOf("iot") >= 0) {
      return s.substring(s.indexOf("{"));
    } else if (s.indexOf("OK") >= 0) {
      return "OK";
    } else if (s.indexOf("CLOSED") >= 0 || s.indexOf("ERROR") >= 0 || s.indexOf("FAIL") >= 0) {
      //reconnect
      connected = false;
      connect();
      delay(2000);
      subscribe();
      line[0] = '\0';
      return "ERROR";
    }
  } else {
    line[0] = '\0';
    lineIndex = 0;
  }
  return "";
}

void Mqtt::sendData(input r) {
  // Serial.println("send");
  String payload = F("{\"action\":\"");
  payload += r.action;
  payload += F("\",\"pin\":");
  payload += String(r.pin, DEC);
  payload += F(",\"value\":\"");
  payload += String(r.value, DEC);
  payload += "\"}";

  print("Send Data" + r.action);

  if (isWifi) {
    Serial.println(payload);
    //digitalWrite(2,HIGH);
  } else {
    publish(payload);
  }
}
void Mqtt::CheckData() {
  // read the input on analog pin 0:

  for (int i = 0; i < ii; i++) {
    int v = 0;
    if (record[i].action == 'A' || record[i].action == 'C') {
      v = map(analogRead(record[i].pin), 0, 1023, 0, 100);
      if (v > record[i].value + 5 || v < record[i].value - 5) {
        record[i].value = v;
        sendData(record[i]);
      }
    } else if (record[i].action == 'I') {

      v = digitalRead(record[i].pin);


      if (v != record[i].value) {
        record[i] = { record[i].pin, v, 'I' };
        sendData(record[i]);
        //  Serial.println(record[i].value);
      }

    } else if (record[i].action == 'T' || record[i].action == 'D') {
      Wire.beginTransmission(DS3231_I2C_ADDRESS);
      Wire.write(0);  // set DS3231 register pointer to 00h
      Wire.endTransmission();

      // request seven bytes of data from DS3231 starting from register 00h
      Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
      byte second = bcdToDec(Wire.read() /*& 0x7f*/);
      byte minute = bcdToDec(Wire.read());
      byte hour = bcdToDec(Wire.read() /*& 0x3f*/);
      byte dayOfWeek = bcdToDec(Wire.read());
      byte dayOfMonth = bcdToDec(Wire.read());
      byte month = bcdToDec(Wire.read());
      byte year = bcdToDec(Wire.read());

      //        // send it to the serial monitor
      // Serial.print(hour, DEC);
      // // convert the byte variable to a decimal number when displayed
      // Serial.print(":");
      // if (minute < 10)
      // {
      //   Serial.print("0");
      // }

      // Serial.println(minute, DEC);

      byte off = 0;
      if (record[i].value == 1) {
        off = 3;
      }



      if (dayOfWeek == record[i].data[0 + off] || record[i].data[0 + off] > 8) {
        if (hour == record[i].data[1 + off]) {
          if (minute == record[i].data[2 + off] + record[i].value) {
            record[i].value = !record[i].value;
            digitalWrite(record[i].pin, record[i].value);
            sendData(record[i]);
          }
        }
      }
    }
  }
}
void Mqtt::setAll() {
  while (Serial.available() <= 0) {
  }

  char c = Serial.read();

  if (!isWifi) {
    Serial.readStringUntil('[');
  }
  while (true) {
    String s = Serial.readStringUntil('}');
    while (Serial.available() <= 0) {
    }
    char c = Serial.read();
    s += "}";

    //Serial.println(s);
    setActions(s);
    if (c == ']') {
      break;
    }
  }
}



void Mqtt::setActions(String line) {
  JSONVar obj = JSON.parse(line);
  if (JSON.typeof(obj) == "undefined") {
    return;
  }
  String ss = (String)((const char*)obj["action"]);
  Serial.println(ss);
  print(ss + ": " + (const char*)obj["pin"] + " = " + (const char*)obj["value"]);
  if (ss == "D") {
    pinMode((int)obj["pin"], OUTPUT);
    String s = (const char*)obj["value"];
    digitalWrite((int)obj["pin"], s.toInt());
    TimeSet(obj);
  } else if (ss == "P") {
    String s = (const char*)obj["value"];
    if ((String)((const char*)obj["o"]) == "S") {
      if (!ser[(int)obj["pin"] - 9].attached()) {
        ser[(int)obj["pin"] - 9].attach((int)obj["pin"]);
      }
      ser[(int)obj["pin"] - 9].write(s.toInt());
    } else {
      ser[(int)obj["pin"] - 9].detach();
      analogWrite((int)obj["pin"], map(s.toInt(), 0, 100, 0, 1023));
      TimeSet(obj);
    }
  } else if (ss == "A" || ss == "C") {
    int ss = getIndex((int)obj["pin"]);
    record[ss] = { (int)obj["pin"], record[ss].value, ss == "A"? 'A':'C' };
  } else if (ss == "I") {
    pinMode((int)obj["pin"], INPUT);
    int ss = getIndex((int)obj["pin"]);
    record[ss] = { (int)obj["pin"], record[ss].value, 'I' };
  } else if (ss == "T") {

    pinMode((int)obj["pin"], OUTPUT);
    TimeSet(obj);

  } else if (ss == "R") {
    String s = (const char*)obj["T"];
    Serial.println(s);
    byte data_num = 0;
    byte time[6];
    while (s.indexOf(",") != -1) {
      time[data_num] = s.substring(0, s.indexOf(",")).toInt();
      Serial.println(time[data_num], DEC);
      data_num++;
      s = s.substring(s.indexOf(",") + 1);
    }
    //time[ data_num ] = s.toInt();
    if (data_num >= 5) {
      Wire.begin();
      Wire.beginTransmission(DS3231_I2C_ADDRESS);
      Wire.write(0);                  // set next input to start at the seconds register
      Wire.write(decToBcd(time[5]));  // set seconds
      Wire.write(decToBcd(time[5]));  // set minutes
      Wire.write(decToBcd(time[4]));  // set hours
      Wire.write(decToBcd(time[0]));  // set day of week (1=Sunday, 7=Saturday)
      Wire.write(decToBcd(time[1]));  // set date (1 to 31)
      Wire.write(decToBcd(time[2]));  // set month
      Wire.write(decToBcd(time[3]));  // set year (0 to 99)
      Wire.endTransmission();
    }
  } else if (ss == "X") {
    for (int i = 0; i < ii; i++) {
      if ((int)obj["pin"] == record[i].pin) {
        record[i] = { 20, 0, 'X' };
        break;
      }
    }


  } else if (ss == "Z") {
    // String s = (const char*)obj["host"];
    debug = (int)obj["debug"] ;
    publishInterval = (int)obj["tri"] ;
  }else if (ss == "L") {
     String msg = (const char*)obj["m"];
     print(msg,2);
  }
}
void Mqtt::TimeSet(JSONVar obj) {
  String s = (const char*)obj["o"];
  if (s.length() < 10) return;
  int ss = getIndex((int)obj["pin"]);
  record[ss] = { (int)obj["pin"], record[ss].value, 'T' };
  byte data_num = 0;
  while (s.indexOf(",") != -1) {
    record[ss].data[data_num] = s.substring(0, s.indexOf(",")).toInt();
    data_num++;
    s = s.substring(s.indexOf(",") + 1);
  }
  record[ss].data[data_num] = s.toInt();
}
int Mqtt::getIndex(int pin) {
  for (int i = 0; i < ii; i++) {
    if (pin == record[i].pin)
      return i;
  }
  return ii++;
}



byte Mqtt::bcdToDec(byte val) {
  return ((val / 16 * 10) + (val % 16));
}
byte Mqtt::decToBcd(byte val) {
  return ((val / 10 * 16) + (val % 10));
}
