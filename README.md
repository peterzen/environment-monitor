# Environment monitor and logger

Arduino sketch that captures temperature, humidity and light intensity data and sends the values to [Thingsboard](https://thingsboard.io/) using MQTT.

Sensors:
  * 2x DHT21 (AM2301) temperature/humidity, indoor and outdoor.
  * BH1750 light sensor
  
It runs on a NodeMCU v3 (ESP8266 12E) board.

Measurements are taken once every minute.

### Dependencies:

 * ArduinoJSON
 * WiFiManager
 * ThingsBoard

