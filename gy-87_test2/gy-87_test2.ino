#include <ESP8266WiFi.h>
#define MAX_SRV_CLIENTS 2
const char* ssid = "I-GEOSCAN123";
const char* password = "zzzzzzzz";

WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];

String send_data = "[{";
int this_device_id = 0;

void setup() {
  WiFi.mode(WIFI_AP);
  Serial.begin(9600);
  WiFi.softAP("think_13", "zzzzzzzz");
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
        char a[10];
        int count = 0;
        while (serverClients[i].available()) {
          char receive_data = serverClients[i].read();
          Serial.write(receive_data);
        }

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


//  bmp180_loop();
  Serial.print("[{");
  Serial.print("'id':");
  Serial.print(this_device_id);
  bh1750_loop();
  mpu6050_loop();
  Serial.println("}]");
  
  
  Serial.print("[{");
  Serial.print("'id':");
  send_data.concat("'id':");
  Serial.print(this_device_id);
  send_data.concat(this_device_id);
  bh1750_loop();
  mpu6050_loop();
//  Serial.println("}]");
  



  char* b = "[{'id':";
//  send_data.toCharArray(b, 100);


    //    Serial.println(sbuf[0]);

    //push UART data to all connected telnet clients
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      if (serverClients[i] && serverClients[i].connected()) {
        serverClients[i].write(b);
        delay(1);

      }
    }


  delay(1000);
}
