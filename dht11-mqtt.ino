// ESP32 + DHT11 + MQTT
// MQTT Client
// Jonatan Ali Medina Molina
// IT Morelia

//Librarys
#include <WiFi.h>
#include <PubSubClient.h>
#include <stdio.h>
#include <DHT.h>

//dht pin
#define DHTPIN 2
#define LED 4
//Sensor type
#define DHTTYPE DHT11

//Sensor configuration
DHT dht(DHTPIN, DHTTYPE);

// WiFi configuration
const char *ssid = "Megcable_2.4G_F156"; // WiFi name
const char *password = "F6D3kqGk";  // WiFi password
//const char *ssid = "Megcable_2.4G_FD48"; // WiFi name
//const char *password = "thTdXdR7";  // WiFi password
//const char *ssid = "TP-Link_835A"; // WiFi name
//const char *password = "79755689";  // WiFi password
int t = 0;
int h = 0;
char textT[20];
char textH[20];
char textS[20];
char letra[20];
char O[10]="o";
char N[10]="n";
char F[10]="f";

WiFiClient espClient;

// mqtt broker:
const char *mqttBroker = "192.168.1.200";
const char *topic1 = "esp32/temp";
const char *topic2 = "esp32/hum";
const char *topic3 = "esp32/out";
const char *mqttUsername = "emqx";
const char *mqttPassword = "";
const int mqttPort = 1883;
PubSubClient client(espClient);


void setup(){
  
  //Start sensor
  dht.begin();
  pinMode(LED, OUTPUT);
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
  client.setServer(mqttBroker, mqttPort);
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
  client.publish(topic1, "Hi broker I'm a ESP32 :)");
  client.subscribe(topic1);
  client.publish(topic2, "Hi broker I'm a ESP32 :)");
  client.subscribe(topic2);
  client.publish(topic3, "Hi broker I'm a ESP32 :)");
  client.subscribe(topic3);
}

void loop(){
  client.loop();
  //Temperature Measurement
  t = dht.readTemperature();
  sprintf(textT, "%d", t);
  //Publishing temperature measure
  client.publish(topic1, textT);

  //Humidity Measurement
  h = dht.readHumidity();
  sprintf(textH, "%d", h);
  //Publishing humidity measure
  client.publish(topic2, textH); 
  delay(2000);
}

//Printing Messages from MQTT Broker 
void callback(char *topic, byte *payload, unsigned int length){
  int contador = 0;
  Serial.print("Message recived in topic: ");
  Serial.println(topic);
  Serial.print("The message is: ");
  for(int i=0;i<length; i++){
    Serial.print((char) payload[i]);
    sprintf(letra, "%c", payload[i]);
    //Serial.print(letra);
    if(letra[0] == N[0]){
          contador+=1;
    }
    if(letra[0] == O[0]){
          contador+=1;
    }
    if(letra[0] == F[0]){
          contador+=1;
    } 
  }

  if(contador == 2){
      digitalWrite(LED, HIGH);
      Serial.println ("LED ON");
    }
  if(contador == 3){
    digitalWrite(LED, LOW);
    Serial.println("LED OFF");
    }
  contador = 0;
  Serial.println();
  Serial.println("-+-+-+End+-+-+-+");
}
