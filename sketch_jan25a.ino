#include <WiFi.h>

const char *ssid = "esp32";
const char *password = "00000000";

WiFiServer server;

IPAddress local_IP(192, 168, 1, 1);  
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
void setup()
{
    Serial.begin(115200);
    Serial.println();

    //WiFi.mode(WIFI_STA);
    WiFi.mode(WIFI_AP);
    //WiFi.setSleep(false);
    //WiFi.begin(ssid, password);
    WiFi.softAPConfig(local_IP,gateway,subnet);
    WiFi.softAP(ssid, password);
    /*
    while (WiFi.status() != WL_CONNECTED)  
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected");
    Serial.print("IP Address:");
    Serial.println(WiFi.localIP());
    */
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
    server.begin(1); 
}

void loop()
{
    WiFiClient client = server.available();
    if (client)
    {
        Serial.println("==connection built==");
        while (client.connected())
        {
            if(Serial.available()){
            String a=Serial.readString();
              client.print("server: " + a); 
              Serial.println("server: " + a);
            }
            if(client.available()) 
            {
                String c = client.readString(); 
                client.print("user: " + c); 
                Serial.println("user: " + c); 
            }
        }
        client.stop(); 
        Serial.println("==disconnect==");
    }
}
