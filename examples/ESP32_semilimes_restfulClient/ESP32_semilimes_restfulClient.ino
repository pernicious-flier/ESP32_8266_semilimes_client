/*
  Esp32 Semilimes restful client

  This sketch:
        1. Connects to a WiFi network
        2. Sends the Semilimes HTTP server a text message 
        3. Prints the incoming message and decode the JSON

  Hardware:
        For this sketch you only need an Esp32 board.

  Created 20/12/2020
  By Flavio Ansovini
  https://github.com/pernicious-flier/ESP32_semilimes_Client.git

*/
#include <WiFi.h>
#include <semilimes.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>

const char* ssid = "flavio_wifi";
const char* password = "flaviowifi27177";
String myToken = " Enter your Token Here";
String ChannelId = "Enter the Channel ID";

semilimes semilimes;
HTTPClient http;

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
  // Semilimes Server URL + API code
  http.begin(SendRestfulMessageRequest_api);  
  // Specify content-type header
  http.addHeader("Content-Type", "application/json");
  String restfulMsg = semilimes.SendRestfulMessageRequest(myToken, ChannelId, "This message from ESP32 using the restful Semilimes libs");
  int httpResponseCode = http.POST(restfulMsg);
  
  if(httpResponseCode>0)
  {
    String response = http.getString(); //Get the response to the request
    //Serial.println(response);           //Print request answer
    JSON_decode(response);
  }
  else
  {
    Serial.print("Error on sending POST: ");
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  //Serial.println(serverResponse);
}

void JSON_decode(String payload)
{
  JSONVar myObject = JSON.parse(payload);
  
  // JSON.typeof(jsonVar) can be used to get the type of the var
  if (JSON.typeof(myObject) == "undefined") {
    Serial.println("Parsing input failed!");
    return;
  }
  
  //Serial.print("JSON object = ");
  //Serial.println(myObject);
  
  // myObject.keys() can be used to get an array of all the keys in the object
  JSONVar keys = myObject.keys();  //the first key is the message and the second is the status of the stransaction  
  JSONVar Message = myObject[keys[0]];  //take the message
  
  if (Message.hasOwnProperty("ConversationID")) {
    Serial.print("Message[\"ConversationID\"] = ");
    Serial.println(Message["ConversationID"]);
  }
  
  if (Message.hasOwnProperty("SenderID")) {
    Serial.print("Message[\"SenderID\"] = ");
    Serial.println(Message["SenderID"]);
  }
  
  if (Message.hasOwnProperty("MessageID")) {
    Serial.print("Message[\"MessageID\"] = ");
    Serial.println(Message["MessageID"]);
  }
  
  if (Message.hasOwnProperty("Body")) {
    Serial.print("Message[\"Body\"] = ");
    Serial.println(Message["Body"]);
  }
}

void loop() {

}
