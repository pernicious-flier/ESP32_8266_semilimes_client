#ifndef semilimes_h
#define semilimes_h

#include <Arduino.h>

#define semilimes_wss_server "wss://cloud.semilimes.net/CloudServer/wsclient"
#define SendRestfulMessageRequest_api "https://cloud.semilimes.net/CloudServer/api/3A01CE9E-F850-4049-AD45-DA372E44B89B"

class semilimes
{
	public:
		String SendTextMessage(String AuthToken, String ReceiverID, String Body);
		String SendSelectOptions(String AuthToken, String ReceiverID, String Body, String* OptionTexts, String* OptionValues, int OptionsNumber);
		String SendLocation(String AuthToken, String ReceiverID, String Body, String latitude, String longitude);
		String SendHTML(String AuthToken, String ReceiverID, String Body);
		String PickDate(String AuthToken, String ReceiverID, String Body);
		String PickTime(String AuthToken, String ReceiverID, String Body);
		String PickLocation(String AuthToken, String ReceiverID, String Body);
		String SendJSON(String AuthToken, String ReceiverID, String TypeId, String Body);
		String SendRestfulMessageRequest(String token, String ReceiverId, String msg);
};

#endif
