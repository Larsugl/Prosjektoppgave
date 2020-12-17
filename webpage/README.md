Datakomm prosjekt, webpage.

CONTENTS OF THIS FILE:

 * REQUIREMENTS
 * INSTALLATION
 * STARTUP VALUES
 * OPTIONS



# Requirements
-----------

This website requires modules:

Paho-MQTT
firebase.js
firebase-database.js
firebase-analytics.js

-------------------------------

# Installation
Only the Paho-MQTT library need to be downloaded, the other libraries are accesed by the website over the internet,
the links are added as an extra incase you want to add them locally instead.
Paho-MQTT:
https://cdnjs.cloudflare.com/ajax/libs/paho-mqtt/1.0.1/mqttws31.js

Firebase:
https://www.gstatic.com/firebasejs/8.1.1/firebase.js"
https://www.gstatic.com/firebasejs/8.1.1/firebase-database.js"
https://www.gstatic.com/firebasejs/8.1.1/firebase-analytics.js"

--------------------------------

Setup values:
These values can be changed to create your own website using the core of the code of this website.
The values in firebaseConfig in script.js.
the values in the client object.
the mqtt-topics that the website sends messages to and the topics that the website is subscribed to.

---------------------------------

Options:
You can change the RGB values in the variable "colours", to change the color of the box in the corner of the website,
and what light is sent to the ESP32.

--------------------------------

Made by Datakomm gruppe 02. 
Inquiries about the website can be sent to mabron@stud.ntnu.no



