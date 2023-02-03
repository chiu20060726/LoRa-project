#include "Arduino.h"
#include "LoRa_E22.h"
#include <WiFi.h>
#include "DHT.h"
#define DHTPIN 14     
#define DHTTYPE DHT22
#define R 27
#define G 26
#define B 25
int sos_count=0;
String last_msg="";
WiFiServer server;
DHT dht(DHTPIN, DHTTYPE);
IPAddress local_IP(192, 168, 1, 1);  
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
const char *ssid = "esp32-test(csc)";
const char *password = "00000000";

LoRa_E22 e22ttl(&Serial2, 18, 21, 19); //  RX AUX M0 M1

void white(){digitalWrite(R, HIGH);digitalWrite(G, HIGH);digitalWrite(B, HIGH);}

void red(){digitalWrite(R, HIGH);digitalWrite(G, LOW);digitalWrite(B, LOW);}
void green(){digitalWrite(R, LOW);digitalWrite(G, HIGH);digitalWrite(B, LOW);}
void blue(){digitalWrite(R, LOW);digitalWrite(G, LOW);digitalWrite(B, HIGH);}

void rg(){digitalWrite(R, HIGH);digitalWrite(G, HIGH);digitalWrite(B, LOW);}
void gb(){digitalWrite(R, LOW);digitalWrite(G, HIGH);digitalWrite(B, HIGH);}
void rb(){digitalWrite(R, HIGH);digitalWrite(G, LOW);digitalWrite(B, HIGH);}

void black(){digitalWrite(R, LOW);digitalWrite(G, LOW);digitalWrite(B, LOW);}

void setup() {
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  Serial.begin(9600);
  blue();
   WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(local_IP,gateway,subnet);
    WiFi.softAP(ssid, password);
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
  delay(500);

  // Startup all pins and UART
  e22ttl.begin();
  server.begin(1);
  Serial.println("Hi, I'm going to send message!");

  // Send message
  ResponseStatus rs = e22ttl.sendBroadcastFixedMessage(70, "Hello, world?");
  // Check If there is some problem of succesfully send
  Serial.println(rs.getResponseDescription());
  dht.begin();
   Serial.println("DHT22 start");
   sos_count=0;
   rg();
}


void loop() {
  
  WiFiClient client = server.available();
    if (client)
    {
        
        Serial.println("==connection built==");
        white();
        while (client.connected())
        {
          if(sos_count>=3){
            while(1){
              ResponseStatus rs = e22ttl.sendBroadcastFixedMessage(70, last_msg);
              Serial.println(rs.getResponseDescription());
              delay(1000);
              client.print("自動SOS已開啟，若要取消請重開板子(按EN或重新接電源線)");
            }
          }
          float h = dht.readHumidity();
          float t = dht.readTemperature();
           // If something available
  if (e22ttl.available()>1) {
    // read the String message
#ifdef ENABLE_RSSI
  ResponseContainer rc = e22ttl.receiveMessageRSSI();
#else
  ResponseContainer rc = e22ttl.receiveMessage();
#endif
  // Is something goes wrong print error
  if (rc.status.code!=1){
    Serial.println(rc.status.getResponseDescription());
  }else{
    // Print the data received
    //Serial.println(rc.status.getResponseDescription());
    String d=rc.data;
    Serial.println(d);
    if(d[3]=='#'){
      rb();
     client.print("server: "+d.substring(4,d.length()));
    }else if(d[3]=='!'){
      client.print("請幫助此SOS呼叫!!! 撥打112or911並提交接收到資料已進行救援! \n PLEASE HELP THIS SOS CALL!!! CALL 112or911AND SUBMIT RECEIVED DATA TO RESCUE! \n 或請聯繫最近可以提供救援服務的單位!!!(嘗試撥打112,911或衛星電話!");
      client.print(d.substring(4,d.length()));
    }
#ifdef ENABLE_RSSI
    Serial.print("RSSI: "); Serial.println(rc.rssi, DEC);
#endif
  }
  }
  if (Serial.available()) {
    rb();
    String input = Serial.readString();
    ResponseStatus rs = e22ttl.sendBroadcastFixedMessage(70, input);
    // Check If there is some problem of succesfully send
    Serial.println("server: "+input); 
    Serial.println(rs.getResponseDescription());
    client.print("server: "+input); 

  }
            
            if(client.available()) 
            {
              rb();
                String c = client.readString(); 
                
                if(c[0]=='?'){
                  client.print("溫度: "+ String(t) +"°C，濕度: "+ String(h) +"%"); 
                }else if(c[0]=='S'&&c[1]=='O'&&c[2]=='S'){
                  sos_count++;
                  ResponseStatus rs = e22ttl.sendBroadcastFixedMessage(70,"!"+ c);
                  Serial.println(rs.getResponseDescription());
                  client.print(c);
                  last_msg="!"+c;
                }else{
                  Serial.println(c); 
                  last_msg=c;
                  ResponseStatus rs = e22ttl.sendBroadcastFixedMessage(70,"#"+ c);
                  client.print(c);
                  Serial.println(rs.getResponseDescription());
                }
            }
            green();
        }
        client.stop(); 
        Serial.println("==disconnect==");
        red();
    }
 
}
