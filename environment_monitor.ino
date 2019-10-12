
#include "DHTesp.h"


// DHT Settings
#define DHTPIN 2     // what digital pin we're connected to. If you are not using NodeMCU change D6 to real pin
#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Update every 30 seconds = 0,5 minutes. Min with Thingspeak is ~20 seconds
const int UPDATE_INTERVAL_SECONDS = 60;

DHTesp dht_indoor;


// BH1750 light sensor
/*
  This is a simple example to test the BH1750 Light sensor
  
  Connect the sensor to a NodeMCU ESP8266:
  VCC  <-> 3V3 [grey]
  GND  <-> Gnd [purple]
  SDA  <-> D2  [green]
  SCL  <-> D1  [blue]
  ADDR <-> RX  [yellow]
 */

#include <Wire.h>
#include <BH1750FVI.h>

// Settings
uint8_t ADDRESSPIN = 13;
BH1750FVI::eDeviceAddress_t DEVICEADDRESS = BH1750FVI::k_DevAddress_H;
BH1750FVI::eDeviceMode_t DEVICEMODE = BH1750FVI::k_DevModeContHighRes;

// Create the Lightsensor instance
BH1750FVI LightSensor(ADDRESSPIN, DEVICEADDRESS, DEVICEMODE);



void LEDOn(){
    digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void LEDOff(){
  digitalWrite(2, LOW);   // turn the LED on (HIGH is the voltage level)  
}


void setup() {
  Serial.begin(115200);
  delay(10);

  networkSetup();

  dht_indoor.setup(2, DHTesp::DHT22); // Connect DHT sensor to GPIO 17

  LightSensor.begin();  

  // initialize LED
  pinMode(2, OUTPUT);
}

void loop() {      

    if(clientConnect() != true){
      return;
    }

    LEDOn();

    uint16_t lux = LightSensor.GetLightIntensity();
    
    float humidity_indoor = dht_indoor.getHumidity();
    float temperature_indoor = dht_indoor.getTemperature();

    if (isnan(humidity_indoor) || isnan(temperature_indoor)) {
      Serial.println("Can't receive data from DHT");
      delay(10);
      return;
    }

    sendData(temperature_indoor, humidity_indoor, lux);
    
    
    Serial.println();
    Serial.print("Temperature: ");
    Serial.print(temperature_indoor);
    Serial.print("C Humidity: "); 
    Serial.print(humidity_indoor);
    Serial.print("% "); 
    
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.print(" lux");


    LEDOff();

    delay(1000 * UPDATE_INTERVAL_SECONDS);
}
