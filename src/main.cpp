#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

// Wi-Fi connection parameters
const char * wifi_ssid = "Redmi Note 9 Pro";
const char * wifi_password = "14121875";
const char* mqttServer = "maqiatto.com";
const int mqttPort = 1883;
const char* mqttUser = "adrien.peyrouty@e-rekcah.com";
const char* mqttPassword = "ISIS_IoT_2020";

WiFiClient espClient;
PubSubClient client(espClient);

void connectToWiFi() {
  Serial.printf("Connecting to '%s'\n", wifi_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    Serial.print("Connected. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Connection Failed!");
  }
}

void callback (char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]");
  for (int i = 0; i<length;i++){
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setUpOverTheAirProgramming() {

  // Change OTA port. 
  // Default: 8266
  // ArduinoOTA.setPort(8266);

  // Change the name of how it is going to 
  // show up in Arduino IDE.
  // Default: esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // Re-programming passowrd. 
  // No password by default.
  // ArduinoOTA.setPassword("123");

  ArduinoOTA.begin();
}


void setup() {
  Serial.begin(115200);
  connectToWiFi();
  setUpOverTheAirProgramming();

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("adrien.peyrouty@e-rekcah.com/test1", "hello bonjouuuuuuur"); //Topic name
  client.subscribe("esp/test");
}


void loop() {
  // Give processing time for ArduinoOTA
  ArduinoOTA.handle();
  client.publish("adrien.peyrouty@e-rekcah.com/test1", "HORUS RECRUTE REJOIGNIEZ NOUS : ON SAMUSE BIEN"); 
  delay(8000);
}