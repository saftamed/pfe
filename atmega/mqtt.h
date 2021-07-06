#ifndef tl
#define tl

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#define LINE_BUFFER_LENGTH 256
typedef struct
  {
      int pin;
      int value;
      String action;
  }  input;

class Mqtt {
  
public:
  Mqtt(bool displayMsg);
  void begin(int baudRate = 4800);
  void connect(String server, String server1, String port, String s,
               bool auth = false, String user = "", String pswd = "");
  void publish(String topic, String msg);
  void subscribe(String topic);
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

  void rr();
  char line[LINE_BUFFER_LENGTH];
  int lineIndex = 0;
  String serverr;
  String userr ="";
  String pwdd ="";
  String portt ="1883";
  String clientId ="";
  bool withauth = false;
  String topicc="";
  void sendPing();
  bool connected =false;
  int timeOut = 0;
  bool sendCmd(String cmd,int timeout);
  int getIndex(int pin);
  void sendData(input r);
  
 
};
#endif
