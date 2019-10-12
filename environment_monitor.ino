
#include "DHTesp.h"


// DHT Settings
#define DHT_OUTDOOR_PIN 0     // outdoor DHT sensor pin
#define DHT_INDOOR_PIN 2      // indoor DHT sensor pin
#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Update every 30 seconds = 0,5 minutes. Min with Thingspeak is ~20 seconds
const int UPDATE_INTERVAL_SECONDS = 60;

DHTesp dht_indoor, dht_outdoor;


// BH1750 light sensor
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

  dht_outdoor.setup(DHT_OUTDOOR_PIN, DHTesp::DHT22);
  dht_indoor.setup(DHT_INDOOR_PIN, DHTesp::DHT22);

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
      Serial.println("Can't receive data from indoor DHT");
      delay(10);
      return;
    }

    float humidity_outdoor = dht_outdoor.getHumidity();
    float temperature_outdoor = dht_outdoor.getTemperature();

    if (isnan(humidity_outdoor) || isnan(temperature_outdoor)) {
      Serial.println("Can't receive data from outdoor DHT");
      delay(10);
      return;
    }

    sendData(temperature_indoor, humidity_indoor, temperature_outdoor, humidity_outdoor, lux);
    
    Serial.println();
    Serial.print("Indoor temp: ");
    Serial.print(temperature_indoor);
    Serial.print("C Indoor humidity: "); 
    Serial.print(humidity_indoor);
    Serial.print("% "); 
    
    Serial.print("Outdoor temp: ");
    Serial.print(temperature_outdoor);
    Serial.print("C Outdoor humidity: "); 
    Serial.print(humidity_outdoor);
    Serial.print("% "); 

    Serial.print("Light: ");
    Serial.print(lux);
    Serial.print(" lux");

    LEDOff();

    delay(1000 * UPDATE_INTERVAL_SECONDS);
}
