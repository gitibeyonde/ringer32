

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <WiFi.h>


WiFiUDP Udp;

String _deviceId;
IPAddress _localIp;

char bufa[512];
char bufb[256];

const uint8_t packe_size=128;
char packetIn[packe_size]; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 

  //get chip id
  _deviceId = String((uint32_t)ESP.getEfuseMac(), HEX);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin("lh6-2304", "tareilly");

  long start_wifi_millis = millis();
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
    if (millis() - start_wifi_millis > 20000) {
      //WiFi.disconnect(true);
      Serial.println("-F-");
    }
  }
  Serial.print(WiFi.localIP());
  _localIp = WiFi.localIP();
    Udp.flush();
    Udp.begin(6999);
}



void udpBroadcastHandler(){
    
    int packetSize = Udp.parsePacket();
  
    if (packetSize > 0 ){
      Serial.print(Udp.remoteIP());
      Serial.print(":");
      Serial.println(Udp.remotePort());
      
      // read the packet into packetBufffer
      int len = Udp.read(packetIn, packe_size);
      if (len > 0) packetIn[len] = 0;

      Serial.println(packetIn);
      
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.print("[IBEYONDE]:");
      Udp.print(_deviceId);
      Udp.print(":CAMERA,MOTON:1636529222.57");
      Udp.endPacket();
    }
    else {
      long rssi = WiFi.RSSI();
      Serial.print("RSSI:");
      Serial.println(rssi);
    }
      
}//udpBroadcastHandler


void loop() {
  // put your main code here, to run repeatedly:
  udpBroadcastHandler();
  delay(1000);
}
