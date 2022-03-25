#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <string.h>
//capteurs
int moistureSensorPin = A0; // pour le capteur de l'humidité du sol
int moistureVal; //analog readings

// Wi-Fi connection parameters
const char * wifi_ssid = "Redmi Note 9 Pro";
const char * wifi_password = "14121875";
const char* mqttServer = "maqiatto.com";
const int mqttPort = 1883;
const char* mqttUser = "mariecgs@protonmail.com";
const char* mqttPassword = "k945PYT&V#U@t";

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

  client.publish("mariecgs@protonmail.com/gislain", "hello gislain"); //Topic name
}


void loop() {
  
    moistureVal = analogRead(moistureSensorPin);//lire la valeur retournée par le capteur de l'humidité
    Serial.println("capteur : ");
    Serial.println(moistureVal);
   // Serial.println(snprintf(msg,10,moistureVal));

    int valueToSend = 5;
    char valueAsString[32] = {0};
    snprintf(valueAsString, sizeof(valueAsString), "%d", moistureVal);
    Serial.println(moistureVal);

    ArduinoOTA.handle();
    client.publish("mariecgs@protonmail.com/gislain", "moistureVal"); 
    client.publish("mariecgs@protonmail.com/gislain", valueAsString); 

    delay(8000);
}