# plantSensor
PlantSensor code for the TUM Junge Akademie Workshop

The device consists of an analog humidity sensor and a 1Wire Dallas DS1820 temperature sensor,
connected to an ESP8266.
The sensor device wakes up every 10 seconds and sends the measurements to AWS IoT.

### Required Libraries for Arduino IDE:
- [Arduino core for ESP8266 WiFi chip](https://github.com/esp8266/Arduino)
- DallasTemperature
- ArduinoJSON
- [AWS-SDK-Arduino](https://github.com/svdgraaf/aws-sdk-arduino)
