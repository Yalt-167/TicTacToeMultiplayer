#pragma once

enum class SerializationHeaders
{
	ConnectionEvent,
	Play,
	PlayResult,
	ChatMessage,
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