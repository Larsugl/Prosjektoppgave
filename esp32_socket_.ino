
#include <analogWrite.h> //Import the analogWrite library for ESP32 so that analogWrite works properly

#include <WiFi.h>//Imports the needed WiFi libraries
#include <WiFiMulti.h> //We need a second one for the ESP32 (these are included when you have the ESP32 libraries)

#include <SocketIoClient.h> //Import the Socket.io library, this also imports all the websockets

WiFiMulti WiFiMulti; //Declare an instane of the WiFiMulti library
SocketIoClient webSocket; //Decalre an instance of the Socket.io library

  float Forceresistor = 35;
  float resistor = 3300;


void Event(const char * payload, size_t length) { //Default event, what happens when you connect
  Serial.printf("got message: %s\n", payload);
}

void forceApplied(const char * Force, size_t length) { //What happens when the ESP32 receives a instruction from the server (with variable) to change the LED
  Serial.printf("LED State: %s\n", Force); //First we print the data formated with the "printf" command
  Serial.println(Force); //Then we just print the LEDStateData which will be a int (0 og 1 so in reeality bool) that tells us what to do with the LED

  //Data conversion //We need som data processing to make this work
  String dataString(Force); //First we convert the const char array(*) to a string in Arduino (this makes thing easier)
  int ForceUsed = dataString.toInt(); //When we have a string we can use the built in Arduino function to convert to an integer
  
  force(ForceUsed);
}

void dataRequest(const char * dataRequestData, size_t length) {
  Serial.printf("Datarequest Data: %s\n", dataRequestData);
  Serial.println(dataRequestData);

  String dataString(dataRequestData);
  int RequestState = dataString.toInt();

  Serial.print("This is the Datarequest Data in INT: ");
  Serial.println(RequestState);

  if (RequestState == 0) { //If the datarequest gives the variable 0, do this (default)

    char str[10]; //Decalre a char array (needs to be char array to send to server)
    itoa(analogRead(35), str, 10); //Use a special formatting function to get the char array as we want to, here we put the analogRead value from port 27 into the str variable
    Serial.print("ITOA TEST: ");
    Serial.println(str);

    webSocket.emit("dataFromBoard", str); //Here the data is sendt to the server and then the server sends it to the webpage
    //Str indicates the data that is sendt every timeintervall, you can change this to "250" and se 250 be graphed on the webpage
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.setDebugOutput(true); //Set debug to true (during ESP32 booting)

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) { //More debugging
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFiMulti.addAP("Ida sin iPhone", "hannah00"); //Add a WiFi hotspot (addAP = add AccessPoint) (put your own network name and password in here)

  while (WiFiMulti.run() != WL_CONNECTED) { //Here we wait for a successfull WiFi connection untill we do anything else
    Serial.println("Not connected to wifi...");
    delay(100);
  }

  Serial.println("Connected to WiFi successfully!"); //When we have connected to a WiFi hotspot

  //Here we declare all the different events the ESP32 should react to if the server tells it to.
  //a socket.emit("identifier", data) with any of the identifieres as defined below will make the socket call the functions in the arguments below
  webSocket.on("clientConnected", Event); //For example, the socket.io server on node.js calls client.emit("clientConnected", ID, IP) Then this ESP32 will react with calling the event function
 // webSocket.on("LEDStateChange", changeLEDState);
  webSocket.on("Force", forceApplied);

  //Send data to server/webpage
  webSocket.on("dataRequest", dataRequest); //Listens for the command to send data

  webSocket.begin("46.249.252.58", 2520); //This starts the connection to the server with the ip-address/domainname and a port (unencrypted)
}


void force(bool forceApplied) {

  forceApplied = analogRead(35) * (3.3 / 1023.00);
  Serial.println(forceApplied);

  /*
  forceApplied = analogRead(35) * (3.3)/10223.00);
  While (forceApplied > 0){
  Serial.println(forceApplied);
  }  
   */
}


void loop() {
  webSocket.loop();
}
