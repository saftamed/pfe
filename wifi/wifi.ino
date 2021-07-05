#include <ESP8266WiFi.h>
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>


//-------- Customise these values -----------
const char* token  = "4561"; 
// const char* password = "u9pHINUEal";
 String ssid = "TOPNET_19E8";
 String password = "u9pHINUEal";
String topic = "iot-2/"+String(token);
//-------- Customise the above values --------

IPAddress local_IP(192, 168, 1, 184);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8); // optional
IPAddress secondaryDNS(8, 8, 4, 4);

String httpServeur = "http://192.168.1.15/espitems/"+String(token);
char serverr[] = "192.168.1.15";
char clientId[] = "safta001";




bool pwd = true;

WiFiClient wifiClient;

void callback(char* topic, byte* payload, unsigned int payloadLength) {
  String myObject = (char *)payload;
  Serial.println(myObject)  ;
         
}
PubSubClient client(serverr, 1883, callback, wifiClient);
ESP8266WebServer server(80);


void handleRoot() {
  String response ="";//F("<style>.form input[type=password],.form select{width:100%;padding:12px 20px;margin:8px 0;display:inline-block;border:1px solid #ccc;border-radius:4px;box-sizing:border-box}.form input[type=submit]{width:100%;background-color:#4caf50;color:white;padding:14px 20px;margin:8px 0;border:0;border-radius:4px;cursor:pointer}.form input[type=submit]:hover{background-color:#45a049}.form{border-radius:5px;background-color:#f2f2f2;padding:20px}</style><form class='form' action=\"update\"><select name=\"ssid\">");
  byte numSsid = WiFi.scanNetworks();

  for (int thisNet = 0; thisNet<numSsid; thisNet++) {
    response +="<option>"+WiFi.SSID(thisNet) + "</option>";
  } 
   response +="</select><input type='password' name='password'><input type='submit' value='send'></form>";
  server.send(200, "text/html", response);

}
void handleUpdate(){

  ssid = server.arg("ssid");
  password =server.arg("password");
  server.send(200, "text/html", "ok");
   //   Serial.println(ssid);
   // Serial.println(password);
   delay(3000);
  pwd = false;  


}
void serveForPwd(){
    WiFi.mode(WIFI_AP_STA);
  //Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");    
  //Serial.print("Configuring access point...");
  WiFi.softAP("safta", "ap_password");
  IPAddress myIP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(myIP);
  server.on("/", handleRoot);  
  server.on("/update", handleUpdate);  
  server.begin();
  //Serial.println("HTTP server started");
}

void setup() {

  Serial.begin(9600);
while(Serial.available()<=0){

}
  String s= Serial.readStringUntil('\n');
  if(s.indexOf("at")>=0){
    Serial.println("wifi");
  }  


  // serveForPwd();
  // while(pwd){
  // server.handleClient();
  // }
  // WiFi.softAPdisconnect (true);


  wifiConnect();
  delay(2000);
  httpGet();
  mqttConnect();
}

void loop() {


  if (!client.loop()) {
    mqttConnect();

  }
  if(Serial.available()){
      String s= Serial.readStringUntil('\n');
      Serial.println(s);
      client.publish(topic.c_str(),s.c_str());
    //Serial.println("OK");
   
  
  }
  // server.handleClient();
}

void wifiConnect() {
    if (!WiFi.config(local_IP, gateway, subnet,primaryDNS,secondaryDNS)) {
  //  Serial.println("STA Failed to configure");
  }
  //Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.print("nWiFi connected, IP address: "); Serial.println(WiFi.localIP());

}

void mqttConnect() {
  if (!!!client.connected()) {
    //Serial.print("Reconnecting MQTT client to "); Serial.println(serverr);
    while (!!!client.connect(clientId)) {
      //Serial.print(".");
        server.handleClient();
      delay(500);
    }
    if (client.subscribe(topic.c_str())) {
      //Serial.println("subscribe to responses OK");
    } else {
      //Serial.println("subscribe to responses FAILED");
    }
    //Serial.println();
  }
}

void httpGet(){
      if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      http.begin(httpServeur);
      int httpResponseCode = http.GET();
      if (httpResponseCode>0) {
      
        
        String payload = http.getString();
        Serial.println(payload);

        
     
  
    
    }
      else {
      Serial.println("F");
      }
      // Free resources
      http.end();
    }
    else {
     
    }
}