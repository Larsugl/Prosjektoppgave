#include <PubSubClient.h>
#include <analogWrite.h>
#include <WiFi.h>

// Wifi
const char* ssid = "<Router Address>";                                                          // Your personal network SSID
const char* wifi_password = "<Router Password>";                                                // Your personal network password

// MQTT
const char* mqtt_server = "datakommg02.duckdns.org";                                            // IP of the MQTT broker
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
    Serial.println(topic);
    Serial.print("data:");  
    //Serial.write(payload, length);
    //Serial.println();
    payload[length] ='\0';
    String info = String((char*)payload);
    Serial.println(info);
    Serial.println();
    String topicStr = topic;
    if (topicStr == "mqtt/webklient/morse")
    {
      morse();
      msg = " ";
      delay(1000);
    }
    else if (topicStr == "mqtt/webklient/force")
    {
      force();
      delay(1000);
    }
    else if (topicStr == "mqtt/webklient/rgb")
    {
      String red = getValue(info, ',', 0);
      String green = getValue(info, ',', 1);
      String blue = getValue(info, ',', 2);
      int redVal = red.toInt();
      int greenVal = green.toInt();
      int blueVal = blue.toInt();
      Serial.println("Red: " + String(redVal));
      Serial.println("Green: " + String(greenVal));
      Serial.println("Blue: " + String(blueVal));
      rgbled(redVal, greenVal, blueVal);
    }
    else if (topicStr == "mqtt/webklient/morsemsg")
    {
      morseMsg(info);
      delay(1000);
    }
}

void morseMsg(String msg)
{
  int j = msg.length();

  for (int i = 0; i < j; i++)
  {
    if (msg[i] == '-')
    {
      analogWrite(LedR, 255);
      delay(750);
    }
    else if (msg[i] == '.')
    {
      analogWrite(LedR, 255);
      delay(250);
    }
    else if (msg[i] == ' ')
    {
      delay(1250);
    }
   analogWrite(LedR, 0);
   delay(250);
  }
}



String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}



void rgbled(int r,int g,int b)
{
  
  if ((r > 0 && r <= 255) or (g > 0 && g <= 255) or(b > 0 && b <= 255))
  {
    if (r > 0 && r <= 255)
    {
      Serial.println(String(r));
      analogWrite(LedR, r);
    }
    if (g > 0 && g <= 255)
    {
      Serial.println(String(g));
      analogWrite(LedG, g);
    }
    if (b > 0 && b <= 255)
    {
      Serial.println(String(b));
      analogWrite(LedB, b);
    }
  }
  delay(1000);
  analogWrite(LedR, 0);
  analogWrite(LedG, 0);
  analogWrite(LedB, 0);
}


void morse()
{
  while (msg != "")
  {
  NextDotDash:
    while (analogRead(35) == 0) {}
    t1 = millis();                                //time at button press
    while (analogRead(35) >= 10) {}
    t2 = millis();                                //time at button release
    signal_len = t2 - t1;                         //time for which button is pressed
    if (signal_len > 50)                          //to account for switch debouncing
    {
      code += readio();                           //function to read dot or dash
    }
    while ((millis() - t2) < 1000)                //if time between button press greater than 0.5sec, skip loop and go to next alphabet
    {     
      if (analogRead(35) >= 100)
      {
        goto NextDotDash;
      }
    }
    convertor();                                  //function to decipher code into alphabet
  }
}

String readio()
{
  if (signal_len < 500 && signal_len > 50)
  {
    return ".";                                   //if button press less than 0.6sec, it is a dot
  }
  else if (signal_len > 500 && signal_len < 1000)
  {
    return "-";                                   //if button press more than 0.6sec, it is a dash
  }
}

void convertor()
{
  static String letters[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
                             ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "Å"
                            };
  int i = 0;
  if (code == ".-.-.-")
  {
    msg += ".";     //add end of message
    Serial.println(msg);
    delay(1000);
    if (client.publish(morse_topic, msg.c_str())) {
      Serial.println("Morse sent!");
    } 

    // If data is not able to be sent to the server, try to reconnect to broker and send again
    else {
      Serial.println("Morse failed to send. Reconnecting to MQTT broker and trying again");
      reconnect();
      client.publish(morse_topic, msg.c_str());
    }
    msg = "";     //reset msg to blank string
  }
  else if (code == "......")
  {
    msg += " ";
    Serial.println(msg);
  }
  else if (signal_len > 1000)
  {
    int lastChar = msg.length() - 1;
    msg.remove(lastChar);
    Serial.println("Removed last character");
    Serial.println(msg);
  }
  
  else
  {
    while (letters[i] != "Å")                          // loop for comparing input code with letters array
    {
      if (letters[i] == code)
      {
        Serial.println(char('A' + i));
        msg += (char('A' + i));                        // add character to msg
        Serial.println(msg);
        break;                                         // break out of loop
      }
      i++;
    }
    if (letters[i] == "Å")
    {
      Serial.println("<Wrong input>");                 // if input code doesn't match any letter, error
    }
  }
  code = "";                                           // reset code to blank string
}

void force()
{
  int start = 0;
  for (start; start < interval; start++)
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
  Serial.begin(115200);                                   // setting the serial data transmission of bits pretty high to ensure correct data being sent
  Serial.setTimeout(500);                                 // Delay between wait for serial data lowered from 1000 to 500
  setup_wifi();                                           // starts up wifi connection
  client.setServer(mqtt_server, mqtt_port);               // starts up server connection
  client.setCallback(callback);                           // starts recording messages from chosen library/libraries
  reconnect();                                            // Reconnects to server to transmit data
}

void loop() {
  client.loop();                                           // sets client in a loop with the server
  callback;                                                // checks incoming messages
  delay(1000);
}
