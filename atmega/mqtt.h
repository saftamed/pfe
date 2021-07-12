#ifndef tl
#define tl

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#define LINE_BUFFER_LENGTH 200
typedef struct
  {
      byte pin;
      byte value;
      char action;
      byte data[6];
  }  input;

class Mqtt {
  
public:
  Mqtt(bool displayMsg);
  void begin(int baudRate = 4800);
  void connect(/*String server, String server1, String port, String s,
               bool auth = false, String user = "", String pswd = ""*/);
  void publish(String msg);
  void subscribe();
  void getData();
  bool available();
  String readString();
      void http();
      void setActions(String line);
      void setAll();
      void CheckData();
      bool isWifi = false;


private:
  void initAt();
  void initTCP();
  void initHTTP();
  byte bcdToDec(byte val);
  byte decToBcd(byte val);
  void rr();
  char line[LINE_BUFFER_LENGTH];
  int lineIndex = 0;
  void sendPing();
  bool connected =false;
  int timeOut = 0;
  bool sendCmd(String cmd,int timeout);
  int getIndex(int pin);
  void sendData(input r);
  
 
};
#endif
