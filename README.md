# Introduction

The present task's main objective is to develop and test an *MQTT Broker* based on the MQTT (Mosquitto) protocol. 

Thus, the student must run the Mosquitto service in the BeagleBone to create the Broker. Then, an ESP32 client must publish the temperature data logged by a sensor. Furthermore, the Node-RED App should be used as a client to show the data as a Graphical User Interface from the ESP32 client. 

Finally, the student must implement a basic **Alarm** indicator that turns ON when the temperature exceeds a limit value or remains OFF if the value is lower.

![](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2018/05/ESP32-MQTT-bme280.png?w=800&quality=100&strip=all&ssl=1)

# Technical Report (Readme file)

The students must consider presenting a detailed document that includes:

- MQTT Broker installation and configuration 
- ESP32 client connection (schematics) and programming code with detailed explanation
- Node-RED installation, configuration, and views
