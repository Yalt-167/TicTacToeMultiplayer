#pragma once

#include "SerializationHeaders.hpp"

struct PacketHeader
{
public:
	int What;
	int Size;

	PacketHeader& Set(int what, int size)
	{
		What = what;
		Size = size;

		return *this;
	}
	PacketHeader& Set(SerializationHeaders what, int size)
	{
		What = (int)what;
		Size = size;

		return *this;
	}
private:
};


// packets constructed as such:
// send packets two by two

// header
// <what><size> -> <int, int> // can easily infer size tho (except for chat msg)

// then
// <body> of size header.<size>