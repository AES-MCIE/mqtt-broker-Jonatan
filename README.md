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

# Mosquito 

## Instalación y activación
En caso de que no se tenga instalado `Mosquito` en la BeagleBone Black, se deben realizar los siguientes pasos: 
- Actualizar los paquetes: 
```
sudo apt update
sudo apt upgrade
```
- Instalar Mosquitto: 
```
sudo apt install mosquitto
```

Una vez instalado, para iniciar el servicio se debe de ingresar el siguiente comando: 
```
sudo systemctl enable mosquitto.service
```

Se puede saber si el servicio se activó correctamente ingresando el siguiente comando, debe de aparecer la leyenda `Running`, como se muestra en la imagen. 
```
sudo systemctl status mosquitto service
```
![](/imagenes/mosquittoStatus.png)

## Configuración

Se pueden modificar ciertos apartados der servicio, como que se mantenga activado el servicio, el puerto en el que se corre, el archivo de contraseña, entre otras funciones.

Para cambiar la configuración que viene por defecto se debe de cambiar el archivo `mosquitto.conf`, lo cual se puede hacer descargando el archivo mosquitto.conf que viene en este repositorio y copiándolo a la ubicación correspondiente con el siguiente comando: 
```
sudo cp ./mosquitto.conf /etc/mosquitto/mosquitto.conf
```
Esto suponiendo que se esté en la carpeta donde se descargó el archivo. 

Otra manera es creando un archivo con el nombre `mosquitto.conf` con el siguiente contenido: 
```
persistence true
listener 1883
persistence_location /var/lib/mosquitto/
connection_messages true
allow_anonymous true
password_file /etc/mosquitto/passwd
```
Después de guardarlo, se debe copiar con el comando que se mostró anteriormente. 
 
Ahora se debe crear el archivo que contenga la contraseña, el cual se indicó en el archivo `mosquitto.conf`, si no se quiere colocar una contraseña, se puede seguir el siguiente comando: 
```
sudo touch /etc/mosquitto/passwd
```
Si se le quiere colocar una contraseña, se puede crear el archivo con el nombre `passwd` en donde se coloque la contraseña y luego copiarlo con el siguiente comando: 
```
sudo cp ./passwd /etc/mosquitto/passwd
```

Fimalmente, para aplicar los cambios se debe reinicar el servicio: 
```
sudo systemctl restart mosquitto
```

## Prueba del servicio

Para probar si el servicio se configuró de forma correcta, se puede descargar `MQTT X`, que es una aplicación disponible para Windows, Linux y MacOS que permite conectarse como cliente, se puede descargar de la página `https://mqttx.app` o con el siguiente comando en Linux: 
```
sudo snap install mqttx
```
Una vez instalado, en la interfaz de debe de presionar el símbolo `+` y se abrirá la pestaña que se muestra en la imagen, entonces se debe de llenar los apartados, en `Name` el nombre de la conexión, en la parte derecha de `Host` se debe colocar la IP de la BeagleBone Black, si no la sabe la puede ver en la terminal tecleando `ifconfig`, en la parte de `Port` se debe colocar el puerto en el que está corriendo el servicio, para nuestro caso es el 1883. ***Importante. Se debe de cambiar la versión de MQTT a la que se tenga instalada, para nuestro caso es la 3.1.1***

![](/imagenes/mosquitto1.png)

Una vez llenados los datos, se le da a conectar y llevará a la pantalla que se muestra en la imagen, donde ahora se debe de presionar `New Subscription`, se le coloca un nombre, para este caso se le colocó `test`. En la parte inferior de la pantalla aparece un símbolo para enviar un mensaje, se pueden de enviar diferentes tipos de datos (se pueden visualizar en la parte de Payload) como JSON, entre otros; para este caso se usará `Plaintext` y debajo debe de asegurarse que esté colocado el nombre la subpcripción que se creó. Finalmente, se escribe el mensaje y se presiona el símbolo de la parte inferior derecha y se debería de recibir de vuelta el mismo mensaje, como se observa en la imagen, lo que indica que el servicio está funcionando y los dispositivos pueden conectarse con la BeagleBone a través de MQTT. 

![](/imagenes/mosquitto2.png)

# Node-Red

## Instalación
Antes de instalar Node-Red, se requiere `nodejs` y `npm`, los cuales se instalan con los siguientes comandos: 
```
sudo apt update
sudo apt install nodejs
sudo apt update npm
```

