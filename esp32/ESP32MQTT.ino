#include <PubSubClient.h>
#include <analogWrite.h>
#include <WiFi.h>

// Wifi
const char* ssid = "<router-name>";                                                             // Your personal network SSID
const char* wifi_password = "<router-password>";                                                // Your personal network password

// MQTT
const char* mqtt_server = "datakommg02.duckdns.org";                                            // IP/adress of the MQTT broker
#define mqtt_port 8883 //port of the MQTT broker
#define force_topic "mqtt/ESP32/forcemsg"                                                       // topic/library for the sensor
#define morse_topic "mqtt/ESP32/morsemsg"                                                       // topic/library for morse code
#define mqtt_username "gruppe2"                                                                 // MQTT username
#define mqtt_password "gruppe2"                                                                 // MQTT password

// Morse
unsigned long signal_len,t1,t2;                                                                 // time for which button is pressed
String code = "";                                                                               // string in which one alphabet is stored
String msg = " ";                                                                               // string in which message is stored

// RGB
#define LedR 21                                                                                 // red connector for the Led
#define LedG 22                                                                                 // green connector for the Led
#define LedB 23                                                                                 // blue connector for the Led

// Force
const int interval = 10;                                                                        // interval for amount of force-messages being sent


// WiFi Client object
WiFiClient wifiClient;                                                                          // defining the wifi connection for the client object

// MQTT Client object
PubSubClient client(wifiClient);                                                                // adding the client object for MQTT 


// Function to setup WiFi connection
void setup_wifi()
{
  Serial.print("Connecting to");
  Serial.print(ssid);                                                                           // Printing out the IP that we connect to

  WiFi.begin(ssid, wifi_password);                                                              // Initiating WiFi connection

  // Check WiFi connection and try to reconnect
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);                                                                                 // delay of 5ms to avoid overload
    Serial.print(".");                                                                          // Simulate a loading-time for reconnection
  }

  // Print out the resulting WiFi connection
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


// Function to reconnect if unable to connect to MQTT Broker
void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    String clientID = "ESP32Client-";                                                           // Creating a Client ID to separate and identify it for clients and server
    clientID += String(random(0xffff), HEX);                                                    // adding a random id for the ESP in case of several ESP-connections
    // Attempt to connect
    if (client.connect(clientID.c_str(),mqtt_username,mqtt_password)) 
    {
      Serial.println("connected");
      client.subscribe("mqtt/#");                                                               // Resubscribe to topic/library
    } 
    // If connection fails, check error and trying to reconnect again
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again");
      delay(500);                                                                               // Wait 5 milliseconds before retrying
    }
  }
}

// Get message from library that we are subscribed to
void callback(char* topic, byte *payload, unsigned int length) 
{
    Serial.println("-------new message from broker-----");
    Serial.print("channel:");
    Serial.println(topic);                                                                      // Check what topic the message is coming from 
    Serial.print("data:");  
    //Serial.write(payload, length);                                                            // Optional way of checking the message
    //Serial.println();
    payload[length] ='\0';                                                                      // Do not include empty spaces of message
    String info = String((char*)payload);                                                       // Convert message to string
    Serial.println(info);                                                                       // Print out message
    Serial.println();
    String topicStr = topic;                                                                    // Make the topic a string
    if (topicStr == "mqtt/webklient/morse")                                                     // Check topic for a specific branch
    {
      morse();                                                                                  // Start up morse function
      msg = " ";                                                                                // Give the string 'msg' an empty space so the function will run when called again
      delay(1000);                                                                              // Give ESP32 time to send packets before checking for new messages
    }
    else if (topicStr == "mqtt/webklient/force")                                                // Check topic for specific branch
    {
      force();                                                                                  // Start up force function
      delay(1000);                                                                              // Give ESP32 time to send packets before checking for new messages
    }
    else if (topicStr == "mqtt/webklient/rgb")                                                  // Check topic for specific branch
    {
      String red = getValue(info, ',', 0);                                                      // Check content of message and  function getValue to get value for red
      String green = getValue(info, ',', 1);                                                    // Check content of message and  function getValue to get value for green
      String blue = getValue(info, ',', 2);                                                     // Check content of message and  function getValue to get value for blue 
      int redVal = red.toInt();                                                                 // Change the value of red to an integer
      int greenVal = green.toInt();                                                             // Change the value of green to an integer
      int blueVal = blue.toInt();                                                               // Change the value of blue to an integer
      Serial.println("Red: " + String(redVal));
      Serial.println("Green: " + String(greenVal));
      Serial.println("Blue: " + String(blueVal));
      rgbled(redVal, greenVal, blueVal);                                                        // Start up rgbled function with the values of the colors as arguments
    }
    else if (topicStr == "mqtt/webklient/morsemsg")                                             // Check topic for specific branch
    {
      morseMsg(info);                                                                           // Start up morseMsg function with message as argument
      delay(1000);                                                                              // Give ESP32 time to send packets before checking for new messages
    }
}

