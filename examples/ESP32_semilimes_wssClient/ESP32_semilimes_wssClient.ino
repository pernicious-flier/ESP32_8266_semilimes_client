/*
	Secured Esp32 Semilimes Websocket client

	This sketch:
        1. Connects to a WiFi network
        2. Connects to the semilimes Websockets server (using WSS)
        3. Sends the semilimes websockets server some messages (txt, otions, location. HTML, pick location, pick date, pick time, JSON)
        4. Prints all incoming messages while the connection is open and decode the JSON

	Hardware:
        For this sketch you only need an Esp32 board.

	Created 20/12/2020
	By Flavio Ansovini
	https://github.com/pernicious-flier/ESP32_semilimes_Client.git

*/

#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <semilimes.h>
#include <Arduino_JSON.h>


const char* ssid = "Enter SSID";
const char* password = "Enter Password";
String AuthToken = "Enter your Auth Code";
    
const char* websockets_connection_string = semilimes_wss_server;
  
using namespace websockets;
semilimes semilimes;

void JSON_decode(String payload)
{
  JSONVar myObject = JSON.parse(payload);
  
  // JSON.typeof(jsonVar) can be used to get the type of the var
  if (JSON.typeof(myObject) == "undefined") {
    Serial.println("Parsing input failed!");
    return;
  }
  
  if (myObject.hasOwnProperty("ConversationID")) {
    Serial.print("myObject[\"ConversationID\"] = ");
    Serial.println(myObject["ConversationID"]);
  }
  if (myObject.hasOwnProperty("SenderID")) {
    Serial.print("myObject[\"SenderID\"] = ");
    Serial.println(myObject["SenderID"]);
  }  
  if (myObject.hasOwnProperty("ClientID")) {
    Serial.print("myObject[\"ClientID\"] = ");
    Serial.println(myObject["ClientID"]);
  }
  if (myObject.hasOwnProperty("MessageID")) {
    Serial.print("myObject[\"MessageID\"] = ");
    Serial.println(myObject["MessageID"]);
  }
  if (myObject.hasOwnProperty("Body")) {
    Serial.print("myObject[\"Body\"] = ");
    Serial.println(myObject["Body"]);
  }
}

void onMessageCallback(WebsocketsMessage message) {
    Serial.print("Got Message: ");
    Serial.println(message.data());
    JSON_decode(message.data());
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

WebsocketsClient client;
void setup() {
    Serial.begin(115200);
    // Connect to wifi
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // run callback when messages are received
    client.onMessage(onMessageCallback);
    
    // run callback when events are occuring
    client.onEvent(onEventsCallback);

    // Connect to server
    client.connect(websockets_connection_string);

    String Body = "This message from ESP32 using the wss Semilimes libs";
    String ReceiverID = "Enter the Channel ID";
    client.send(semilimes.SendTextMessage(AuthToken, ReceiverID, Body));
    
    Body = "Select Options from ESP32 using the wss Semilimes libs";
    String OptionTexts[3];
    String OptionValues[3];
    OptionTexts[0] = "option 0";
    OptionTexts[1] = "option 1";
    OptionTexts[2] = "option 2";
    OptionValues[0] = "100";
    OptionValues[1] = "200";
    OptionValues[2] = "300";
    client.send(semilimes.SendSelectOptions(AuthToken, ReceiverID, Body, OptionTexts, OptionValues, 3));
    
    Body = "This Location from ESP32 using the wss Semilimes libs";
    String latitude = "10.8169596";
    String longitude = "106.6837259";
    client.send(semilimes.SendLocation(AuthToken, ReceiverID, Body, latitude, longitude));
    
    Body = "<html> \
    <body> \
    <h1>My First Heading</h1> \
    <p>My first paragraph.</p> \
    </body> \
    </html>";
    client.send(semilimes.SendHTML(AuthToken, ReceiverID, Body));
    
     Body = "Pick a Date";
     client.send(semilimes.PickDate(AuthToken, ReceiverID, Body));
    
     Body = "Pick a Time";
     client.send(semilimes.PickTime(AuthToken, ReceiverID, Body));
     
     Body = "Pick a Location";
     client.send(semilimes.PickLocation(AuthToken, ReceiverID, Body));

     String TypeID = "38199F47-504C-4C73-97E5-8076C8CFAA21";  //send HTML
     Body = "<html> \
    <body> \
    <h1>My First json Heading</h1> \
    <p>My first json paragraph.</p> \
    </body> \
    </html>";
    client.send(semilimes.SendJSON(AuthToken, ReceiverID, TypeID, Body));
}

void loop() {
    client.poll();
}
