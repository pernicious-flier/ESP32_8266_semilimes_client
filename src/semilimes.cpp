#include "semilimes.h"

String semilimes::SendTxtMsg(String AuthToken, String ReceiverID, int dest_type, String Body)
{
  String TypeID = "590E4E6C-2C5D-47E8-8F38-311D5A299EE7";
  String wsmsg = "{\"AuthToken\": \"" + AuthToken + "\", ";
  wsmsg += "\"Type\": \"chat\", ";
  wsmsg += "\"TypeID\": \"" + TypeID + "\", ";  
  if(dest_type==0) wsmsg += "\"ReceiverID\": \"" + ReceiverID + "\", "; //send msg to a User ID
  else wsmsg += "\"ConversationID\": \"" + ReceiverID + "\", "; //send msg to a Channel
  wsmsg += "\"Body\": \"" + Body + "\"}";

  return wsmsg;
}

String semilimes::SendSelectOpt(String AuthToken, String ReceiverID, int dest_type, String Body, String* OptionTexts, String* OptionValues, int OptionsNumber)
{
  int i;
  String TypeID = "4DB40F80-4C25-454B-BDB4-330A05285D71";
  String wsmsg = "{\"AuthToken\": \"" + AuthToken + "\", ";
  wsmsg += "\"Type\": \"chat\", ";
  wsmsg += "\"TypeID\": \"" + TypeID + "\", ";
  if(dest_type==0) wsmsg += "\"ReceiverID\": \"" + ReceiverID + "\", "; //send msg to a User ID
  else wsmsg += "\"ConversationID\": \"" + ReceiverID + "\", "; //send msg to a Channel
  wsmsg += "\"Body\": \"" + Body + "\", ";
  wsmsg += "\"Layout\": \"flexible\", ";
  wsmsg += "\"Options\": [ ";
  for(i=0;i<OptionsNumber-1;i++)
  {
    wsmsg += "{\"Text\": \"" + *(OptionTexts+i) + "\", \"Value\": \"" + *(OptionValues+i) + "\"},";
  }
  wsmsg += "{\"Text\": \"" + *(OptionTexts+i) + "\", \"Value\": \"" + *(OptionValues+i) + "\"}],";
  wsmsg += "\"Data\": {}}";

  return wsmsg;
}

String semilimes::SendLocation(String AuthToken, String ReceiverID, int dest_type, String Body, String latitude, String longitude)
{
  String TypeID = "4BDD5F50-DC68-11EA-9FCE-5302A705E738";
  String wsmsg = "{\"AuthToken\": \"" + AuthToken + "\", ";
  wsmsg += "\"Type\": \"chat\", ";
  wsmsg += "\"TypeID\": \"" + TypeID + "\", ";
  if(dest_type==0) wsmsg += "\"ReceiverID\": \"" + ReceiverID + "\", "; //send msg to a User ID
  else wsmsg += "\"ConversationID\": \"" + ReceiverID + "\", "; //send msg to a Channel
  wsmsg += "\"Location\": {\"latitude\":" + latitude + ",\"longitude\":" + longitude + "},";
  wsmsg += "\"Body\": \"" + Body + "\"} ";

  return wsmsg;
}

String semilimes::SendHTML(String AuthToken, String ReceiverID, int dest_type, String Body)
{
  String TypeID = "38199F47-504C-4C73-97E5-8076C8CFAA21";
  String wsmsg = "{\"AuthToken\": \"" + AuthToken + "\", ";
  wsmsg += "\"Type\": \"chat\", ";
  wsmsg += "\"TypeID\": \"" + TypeID + "\", ";
  if(dest_type==0) wsmsg += "\"ReceiverID\": \"" + ReceiverID + "\", "; //send msg to a User ID
  else wsmsg += "\"ConversationID\": \"" + ReceiverID + "\", "; //send msg to a Channel
  wsmsg += "\"Body\": \"" + Body + "\"} ";

  return wsmsg;
}

String semilimes::ReceiveDate(String AuthToken, String ReceiverID, int dest_type, String Body)
{
  String TypeID = "242B5A3B-C1AF-4663-BD97-E296E3DB4D2F";
  String wsmsg = "{\"AuthToken\": \"" + AuthToken + "\", ";
  wsmsg += "\"Type\": \"chat\", ";
  wsmsg += "\"TypeID\": \"" + TypeID + "\", ";
  if(dest_type==0) wsmsg += "\"ReceiverID\": \"" + ReceiverID + "\", "; //send msg to a User ID
  else wsmsg += "\"ConversationID\": \"" + ReceiverID + "\", "; //send msg to a Channel
  wsmsg += "\"Body\": \"" + Body + "\", ";
  wsmsg += "\"Data\": {}}";

  return wsmsg;
}