Lo siguiente es la instalación de Node-Red a través de `npm` con un `--unsafe-perm` para instalar a pesar de que Node-Red indique que requiere una versión más reciente de nodejs: 
```
sudo npm install -g --unsafe-perm node-red
```
Luego se debe instalar el panel de Node-Red con los siguientes comandos: 
```
node-red-stop
npm install node-red-dashboard
```
## Activación 
Ahora es necesario poner en funcionamiento Node-Red en segundo plano y visualizar si se activó de forma correcta: 
```
node-red &
sudo netstat -plnt
```
En caso de que aparezca un mensaje diciendo que el puerto está en uso, como se muestra en la imagen, se deberá presionar `ctrl+c`, el puerto por defecto es el 1880, pero se puede colocar otro puerto, por ejemplo, el 1885:
```
node-red -p 1885 &
```
![](/imagenes/nodered1.png)

Si el puerto no está en uso deberá de aparecer algo parecido a lo que se muestra en la imagen, además se puede observar que se conecta directamente con el servidor MQTT que está corriendo en el puerto 1883. 

![](/imagenes/nodered2.png)

## Prueba de conexión de MQTT y Node-Red

Para ver el panel de Node-red, en el navegador de la computadora se debe acceder a `Nombre-IP_Host:1885`, en este caso se accedió con `192.168.1.200:1885`, lo cual nos lleva a la pantalla que se muestra en la imagen. 

![](/imagenes/nodered3.png)

Para comprobar que Node-Red y MQTT están conectados de forma correcta, se utilizaron los siguientes nodos que se pueden encontrar en la parte izquierda del panel de Node-Red:
- `mqtt in` que se encuentra en `network`. Permite recibir los mensajes que llegan a un tópico de MQTT.
- `debug` que se encuentra en `common`. Sirve para poder visualizar los mensajes que llegan o salen de un nodo en específico. 
- `chart` que se encuentra en `dashboard`. Permite graficar valores numéricos que le lleguen como mensaje. 

Como se acaban de colocar, aparecerá un triángulo rojo encima de los nodos, lo que indica que no están configurados, como se muestra en la imagen. 

![](/imagenes/nodered4.png)

Para configurarlos solamente es cuestión de dar doble click al nodo. La primer configuración será del nodo `mqtt in`, la configuración aparecerá como se muestra a continuación, la cual es muy parecida a la hecha en MQTTX, en server colocamos `mosquitto`, en topic colocamos `test` y en output colocamos `a String`, como se muestra en la imagen. 

![](/imagenes/nodered5.png)

Después es necesario dar click en la imagen del lapiz que está en la parte derecha de server, lo cual lleva a la configuración del servidor MQTT al que se conectará. En name colocamos `mosquitto`, en server colocamos la dirección IP de la BeagleBone, en este caso es `192.168.1.9`, en port colocamos el puerto en el que se esté corriendo mosquitto, en este caso es el `1883` y en protocol seleccionamos `MQTT V3.1.1`, en la imagen se muestran los campos llenados con lo descrito anteriormente. 

![](/imagenes/nodered6.png)

Se debe de presionar el botón `Update`, lo cual regresará a la ventana de la primera configuración y luego se debe de presionar `Done`, lo cual debe de quitar el triángulo rojo del nodo, en caso contrario, la configuración no se realizó de forma correcta. 

El siguiente nodo a configurar es `chart`, el cual es muy sencillo, ya que no debemos cambiar ninguna de las configuraciones, solamente debemos de dar click en el lapiz que está a la derecha de Group, en la ventana a la que lleva presionar `Update` y luego `Done`, con esto se debe de quitar el triángulo rojo del nodo. 

![](/imagenes/nodered7.png)

Luego de que todo esté configurado, la última parte es conectar los nodos, lo cual es muy sencillo, al acercar el ratón a los pequeños cuadrados que se encuentran a los lados de los nodos se iluminarán de color naranja, damos click y solamente es cuestión de arrastrar hasta el nodo al que se quiera conectar, para esta prueba, las conexiones quedaron como las que se muestran en la imagen. ***Importante: No olvidar presionar `Deploy` al terminar de realizar las conexiones.***

![](/imagenes/nodered8.png)

La prueba final para saber si todo funciona correctamente es entrar a la dirección `192.168.1.9:1885/ui` donde nos deberá aparecer lo que se muestra en la imagen. 

![](/imagenes/nodered9.png)

# DHT11

