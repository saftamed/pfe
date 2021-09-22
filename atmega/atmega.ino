#include "mqtt.h"


Mqtt mqtt(false);


//int publishInterval = 1500;
long lastPublishMillis = 0;

void setup() {
  Serial.begin(9600);



  while (!getModule(3))
    ;

  if (!mqtt.isWifi) {
    mqtt.begin(9600);
    mqtt.http();
    mqtt.connect();
    delay(6000);
    mqtt.subscribe();
    // delay(4000);
    // mqtt.publish("iot-2/4561", F("{\"action\":\"D\",\"pin\":13,\"value\":1,\"options\":50}"));


  } else {

    //  String s = Serial.readString();
    //  Serial.println(s);
    mqtt.setAll();
  }
}

void loop() {
  // displayTime();
  //  setDS3231time(0,15,12,3,14,11,16);
  if (mqtt.available()) {
    String line = mqtt.readString();
    if (line == "ERROR") {
      //Serial.println(line);
    } else {
      // Serial.println(line);
      mqtt.setActions(line);
    }
  }
  if (millis() - lastPublishMillis > mqtt.publishInterval) {
    mqtt.CheckData();
    lastPublishMillis = millis();
  }
}


bool getModule(int timeout) {
  Serial.println("at");
  int timeOut = 0;
  while (Serial.available() <= 0) {
    timeOut++;
    if (timeOut >= timeout) {
      return false;
    }
    delay(1000);
  }
  String s = Serial.readStringUntil('\n');
  if (s.indexOf("OK") >= 0) {
    mqtt.isWifi = false;
    mqtt.print(F("Sim Detected "));
    return true;
  } else if (s.indexOf("wifi") >= 0) {
    mqtt.isWifi = true;
    mqtt.print(F("Wifi Detected"));
    return true;
  }
  return false;
}

















































/*


byte decToBcd(byte val)
{
  return ( (val / 10 * 16) + (val % 10) );
}

// Convert binary coded decimal to decimal numbers
byte bcdToDec(byte val)
{
  return ( (val / 16 * 10) + (val % 16) );
}

// Set the time
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();

  // request seven bytes of data from DS3231 starting from register 00h
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}


void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
                 &year);
                 
  // send it to the serial monitor
  
}



*/
