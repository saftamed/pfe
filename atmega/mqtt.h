#ifndef tl
#define tl

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Arduino_JSON.h>
#define LINE_BUFFER_LENGTH 200
typedef struct
{
  byte pin;
  byte value;
  char action;
  byte data[6];
} input;

class Mqtt {

public:
  Mqtt(bool displayMsg);
  void begin(int baudRate = 4800);
  void connect( bool auth = false, String user = "", String pswd = "");
  void publish(String msg);
  void print(String msg, int l = 1);
  void beginDebug();
  void subscribe();
  void getData();
  bool available();
  String readString();
  void http();
  void setActions(String line);
  void setAll();
  void CheckData();
  bool isWifi = false;
  int publishInterval = 1500;


private:
  void initAt();
  void initTCP();
  void initHTTP();
  byte bcdToDec(byte val);
  byte decToBcd(byte val);
  bool debug = true;
  void rr();
  char line[LINE_BUFFER_LENGTH];
  int lineIndex = 0;
  void sendPing();
  void TimeSet(String obj,int pin);
  bool connected = false;
  int timeOut = 0;
  bool sendCmd(String cmd, int timeout);
  int getIndex(int pin);
  void sendData(input r);
};
#endif