El DHT11 es un sensor digital de temperatura y humedad relativa de bajo costo, cuenta con las siguientes características: 
- Voltaje de Operación: 3V - 5V DC
- Rango de medición de temperatura: 0 a 50 °C
- Precisión de medición de temperatura: ±2.0 °C
- Resolución Temperatura: 0.1°C
- Rango de medición de humedad: 20% a 90% RH.
- Precisión de medición de humedad: 5% RH.
- Resolución Humedad: 1% RH
- Tiempo de sensado: 1 seg.

![](/imagenes/dht11.png)

Para más imformación se puede ingresar al siguiente link https://image.dfrobot.com/image/data/DFR0067/DFR0067_DS_10_en.pdf. 

# Arduino IDE
Para llevar a cabo la prgramación del ESP32 se usará Arduino IDE, el cual se puede descargar desde la tienda de aplicaciones de Windows, Ubuntu o MacOS, o desde la página oficial https://www.arduino.cc/en/software.

## Gestor de tarjetas

Para que Arduino IDE pueda compilar el programa y subir la información a la ESP32, primero es necesario descargar el gestor de placas de la ESP32, para eso se debe ir a `Herramientas -> Placa -> Gestor de tarjetas`, lo cual llevará a una ventana como la que se muestra en la imagen, donde solamente se debe buscar `ESP32` y el gestor a instalar es el creado por `Espressif Systems`.

![](/imagenes/tarjetas1.png)

En caso de que no aparezca el gestor mencionada, se deberá ingresar a `Archivo -> Preferencias`, lo cual llevará a una ventana como la que se muestra en la imagen, en el apartado de `Gestor de URLs Adicionales de Tarjetas` se deberá copiar el siguiente link https://dl.espressif.com/dl/package_esp32_index.json y oprimir Ok. Después es necesario volver a buscar en el gestor de tarjetas `ESP32`. 

![](/imagenes/tarjetas2.png)

## Librerías necesarias

El siguiente paso es instalar la librería `PubSubClient`, la cual servirá para la conexión con MQTT, para ello debemos de ir a `Programa -> Incluir Librería -> Administrar Bibliotecas`, en donde debemos de buscar PubSubClient e instalar la creada por `Nick O'Leary`.

![](/imagenes/libreria1.png)

Otra librería a instalar es la `DHT`, ya que es la que facilita la lectura del sensor que se va utilizar, para ello, estando en `Administrar Bibliotecas`, buscamos DHT e instalamos la que se muestra en la imagen. 

![](/imagenes/libreria2.png)

Y finalmente, aún estando en `Administrar Bibliotecas`, buscamos Adafruit Unified Sensor e instalamos la que se muestra en la imagen. 

![](/imagenes/libreria3.png)


# Esp32 Wifi + Bluetooth 4.2 Ble Nodemcu

El dispositivo que fungirá como cliente del servidor MQTT es el ESP32, cuya versión es la que se muestra en la imagen. 

![](/imagenes/esp32pinout.png)


## Prueba sensor DTH11

Las conexiones usadas para la prueba del sensor de temperatura y humedad se muestran en la siguiente imagen. 

![](/imagenes/esp32-proto.png)

Se hizo uso del código `dht11.ino` que se encuentra en este repositorio. Una vez con el código en Arduino IDE, se debe de conectar la ESP32 a la computadora y verificar los siguientes apartados: 
- Ir a `Herramientas -> Placa -> ESP32 Arduino` y ahí seleccionar `DOIT ESP32 DEVKITV1`. 
- Verificar que `Upload Speed` se encuentre a 921600. 
- En `Puerto`, se debe de seleccionar en el que está conectado el ESP-32, en caso de que no sepa cúal es, se puede conectar y desconectar para rápidamente identificar el puerto. 

Una vez hechas las configuraciones, solamente queda presionar el botón de subir, el cual es un símbolo de una flecha apuntando a la derecha `->`, con lo que se compilará el archivo y se empezará a subir. En dado caso de que aparezca un mensaje de conectando en la parte inferior de la pantalla, se deberá presionar el botón de `boot` de la ESP32 hasta que se comience a subir, para la placa usada en esta prueba no es necesario que se presione el botón. Una vez subido el programa se mostrará un mensaje como el que se muestra en la imagen. 

![](/imagenes/esp32-subir.png)

Para visualizar las mediciones se debe ir a `Herramienta -> Monitor Serie` y ahí asegurarse de que la velocidad que se encuentra en la parte inferior se encuentre en `9600`, si todo está correcto, deben aparecer mediciones como se muestra en la imagen. 

![](/imagenes/esp32Serie.png)


# Prueba de Node-Red, MQTT, ESP32 y DHT11

