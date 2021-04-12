/*
  semilimes mesh library

  This sketch:
        1. Connects to a WiFi network
        2. Connects to a Websockets server (using WSS)
        3. Enable the callbacks on event and messages 
        4. Sends the semilimes channel a "text" msg form
        5. Sends the semilimes channel a "select option" msg form
        6. Sends the semilimes channel a "geographic location" msg form
        7. Sends the semilimes channel an "HTML" msg form
        8. Sends the semilimes channel a "Picka date" msg form
        9. Sends the semilimes channel a "Picka time" msg form
        10. Sends the semilimes channel a "Picka location" msg form
        11. Sends the semilimes channel a plain "JSON" msg form
        12. Wait for a callback and in the case of "onMessageCallback" will 
            decode ther JSON and print to terminal some properties

    NOTE:
        In orter to use it you need to install the semilimes app on your smartphone,
        create a channel and save the "Auth Token" and "Channel Id". 

  Hardware:
        For this sketch you only need an ESP8266 board.

  used libraries:  
  ArduinoWebsockets - https://github.com/gilmaimon/ArduinoWebsockets
  WiFi - Arduino official lib
  Arduino_JSON - Arduino official lib
  semilimes - https://github.com/pernicious-flier/ESP32_semilimes_Client
  
  Created 09/04/2021
  By Flavio Ansovini  https://github.com/pernicious-flier/
*/

#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <semilimes.h>
#include <Arduino_JSON.h>

const char* ssid = "ssid"; //Enter SSID
const char* password = "password"; //Enter Password
String AuthToken = ".....................";
String ReceiverID = "....................."; //the channel ID
 
const char* websockets_URL = semilimes_wss_server;
  
using namespace websockets;

WebsocketsClient client;
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

void setup() {    
    Serial.begin(115200);
    // Connect to wifi
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }

    // run callback when messages are received
    client.onMessage(onMessageCallback);
    
    // run callback when events are occuring
    client.onEvent(onEventsCallback);
    
    // Connect to server
    client.connect(websockets_URL);
    
    String Body = "This message from ESP8266 using the wss Semilimes libs";
    client.send(semilimes.SendTxtMsg(AuthToken, ReceiverID, semilimes_channel, Body));
    
    Body = "Select Options from ESP8266 using the wss semilimes libs";
    String OptionTexts[3];
    String OptionValues[3];
    OptionTexts[0] = "option 0";
    OptionTexts[1] = "option 1";
    OptionTexts[2] = "option 2";
    OptionValues[0] = "100";
    OptionValues[1] = "200";
    OptionValues[2] = "300";
    client.send(semilimes.SendSelectOpt(AuthToken, ReceiverID, semilimes_channel, Body, OptionTexts, OptionValues, 3));
    
    Body = "This Location from ESP8266 using the wss Semilimes libs";
    String latitude = "10.8169596";
    String longitude = "106.6837259";
    client.send(semilimes.SendLocation(AuthToken, ReceiverID, semilimes_channel, Body, latitude, longitude));
    
    Body = "<html> \
    <body> \
    <h1>My First Heading</h1> \
    <p>My first paragraph.</p> \
    </body> \
    </html>";
    client.send(semilimes.SendHTML(AuthToken, ReceiverID, semilimes_channel, Body));
    
    Body = "Pick a Date";
    client.send(semilimes.ReceiveDate(AuthToken, ReceiverID, semilimes_channel, Body));
    
    Body = "Pick a Time";
    client.send(semilimes.ReceiveTime(AuthToken, ReceiverID, semilimes_channel, Body));
    
    Body = "Pick a Location";
    client.send(semilimes.ReceiveLocation(AuthToken, ReceiverID, semilimes_channel, Body));
    
     String TypeID = "38199F47-504C-4C73-97E5-8076C8CFAA21";  //send HTML
     Body = "<html> \
    <body> \
    <h1>My First json Heading</h1> \
    <p>My first json paragraph.</p> \
    </body> \
    </html>";
    client.send(semilimes.SendJSON(AuthToken, ReceiverID, semilimes_channel, TypeID, Body));
}

void loop() {
    client.poll();
}
