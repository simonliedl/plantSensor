#include <ESP8266WiFi.h>
#include <AmazonIOTClient.h>
#include "Esp8266AWSImplementations.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

const char* ssid = "";
const char* key_id = "";
const char* secret_key = "";
const char* endpoint = "XYZ.iot.eu-west-1.amazonaws.com";

#define SLEEP_TIME_S 10
#define JSON_BUFFER_LEN (200)
#define JSON_OUTPUT_BUFFER_LEN (120)

#define ONE_WIRE_BUS D4  // DS18B20 pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS1820(&oneWire);


Esp8266HttpClient httpClient;
Esp8266DateTimeProvider dateTimeProvider;

AmazonIOTClient iotClient;
ActionError actionError;

void setup() {
  // put your setup code here, to run once:

  String clientMac = "";
  unsigned char mac[6];
  int val;
  char buf[JSON_OUTPUT_BUFFER_LEN];
  float temp;
  StaticJsonBuffer<JSON_BUFFER_LEN> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();


  /**** Connect Wifi */
  
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  /**** Read sensors and prepare data */

  val = analogRead(A0);
  Serial.println(val);

  do {
    DS1820.requestTemperatures(); 
    temp = DS1820.getTempCByIndex(0);
    Serial.print("Temperature: ");
    Serial.println(temp);
  } while (temp == 85.0 || temp == (-127.0));

  root.printTo(buf, JSON_OUTPUT_BUFFER_LEN);

  /*** Send sensor data */

  root["hello"] = 12;
  root["world"] = 12.3;
  iotClient.setAWSRegion("eu-west-1");
  iotClient.setAWSEndpoint("amazonaws.com");
  iotClient.setAWSDomain(endpoint);
  
  iotClient.setAWSPath("/topics/plantSensor");
  iotClient.setAWSKeyID(key_id);
  iotClient.setAWSSecretKey(secret_key);
  iotClient.setHttpClient(&httpClient);
  iotClient.setDateTimeProvider(&dateTimeProvider);

  char* result = iotClient.update_shadow(buf, actionError);
  Serial.print(result);

  /*** Go to sleep mode */

  ESP.deepSleep(SLEEP_TIME_S * 1000000);
  // WiFi.disconnect();
  
}

void loop() {
}
