// DHT11 ESP32
// Jonatan Medina

// Librarys
#include <Adafruit_Sensor.h>
#include <DHT.h>

// dht pin
#define DHTPIN 34
//Sensor type
#define DHTTYPE DHT11

// Sensor configuration
DHT dht(DHTPIN, DHTTYPE);

void setup(){ 
    //serial communication
  Serial.begin(9600);
  //Start sensor
  dht.begin();
}

void loop(){
  //Reading Humidity
  float h = dht.readHumidity();
  //Reading Temperature °C
  float t = dht.readTemperature();
  //Reading Temperature °F
  float f = dht.readTemperature(true);
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("    Temperature: ");
  Serial.print(t);
  Serial.print(" °C");
  Serial.print("    Temperature: ");
  Serial.print(f);
  Serial.print(" °F \n");
  delay(5000);
}
