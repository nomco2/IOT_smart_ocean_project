#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti WiFiMulti;

#define MAX_SRV_CLIENTS 2
const char* ssid = "I-GEOSCAN123";
const char* password = "zzzzzzzz";

WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];

//String send_data = "[{";
int this_device_id = 1;

WiFiClient client;

void setup() {
  WiFi.mode(WIFI_STA);
  Serial.begin(9600);
//  WiFi.softAP("iot_smart_ocean_master", "");
//  IPAddress myIP = WiFi.softAPIP();
//  server.begin();
//  server.setNoDelay(true);
//  bmp180_setup();
  WiFiMulti.addAP("iot_smart_ocean_master", "");
  while (WiFiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
    }
  delay(500);

  const uint16_t port = 23;
  const char * host = "192.168.4.1"; // ip or dns


  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  

  while (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(5000);
    return;
  }
  
  mpu6050_setup();
  bh1750_setup();
}

void loop() {

  //read back one line from server
//  if(client.available()){
////   String line = client.readStringUntil('/');
//   char a = client.read();
//   Serial.println(a);
//   
//  }
  if(Serial.available()){
      char a = Serial.read();
      client.println(a);
  }




  /*Serial communication */

  Serial.print("[{");
  Serial.print("'id':");
  Serial.print(this_device_id);
  
  client.print("[{");
  client.print("'id':");
  client.print(this_device_id);
  
//  char* start_char = "[{'id':";
//  send_client_data(start_char);
//  
//  char charVal[10];               //temporarily holds data from vals 
//  dtostrf(this_device_id, 4, 4, charVal);
//  
//  send_client_data(charVal);
  
  bh1750_loop();
  mpu6050_loop();
//  Serial.println("}]");
  



  delay(1000);
}



