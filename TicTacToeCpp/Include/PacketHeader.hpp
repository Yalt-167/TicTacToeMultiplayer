#pragma once

#include "SerializationHeaders.hpp"

struct PacketHeader
{
public:
	SerializationHeaders SerializationHeader;
	int Size;

	// returns self as char* (basically self but serialized)
	char* Set(const SerializationHeaders serializationHeader, const  int size)
	{
		SerializationHeader = serializationHeader;
		Size = size;

		return reinterpret_cast<char*>(this);
	}

    static std::string LegibleSerializationHeaders(SerializationHeaders sH)
    {
        switch (sH)
        {
        case SerializationHeaders::Disconnection:
            return "ConnectionEvent";

        case SerializationHeaders::Play:
            return "Play";

        case SerializationHeaders::PlayResult:
            return "PlayResult";

        case SerializationHeaders::ChatMessage:
            return "ChatMessage";

        case SerializationHeaders::CatchupPacket:
            return "CatchupPacket";
        }

        return "Somehow you made it here";
    }
private:
};


// packets constructed as such:
// send packets two by two

// header
// <what><size> -> <SerializationHeaders, int> // can easily infer size tho (except for chat msg)

// then body
// <body> -> <char[<header.Size>]>