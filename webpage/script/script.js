const colours = ["255,0,0", "0,0,255", "0,255,0", "255,255,0", "0,255,255"];
var clr_nmr = 0;
var mode = "morse";
const morseCode = {
    "A": ".-",
    "B": "-...",
    "C": "-.-.",
    "D": "-..",
    "E": ".",
    "F": "..-.",
    "G": "--.",
    "H": "....",
    "I": "..",
    "J": ".---",
    "K": "-.-",
    "L": ".-..",
    "M": "--",
    "N": "-.",
    "O": "---",
    "P": ".--.",
    "Q": "--.-",
    "R": ".-.",
    "S": "...",
    "T": "-",
    "U": "..-",
    "V": "...-",
    "W": ".--",
    "X": "-..-",
    "Y": "-.--",
    "Z": "--.."
}


client = new Paho.MQTT.Client("datakommg02.duckdns.org", 9001, "jsClient");
client.id += Math.floor(Math.random() * 1000);

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
var firebaseConfig = {
    apiKey: "AIzaSyCXZasDhdXqjveo41lfnQYDSRl6NSWzbKM",
    authDomain: "datakommgruppeo2.firebaseapp.com",
    databaseURL: "https://datakommgruppeo2.firebaseio.com",
    projectId: "datakommgruppeo2",
    storageBucket: "datakommgruppeo2.appspot.com",
    messagingSenderId: "246262440545",
    appId: "1:246262440545:web:30e3113a14af76a2d3c020",
    measurementId: "G-9BVW8R87T6"
};
// Initialize Firebase
firebase.initializeApp(firebaseConfig);
firebase.analytics();
function writeData(data){ //Sends the input to firebase
    firebase.database().ref("User").push({
        morsekode: data,
        //dekryptert: document.getElementById("Tekstfel").value
    });
}

client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;
client.connect({onSuccess:onConnect,
    useSSL:true,
    userName:"gruppe2",
    password:"gruppe2"});

function onConnect() {
    console.log("onConnect");
    client.subscribe("mqtt/ESP32/#"); // This decides which messages we subscribe to. We are subscribed to all messages from the ESP32 topic.
    message = new Paho.MQTT.Message("Hello");
    message.destinationName = "mqtt/force";
    client.send(message);
}

function onConnectionLost(responseObject) {
    if (responseObject.errorCode !== 0) {
        console.log("onConnectionLost:" + responseObject.errorMessage);
    }
}

function onMessageArrived(message) {
    console.log("onMessageArrived:"+message.payloadString);
    message = message.payloadString;
    message = String(message);
    //writeData(message); TODO: ENABLE IN FINAL BUILD
    if (mode == "log"){
    pressurelight(message);
    document.getElementById("log").innerHTML += (message); //Adds text to the log in the message box
    document.getElementById("log").innerHTML += ("<br />"); //Skips to the next line
    message_box.scrollTop = message_box.scrollHeight; //scrolls down the scroll bar
    }
    if (mode == "morse") {
        document.getElementById("morse_recieved").innerHTML = (message);
    }
}




function showhelp() { //most of the help code is taken from this https://www.w3schools.com/howto/tryit.asp?filename=tryhow_css_modal
    // This code displays a pop up help box for the website.
    document.getElementById("help_box").style.display = "block";
}

function closehelp() { //closes the help box.
    document.getElementById("help_box").style.display = "none";
}

function CheckforEnter(ele) { //runs for every button press, if its enter the message is sent to the server.
    if(event.key === "Enter") {
        if (mode == "morse") { //if morse mode is enabled, the message is sent as morse instead of as text
            //writeData(ele); TODO: ENABLE IN FINAL BUILD Sends the message to firebase before it is converted to morse.
            ele = convertToMorse(ele); //converts the message to morse
            message = new Paho.MQTT.Message(ele);
            message.destinationName = "mqtt/webklient/morsemsg";
        }
        sendMessage(ele);
    }
}

function convertToMorse(text) { //Taken from this website: https://www.tutorialspoint.com/converting-string-to-morse-code-in-javascript
    //converts the text to morse.
    return text.toUpperCase().split("").map(el => {
        return morseCode[el] ? morseCode[el] : el;
    }).join(" ");
}


function sendMessage(val) {     //Sends the message, displays it in console, and sends data to firebase
    // Important note; val is not the message, val is just what you want the console to show. "message" is the message.
    //writeData(val); TODO: ENABLE IN FINAL BUILD Sends morse message to firebase
    client.send(message);
        console.log(val + " was sent to topic: " + message.destinationName)
}

function swaplight() { //Swaps the light on the website
    if (clr_nmr < 4) {
        clr_nmr +=1 ;
    } else {
        clr_nmr = 0;
    }
    document.getElementById("light_box").style.backgroundColor = "rgb("+colours[clr_nmr]+")";
    console.log("Change color to: " + colours[clr_nmr]);

}

function sendlight() { //Sends the current light on the buttomleft corner to the ESP32 to change the color of an LED.
    message = new Paho.MQTT.Message(colours[clr_nmr]);
    message.destinationName = "mqtt/webklient/rgb";
    sendMessage(colours[clr_nmr]); //sends request to server
    document.getElementById("color_text").style.color = "rgb("+colours[clr_nmr]+")";

}

function pressurelight(sensor_reading) { //Changes the light based on sensor readings red = 0 pressure, teal = max, black = null (no data)
        sensor_reading = ((sensor_reading / 12) * 255);
        sensor_reading_reversed = 255 - sensor_reading;
        document.getElementById("pressure_box").style.backgroundColor = "rgb(" + sensor_reading_reversed + "," + sensor_reading + "," + sensor_reading + ")";

    //}
}

function showdisplay(display) { //Swaps the display of the website depending on what mode you want to run. Controlled by clicking on the headers.

    if (display == "log") { //Pressure mode
        document.getElementById("text_box").style.display = "none"
        document.getElementById("log_box").style.display = "initial"
        document.getElementById("morse_text").style.color = "grey"
        document.getElementById("pressure_text").style.color = "White"
        mode = "log";
        message = new Paho.MQTT.Message("Start");
        message.destinationName = "mqtt/webklient/force";
        sendMessage("start");

    } if (display == "message") { // Morse mode
        document.getElementById("log_box").style.display = "none"
        document.getElementById("text_box").style.display = "initial"
        document.getElementById("morse_text").style.color = "white";
        document.getElementById("pressure_text").style.color = "grey";
        mode = "morse";
        message = new Paho.MQTT.Message("Start");
        message.destinationName = "mqtt/webklient/morse";
        sendMessage("start");
    }
}
// TODO: IMPLEMENT BETTER STYLE gjør alt norsk
//TODO: Legg til ny boks under med cheat sheet for morse kode, Kombiner lys.