En el repositorio se encuentra el archivo `dht11-mqtt.ino`, en el cual se conecta al servidor MQTT que se encuentra en la BeagleBone y le envía el valor de temperatura y humedad leído por el sensor DHT11 cada 2 segundos, además, si recibe una orden de `on` u `off`, enciende o apaga un LED conectado al GPIO-4. El esquemático usado se muestra en la siguiente imagen.

![](/imagenes/final-proto.png)

En el código, las partes importantes a modificar son dos, la primera corresponde al nombre y contraseña de la red inalámbrica que se va a conectar la placa, lo cual se debe de colocar en `ssid` y `password`. La segunda parte a modificar son del servidor MQTT, en `mqttBroker` se debe colocar la dirección IP de la BeagleBone (192.168.1.200 en este caso), en `mqttUsername` se puede colocar cualquiera, como no se colocó contraseña`mqttPassword` se queda vacío y en `mqttPort` se coloca `1883`. En cuanto a los tópicos, se necesitan de tres para la prueba final, uno para temperatura, otro para humedad y el último para el control del LED, se le colocaron los nombres de `esp32/temp`, `esp32/hum` y `esp32/out`, respectivamente, pero se le pueden cambiar de nombre, solo es importante que en Node-Red, las entradas y salidas MQTT tengan el mismo tópico que los colocados. 

![](/imagenes/final-mqttArduino.png)

Pasando de nuevo a Node-Red, se requiere graficar los valores de humedad y temperatura, por lo que la primer parte es colocar los nodos que se encuentran en los siguientes apartados. 

- `mqtt in` que se encuentra en `network` X2.
- `debug` que se encuentra en `common` X2. 
- `chart` que se encuentra en `dashboard` X2. 

Los nodos quedaron conectados como se muestra en la siguiente imagen, las configuraciones son idénticas a las que se explicaron en el apartado `Prueba de conexión de MQTT y Node-Red`, solamente se debe de colocar en `Topic`: `esp32/temp` y `esp32/hum`. La configuración de los `charts` fue colocar `Temperature` y `Humidity` en el apartado de `Label`, para poder identificar la gráfica de las dos variables más fácilmente y el rango en el eje Y para que se acomode a los valores que puede arrojar el sensor durante las mediciones. 

![](/imagenes/final-charts.png)

La siguiente parte fue poder controlar el LED de acuerdo a la lectura de la temperatura recibida, para eso se hizo uso de los siguientes nodos: 

- `switch` que se encuentra en `function`. Permite redirigir los valores de acuerdo al mensaje de entrada que recibe y a las condicionales que se le indican. 
- `change` que se encuentra en `function` X2. Permite enviar un mensaje(se puede configurar el tipo de mensaje)cuando recibe algo a la entrada.
- `filter` que se encuentra en `function`. Permite bloquear que pase un mensaje de acuerdo a varias condicionales, por ejemplo, si no ocurre un cambio.
- `text` que se encuentra en `dashboard`. Permite visualizar un mensaje. 
- `mqtt out` que se encuentra en `network`. Permite publicar algo en un tópico de MQTT. 
- `debug` que se encuentra en `common`. 

La configuración de los nodos se muestra en la siguiente imagen. 

![](/imagenes/final-filtro.png)

La conexión de nodos resultante se muestra en la imagen siguiente, el funcionamiento es el siguiente, la temperatura que se recibe del nodo `esp32/temp` pasa al `switch`, el que manda el mensaje hacia la `On` si es mayor a 25 y hacia `Off` si es menor o igual a 25. Tanto `On` como `Off` solamente sirven para crear un mensaje (con 'on' u 'off') para `filter`, que hace la función de bloquear el paso de los mensajes hasta que haya un cambio, así se evita estar enviando repetidamente alguno de los dos mensajes. Finalmente, cuando alguno de los mensajes pasa, se publica en el tópico `esp32/out` y se coloca también en el nodo de `LED`, el cual mostrará si se encuentra encendido o apagado en pantalla.  ***Importante: No olvidar presionar `Deploy` para aplicar los cambios.*** 

![](/imagenes/final-nodosFiltro.png)

Y al ingresar desde el navegador a `192.168.1.200:1885/ui` se debe mostrar lo que se muestra en la siguiente imagen.

![](/imagenes/final-ui.png)

En la siguiente imagen se muestra la conexión en físico del circuito montado en la ESP32. 

![](/imagenes/prueba-fisico.jpg)

Y en la siguiente imagen, se muestra el monitor serie (debe estar colocado a velocidad de 115200 baudios) del Arduino IDE y las gráficas que se van realizando conforme llegan los datos. 

![](/imagenes/prueba-final.png)



