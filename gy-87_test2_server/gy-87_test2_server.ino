#include <ESP8266WiFi.h>
#define MAX_SRV_CLIENTS 2
const char* ssid = "I-GEOSCAN123";
const char* password = "zzzzzzzz";

WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];

//String send_data = "[{";
int this_device_id = 0;

void setup() {
  WiFi.mode(WIFI_AP);
  Serial.begin(9600);
  WiFi.softAP("iot_smart_ocean_master", "");
  IPAddress myIP = WiFi.softAPIP();
  server.begin();
  server.setNoDelay(true);
//  bmp180_setup();
  mpu6050_setup();
  bh1750_setup();
}

void loop() {

  /* telnet service */
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()) {
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()) {
        if (serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
                Serial.print("New client: "); Serial.print(i);
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  //check clients for data
  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      if (serverClients[i].available()) {
        //get data from the telnet client and push it to the UART
        char* receive_client;
        int count = 0;
        while (serverClients[i].available()) {
          char receive_data = serverClients[i].read();
//          receive_client[count++] = serverClients[i].read();
          Serial.write(receive_data);
//          for (int i = 0; i < MAX_SRV_CLIENTS; i++) {
//            if (serverClients[i] && serverClients[i].connected()) {
//              serverClients[i].write(receive_data);
//              delay(1);
//      
//            }
//          }
        }
        
        count = 0;
        
      }
    }
  }
  //check UART for data
  if (Serial.available()) {
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    //    Serial.println(sbuf[0]);

    //push UART data to all connected telnet clients
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      if (serverClients[i] && serverClients[i].connected()) {
        serverClients[i].write(sbuf, len);
        delay(1);

      }
    }
  }





  /*Serial communication */

  Serial.print("[{");
  Serial.print("'id':");
  Serial.print(this_device_id);
  
  char* start_char = "[{'id':";
  send_client_data(start_char);
  
  char charVal[10];               //temporarily holds data from vals 
  dtostrf(this_device_id, 4, 4, charVal);
  
  send_client_data(charVal);
  
  bh1750_loop();
  mpu6050_loop();
  Serial.println("}]");
  



  delay(1000);
}

void send_client_data(char* send_data){
//push UART data to all connected telnet clients
    for (int i = 0; i < MAX_SRV_CLIENTS; i++) {
      if (serverClients[i] && serverClients[i].connected()) {
        serverClients[i].write(send_data);
        delay(1);

      }
    }
}