String semilimes::ReceiveTime(String AuthToken, String ReceiverID, int dest_type, String Body)
{
  String TypeID = "F489C072-2C8B-4BC6-AD75-946D3CA721B7";
  String wsmsg = "{\"AuthToken\": \"" + AuthToken + "\", ";
  wsmsg += "\"Type\": \"chat\", ";
  wsmsg += "\"TypeID\": \"" + TypeID + "\", ";
  if(dest_type==0) wsmsg += "\"ReceiverID\": \"" + ReceiverID + "\", "; //send msg to a User ID
  else wsmsg += "\"ConversationID\": \"" + ReceiverID + "\", "; //send msg to a Channel
  wsmsg += "\"Body\": \"" + Body + "\", ";
  wsmsg += "\"Data\": {}}";

  return wsmsg;
}

String semilimes::ReceiveLocation(String AuthToken, String ReceiverID, int dest_type, String Body)
{
  String TypeID = "20A0CE4B-A236-4E96-9629-45A3AF5F62EA";
  String wsmsg = "{\"AuthToken\": \"" + AuthToken + "\", ";
  wsmsg += "\"Type\": \"chat\", ";
  wsmsg += "\"TypeID\": \"" + TypeID + "\", ";
  if(dest_type==0) wsmsg += "\"ReceiverID\": \"" + ReceiverID + "\", "; //send msg to a User ID
  else wsmsg += "\"ConversationID\": \"" + ReceiverID + "\", "; //send msg to a Channel
  wsmsg += "\"Body\": \"" + Body + "\", ";
  wsmsg += "\"Data\": {}}";

  return wsmsg;
}

String semilimes::SendJSON(String AuthToken, String ReceiverID, int dest_type, String TypeID, String Body)
{
  String wsmsg = "{\"AuthToken\": \"" + AuthToken + "\", ";
  wsmsg += "\"Type\": \"chat\", ";
  wsmsg += "\"TypeID\": \"" + TypeID + "\", ";
  if(dest_type==0) wsmsg += "\"ReceiverID\": \"" + ReceiverID + "\", "; //send msg to a User ID
  else wsmsg += "\"ConversationID\": \"" + ReceiverID + "\", "; //send msg to a Channel
  wsmsg += "\"Body\": \"" + Body + "\", ";
  wsmsg += "\"Data\": {}}";

  return wsmsg;
}

String semilimes::RequestNmsgs(String AuthToken, String ReceiverID, int dest_type, int MessageCount, String StartTime, String BeforeTime)
{
  String TypeID = "DF55B2B3-03B0-48C6-8C3F-32995FAB10E4";
  String wsmsg = "{\"AuthToken\": \"" + AuthToken + "\", ";
  wsmsg += "\"Type\": \"client\", ";
  wsmsg += "\"TypeID\": \"" + TypeID + "\", ";
  if(dest_type==0) wsmsg += "\"ReceiverID\": \"" + ReceiverID + "\", "; //send msg to a User ID
  else wsmsg += "\"ConversationID\": \"" + ReceiverID + "\", "; //send msg to a Channel
  if(StartTime!="") wsmsg += "\"StartTime\": \"" + StartTime + "\", ";
  if(BeforeTime!="") wsmsg += "\"BeforeTime\": \"" + BeforeTime + "\", ";
  wsmsg += "\"MessageCount\": \"" + String(MessageCount) + "\",}";  

  return wsmsg;	
}

String semilimes::SendRestMsgReq(String token, String ReceiverID, int dest_type, String msg)
{
  // Data to send with HTTP POST
  String restfulMsg = "{ ";
  restfulMsg += "\"Token\": \"" + token + "\", ";
  restfulMsg += "\"Message\": { ";
  restfulMsg += "\"Type\": \"chat\", ";
  if(dest_type==0) restfulMsg += "\"ReceiverID\": \"" + ReceiverID + "\", "; //send msg to a User ID
  else restfulMsg += "\"ConversationID\": \"" + ReceiverID + "\", "; //send msg to a Channel
  restfulMsg += "\"Body\": \"" + msg + "\"}";
  restfulMsg += "}}";        
    
  return restfulMsg;
}