void morseMsg(String msg)                                                                       // morseMsg function, with the recieved message as input
{
  int j = msg.length();                                                                         // Check the length of the message

  for (int i = 0; i < j; i++)                                                                   // Go through each character in the message
  {
    if (msg[i] == '-')                                                                          // Checks if the character is a '-'
    {
      analogWrite(LedR, 255);                                                                   // LED lights up
      delay(750);                                                                               // How long it will light up in milliseconds
    }
    else if (msg[i] == '.')                                                                     // Checks if the character is a '.'
    {
      analogWrite(LedR, 255);                                                                   // LED lights up
      delay(250);                                                                               // How long it will light up in milliseconds
    }
    else if (msg[i] == ' ')                                                                     // Checks if the character is an empty space
    {
      delay(1250);                                                                              // How long it will wait
    }
   analogWrite(LedR, 0);                                                                        // Make sure the LED turns off
   delay(250);                                                                                  // Give ESP32 time to process before going to next character
  }
  reconnect();                                                                                  // To make sure the ESP32 can go back to recieving messages.
                                                                                                // The server closes connection if there's no interaction from client within 15 seconds, therefore a reconnect is to make sure next message is recieved
  loop();                                                                                       // Starts up loop to get new messages                                                                                
}



String getValue(String data, char separator, int index)                                         // Function to get the value of the colors                           
{
    int found = 0;                                                                              // Defining a variable for found integers
    int strIndex[] = { 0, -1 };                                                                 // Variable for the stringIndex
    int maxIndex = data.length() - 1;                                                           // Variable for the max Index

    for (int i = 0; i <= maxIndex && found <= index; i++) {                                     // Go through each value
        if (data.charAt(i) == separator || i == maxIndex) {                                     // Check if there's a comma, then go through everything before the comma
            found++;                                                                            // Add the value to the variable found
            strIndex[0] = strIndex[1] + 1;                                                      // increase the index
            strIndex[1] = (i == maxIndex) ? i+1 : i;                                            // Check if string index is the same as max index
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";                       // Return the values
}



void rgbled(int r,int g,int b)                                                                  // Function rgbled with the color-values as arguments
{
  
  if ((r > 0 && r <= 255) or (g > 0 && g <= 255) or(b > 0 && b <= 255))                         // Check if the values are higher than 0 and less than 255
  {
    if (r > 0 && r <= 255)                                                                      // If red values are within the limits
    {
      Serial.println(String(r));                                                                
      analogWrite(LedR, r);                                                                     // Turn on the LED with the colorcodes for red
    }
    if (g > 0 && g <= 255)                                                                      // If green values are within the limits
    {
      Serial.println(String(g));                                                                
      analogWrite(LedG, g);                                                                     // Turn on the LED with the colorcodes for green
    }
    if (b > 0 && b <= 255)                                                                      // If blue values are within the limits
    {
      Serial.println(String(b));
      analogWrite(LedB, b);                                                                     // Turn on the LED with the colorcodes for blue 
    }
  }
  delay(1000);                                                                                  // How long the LED will light up
  // Turn all the colorcodes back to 0 to reset for next
  analogWrite(LedR, 0);
  analogWrite(LedG, 0);
  analogWrite(LedB, 0);
}


void morse()                                                                                  // Function morse
{
  while (msg != "")
  {
  NextDotDash:
    while (analogRead(35) == 0) {}
    t1 = millis();                                                                            //time at button press
    while (analogRead(35) >= 10) {}
    t2 = millis();                                                                            //time at button release
    signal_len = t2 - t1;                                                                     //time for which button is pressed
    if (signal_len > 50)                                                                      //to account for switch debouncing
    {
      code += readio();                                                                       //function to read dot or dash
    }
    while ((millis() - t2) < 1000)                                                            //if time between button press greater than 0.5sec, skip loop and go to next alphabet
    {     
      if (analogRead(35) >= 100)                                                              // if button pressed
      {
        goto NextDotDash;                                                                     // Start function over(go to next character)
      }
    }
    convertor();                                                                              //function to decipher code into alphabet
  }
}

String readio()
{
  if (signal_len < 500 && signal_len > 50)
  {
    return ".";                                                                               //if button press less than 0.6sec, it is a dot
  }
  else if (signal_len > 500 && signal_len < 1000)
  {
    return "-";                                                                               //if button press more than 0.6sec, it is a dash
  }
}

void convertor()
{
  static String letters[] = {".-", "-...", "-.-.", "-..", ".",                                //Alphabet of dot/dash combinations - from A-Z
                              "..-.", "--.", "....", "..", ".---", 
                              "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
                             ".-.", "...", "-", "..-", "...-", 
                             ".--", "-..-", "-.--", "--..", "Å"
                            };
  int i = 0;
  if (code == ".-.-.-")                                                                       // Check if the combination is equal to the morsecode of '.'
  {
    msg += ".";                                                                               //add end of message
    Serial.println(msg);
    delay(1000);
    if (client.publish(morse_topic, msg.c_str())) {                                           // Publish the message
      reconnect();                                                                            // Retry the connection to make sure the next message is recieved
      Serial.println("Morse sent!");
    } 

    // If data is not able to be sent to the server, try to reconnect to broker and send again
    else {
      Serial.println("Morse failed to send. Reconnecting to MQTT broker and trying again");
      reconnect();
      client.publish(morse_topic, msg.c_str());
    }
    msg = "";                                                                                 //reset msg to blank string
  }
  else if (code == "......")                                                                  // Checks if message is 6 dots, then it's the same as a space
  {
    msg += " ";
    Serial.println(msg);
  }
  else if (signal_len > 1000)                                                                 // Checks if the force-sensor is pressed longer than a sec
  {
    int lastChar = msg.length() - 1;
    msg.remove(lastChar);                                                                     // Remove the last character in the message
    Serial.println("Removed last character");
    Serial.println(msg);
  }
  
  else
  {
    while (letters[i] != "Å")                                                                 // loop for comparing input code with letters array
    {
      if (letters[i] == code)
      {
        Serial.println(char('A' + i));
        msg += (char('A' + i));                                                               // add character to msg
        Serial.println(msg);
        break;                                                                                // break out of loop
      }
      i++;
    }
    if (letters[i] == "Å")
    {
      Serial.println("<Wrong input>");                                                        // if input code doesn't match any letter, error
    }
  }
  code = "";                                                                                  // reset code to blank string
}

void force()                                                                                  // Function force
{
  int start = 0;
  for (start; start < interval; start++)                                                      // Check the timer
  {
                                                                                              // Gets the force data from the sensor
    float forceApplied = analogRead(35); 
    float force = forceApplied * (3.3/1023.00);
  
                                                                                              // Checks if there's any data recorded
    if (isnan(forceApplied) || isnan(force)) {
      Serial.println("Failed to read force");
    } 
                                                                                              // Starts the process of reconfiguring message to be sent
    else {
      Serial.print("Force: ");
      Serial.print(force);
      
                                                                                              // MQTT can only send Strings
      String fs="Force: "+String((float)force);
      
                                                                                              // Tries to publish the data, and checks if it is sent
      if (client.publish(force_topic, String(force).c_str())) {
        Serial.println("Force sent!");
      } 
  
                                                                                              // If data is not able to be sent to the server, try to reconnect to broker and send again
      else {
        Serial.println("Force failed to send. Reconnecting to MQTT broker and trying again");
        reconnect();
        client.publish(force_topic, String(force).c_str());
      }
      delay(250);
    }
  }
}



// Setup function for the client
void setup() {
  pinMode(LedR, OUTPUT);
  pinMode(LedG, OUTPUT);
  pinMode(LedB, OUTPUT);
  Serial.begin(115200);                                                                       // setting the serial data transmission of bits pretty high to ensure correct data being sent
  Serial.setTimeout(500);                                                                     // Delay between wait for serial data lowered from 1000 to 500
  setup_wifi();                                                                               // starts up wifi connection
  client.setServer(mqtt_server, mqtt_port);                                                   // starts up server connection
  client.setCallback(callback);                                                               // starts recording messages from chosen library/libraries
  reconnect();                                                                                // Reconnects to server to transmit data
}

void loop() {
  client.loop();                                           // sets client in a loop with the server
  callback;                                                // checks incoming messages
  delay(1000);
}
