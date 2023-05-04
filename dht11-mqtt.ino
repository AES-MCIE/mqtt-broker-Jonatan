//ESP32 + DHT11 + MQTT
//MQTT Client

//Librarys
#include <WiFi.h>
#include <PubSubClient.h>
#include <stdio.h>
#include <DHT.h>

//dht pin
#define DHTPIN 34
//Sensor type
#define DHTTYPE DHT11

//Sensor configuration
DHT dht(DHTPIN, DHTTYPE);

// WiFi configuration
const char *ssid = "Megcable_2.4G_F156"; // WiFi name
const char *password = "F6D3kqGk";  // WiFi password
float t = 0;
char text[20];
WiFiClient espClient;

// mqtt broker:
const char *mqttBrocker = "192.168.1.200";
const char *topic = "test";
const char *mqttUsername = "emqx";
const char *mqttPassword = "";
const int mqttPort = 1883;
PubSubClient client(espClient);


void setup(){
  
  //Start sensor
  dht.begin();
  
  //serial communication
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("Connecting to ");
    Serial.println(ssid); 
  }
  Serial.println("Connection done.");
  
  //connecting to a mqtt brocker:
  client.setServer(mqttBrocker, mqttPort);
  client.setCallback(callback);
  while(!client.connected()){
    String clientID = "esp32-jonatan-";
    clientID += String(WiFi.macAddress());
    Serial.printf("The %s tries to connect to mqtt borcker...\n",clientID.c_str());
    if(client.connect(clientID.c_str(), mqttUsername, mqttPassword)){
      Serial.println("mqtt brocker connected");
    }
    else {
      Serial.print("mqtt connection failed");
      Serial.println(client.state());
      delay(2000);
    }
  }
  //once connected publish and suscribe:
  client.publish(topic, "Hi broker I'm a ESP32 :)");
  client.subscribe(topic);
}

void loop(){
  client.loop();
  //Reading Sensor
  t = dht.readTemperature();
  sprintf(text, "%d", t);
  
  //Publishing temperature measure
  client.publish(topic, text);
  delay(5000);
}

//Printing Messages from MQTT Broker 
void callback(char *topic, byte *payload, unsigned int length){
  Serial.print("Message recived in topic: ");
  Serial.println(topic);
  Serial.print("The message is: ");
  for(int i=0;i<length; i++){
    Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-+-+-+End+-+-+-+");
}
