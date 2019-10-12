#include <ESP8266WiFi.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic


const char* host = "api.thingspeak.com";
const char* THINGSPEAK_API_KEY = "xxxxx";


void networkSetup(){
  WiFiManager wifiManager;
  wifiManager.autoConnect("IOT");
  Serial.println("");
  Serial.println("WiFi connection");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Use WiFiClient class to create TCP connections
WiFiClient client;

bool clientConnect(){
  
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.print("Unable to connect to host ");
      Serial.println(host);
      delay(1000); // don't hammer the remote host
      return false;
    }
    return true;
}


void sendData(float temperature_indoor, float humidity_indoor, float temperature_outdoor, float humidity_outdoor, uint16_t lux){
      // We now create a URI for the request
    String url = "/update?api_key=";
    url += THINGSPEAK_API_KEY;
    url += "&field1=";
    url += String(temperature_indoor);
    url += "&field2=";
    url += String(humidity_indoor);
    url += "&field3=";
    url += String(temperature_outdoor);
    url += "&field4=";
    url += String(humidity_outdoor);
    url += "&field5=";
    url += String(lux);
    
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
    delay(10);
    while(!client.available()){
      delay(100);
    }

    
    // Read all the lines of the reply from server and print them to Serial
//    while(client.available()){
//      String reply = client.readStringUntil('\r');
//    }

}
