#pragma once

enum class SerializationHeaders
{
	ConnectionEvent, // packet associated => <int: connection disconnection (1 -1)>
	Play, // packet associated => 
	PlayResult, // packet associated => { <wether the game state has changed>, <the play itself>, <wether u can play>, <who played> }
	ChatMessage, // packet associated => just text
};

//std::string LegibleSerializationHeaders(SerializationHeaders sH)
//{
//	switch (sH)
//	{
//		case SerializationHeaders::ConnectionEvent:
//			return "ConnectionEvent";
//
//		case SerializationHeaders::Play:
//			return "Play";
//
//		case SerializationHeaders::PlayResult:
//			return "PlayResult";
//
//		case SerializationHeaders::ChatMessage:
//			return "ChatMessage";
//	}
//
//	return "Somehow you made it";
//}