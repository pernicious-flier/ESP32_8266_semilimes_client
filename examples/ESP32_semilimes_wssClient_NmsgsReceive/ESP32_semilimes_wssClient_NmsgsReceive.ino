/*
  semilimes mesh library

  This sketch:
        1. Connects to a WiFi network
        2. Connects to a Websockets server (using WSS)
        3. Enable the callbacks on event and messages 
        4. Sends the semilimes channel a "text" msg form
        5. Ask the semilimes server to send the last "n" msgs
        6. Ask the semilimes server to send "n" msgs before the the last timestamp
        7. Ask the semilimes server to send other "n" msgs before the the last timestamp

    NOTE:
        In orter to use it you need to install the semilimes app on your smartphone,
        create a channel and save the "Auth Token" and "Channel Id". 

  Hardware:
        For this sketch you only need an ESP32 board.

  used libraries:  
  ArduinoWebsockets - https://github.com/gilmaimon/ArduinoWebsockets
  WiFi - Arduino official lib
  Arduino_JSON - Arduino official lib
  semilimes - https://github.com/pernicious-flier/ESP32_semilimes_Client
  
  Created 09/04/2021
  By Flavio Ansovini  https://github.com/pernicious-flier/
*/

#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <semilimes.h>
#include <Arduino_JSON.h>

const char* ssid = "ssid"; //Enter SSID
const char* password = "password"; //Enter Password
String AuthToken = ".....................";
String ReceiverID = "....................."; //the channel ID
  
const char* websockets_connection_string = semilimes_wss_server;

using namespace websockets;
semilimes semilimes;

String FrstRcvMsgTimestamp;

String JSONarraySplit(String payload, int splitNum)
{
  int startIndex, stopIndex;
  
  startIndex = payload.indexOf("{");

  for(int i=0; i<splitNum; i++)
  {
    startIndex = payload.indexOf('{', startIndex+1);
    stopIndex = payload.indexOf('}', startIndex)+1;
  }
  if(startIndex<0 || stopIndex<0)
  {
    return "null";
  }
  else
  {
    //Serial.println(payload.substring(startIndex,stopIndex));
    return payload.substring(startIndex,stopIndex);
  }
}

void JSON_decode(String payload)
{
  JSONVar myObject = JSON.parse(payload);
  
  // JSON.typeof(jsonVar) can be used to get the type of the var
  if (JSON.typeof(myObject) == "undefined") {
    Serial.println("Parsing input failed!");
    return;
  }  

  //Serial.print("myObject.keys() = ");
  //Serial.println(myObject.keys());
 
  
  if (myObject.hasOwnProperty("Messages")) 
  {
    int i=1;
    JSONVar myObjectTemp = JSON.parse(JSONarraySplit(payload,i));
    while(!JSONarraySplit(payload,i).equals("null"))
    {
      if(myObjectTemp.hasOwnProperty("Body")) {
        Serial.print("myObject[\"Body\"] = ");
        Serial.print(myObjectTemp["Body"]);
        Serial.print(" - ");
        Serial.print("myObject[\"UnixTime\"] = ");
      }
      if(myObjectTemp.hasOwnProperty("UnixTime")) {  
        String msg = JSON.stringify(myObjectTemp["UnixTime"]);
        if(i==1) FrstRcvMsgTimestamp = msg;
        Serial.println(msg);
      }
      i++;
      myObjectTemp = JSON.parse(JSONarraySplit(payload,i));
    }
    //Serial.println(FrstRcvMsgTimestamp);
  }
  else
  {
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
    client.send(semilimes.SendTxtMsg(AuthToken, ReceiverID, semilimes_channel, Body));
   
    Serial.printf("ask for last %d messages before now\r\n", 2);
    client.send(semilimes.RequestNmsgs(AuthToken, ReceiverID, semilimes_channel, 2, "",""));
  
}

int cnt = 0;
void loop() {
    client.poll();
    if(FrstRcvMsgTimestamp!="" && cnt < 2)
    {
      Serial.printf("ask for last %d messages before %d msgs\r\n", 2, 2*(cnt+1));
      cnt++;
      client.send(semilimes.RequestNmsgs(AuthToken, ReceiverID, semilimes_channel, 2, "", FrstRcvMsgTimestamp));
      FrstRcvMsgTimestamp = "";
    }
}
