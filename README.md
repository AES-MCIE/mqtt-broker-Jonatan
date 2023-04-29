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
![](./mosquittoStatus.png)

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

![](./mosquitto1.png)

Una vez llenados los datos, se le da a conectar y llevará a la pantalla que se muestra en la imagen, donde ahora se debe de presionar `New Subscription`, se le coloca un nombre, para este caso se le colocó `test`. En la parte inferior de la pantalla aparece un símbolo para enviar un mensaje, se pueden de enviar diferentes tipos de datos (se pueden visualizar en la parte de Payload) como JSON, entre otros; para este caso se usará `Plaintext` y debajo debe de asegurarse que esté colocado el nombre la subpcripción que se creó. Finalmente, se escribe el mensaje y se presiona el símbolo de la parte inferior derecha y se debería de recibir de vuelta el mismo mensaje, como se observa en la imagen, lo que indica que el servicio está funcionando y los dispositivos pueden conectarse con la BeagleBone a través de MQTT. 

![](./mosquitto2.png)

# Node-Red

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

Ahora es necesario poner en funcionamiento Node-Red en segundo plano y visualizar si se activó de forma correcta: 
```
node-red &
sudo netstat -plnt
```
En caso de que aparezca un mensaje diciendo que el puerto está en uso, como se muestra en la iamgen, se deberá presionar `ctrl+c`, el puerto por defecto es el 1880, pero se puede colocar otro puerto, por ejemplo, el 1885:
```
node-red -p 1885 &
```

![](./nodered1.png)

Si el puerto no está en uso deberá de aparecer algo parecido a lo que se muestra en la imagen, además se puede observar que se conecta directamente con el servidor MQTT que está corriendo en el puerto 1883. 

![](./nodered2.png)

Para ver el panel de Node-red, en el navegador de la computadora se debe acceder a `Nombre/IP_Host:1885`, en este caso se accedió con `192.168.1.9:1885`, lo cual nos lleva a la pantalla que se muestra en la imagen. 

![](./nodered3.png)