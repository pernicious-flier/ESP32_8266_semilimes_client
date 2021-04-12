/*
  semilimes mesh library

  This sketch:
        1. Connects to a WiFi network
        2. Connects to a Websockets server (using WSS)
        3. Enable the callbacks on event and messages 
        4. Create a new device called "ESP Lamp" with 1 output(the blue led onboard) and 2 input (the status of the LED and the status of the "FLASH" button)
        5. Whait for a msg and eventually set/reset the LED or reply with one of the two status

    NOTE:
        In orter to use it you need to install the semilimes app on your smartphone,
        create a channel and save the "Auth Token" and "Channel Id". 

  Hardware:
        For this sketch you only need an ESP8266 board. On the NodeMCU1.0 the blue LED is connected to D0 and the Flash BTN to D3

  used libraries:  
  ArduinoWebsockets - https://github.com/gilmaimon/ArduinoWebsockets
  WiFi - Arduino official lib
  Arduino_JSON - Arduino official lib
  semilimes - https://github.com/pernicious-flier/ESP32_semilimes_Client
  semilimes_device - https://github.com/pernicious-flier/ESP32_semilimes_Client
  
  Created 09/04/2021
  By Flavio Ansovini  https://github.com/pernicious-flier/
*/

#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <semilimes.h>
#include <semilimes_device.h>
#include <Arduino_JSON.h>

const char* ssid = "ssid"; //Enter SSID
const char* password = "password"; //Enter Password
String AuthToken = ".....................";
String ReceiverID = "....................."; //the channel ID

const char* websockets_URL = semilimes_wss_server;
  
using namespace websockets;

WebsocketsClient client;
semilimes semilimes;

#define LED D0
#define BTN D3

void Led_EN(bool en)
{
    digitalWrite(LED, !en);  
}

class digital : public semilimes_device
{
  public:
  virtual int SetOutSpecific(int output_n,int value)
  {
    dev_outputs[output_n]=value;
    if (output_n==1) 
    {
      if(value) Led_EN(HIGH);
      else Led_EN(LOW);
    }
    Serial.println("name: "+dev_name+" id:"+dev_id+" group:"+dev_group+" subg:"+dev_subgroup+" OutN:"+output_n+" value:"+dev_outputs[output_n]);
    return value;
  }
  virtual int GetInSpecific(int input_n)
  {   
    if (input_n==1) 
    {
      dev_inputs[input_n] = dev_outputs[1];
    }
    else if (input_n==2) 
    {
      dev_inputs[input_n] = !digitalRead(BTN);
    }
    Serial.println("name: "+dev_name+" id:"+dev_id+" group:"+dev_group+" subg:"+dev_subgroup+" InN:"+input_n+" value:"+dev_inputs[input_n]);
    return dev_inputs[input_n];
  }
};

digital Lamp;

void JSON_decode(String payload)
{
  JSONVar myObject = JSON.parse(payload);
  
  //Serial.println(payload); //print the whole JSON to terminal, just for debug
  /*
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
  }*/
  if (myObject.hasOwnProperty("ConversationID") && (JSON.stringify(myObject["ConversationID"])=='"'+ReceiverID+'"')) {
    //String msg = JSON.stringify(myObject["FormReference"]); //the "reference" string of a "Button Form" 
    String msg = JSON.stringify(myObject["Body"]);  //the "Option value" of the "Single choice Form"
    //remove the quotation marks
    msg.remove(0,1);
    msg.remove(msg.length()-1,1);
    Serial.println(msg);
    client.send(Lamp.parse_msg(msg,ReceiverID));
  }
}

void onMessageCallback(WebsocketsMessage message) {
    JSON_decode(message.data());
    //Serial.print("Got Message: ");
    //Serial.println(message.data());
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
 
    pinMode(LED, OUTPUT);     // LED pin as output.
    Led_EN(LOW);
    pinMode(BTN, INPUT_PULLUP);  // Flash BTN as input
    
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
    client.connect(semilimes_wss_server);
    
    client.send(semilimes.SendTxtMsg(AuthToken, ReceiverID, semilimes_channel, "This message from ESP8266 using the wss Semilimes libs"));
    
    Lamp.init(AuthToken, "ESP Lamp",1,1,1,1,2); // AuthToken, name of the device, id, group, subgroup, number of outputs, number of inputs
}

void loop() {
    client.poll();
}
