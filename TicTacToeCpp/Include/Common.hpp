#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/window.hpp"
#include "SFML/system.hpp"

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include<functional>
#include <chrono>

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // just found out about this: this is great


constexpr const int PORT = 54000;
constexpr const int CHAT_BUFFER_SIZE = 1024;

class Discarder
{
public:
	template<typename T> constexpr inline void operator=(T&) const {}
	template<typename T> constexpr inline void operator=(T&&) const {}
private:
};

constexpr inline Discarder _;

class Game_
{
public:
	virtual void Run() = 0;
protected:
private:
};


enum class PacketSendTarget
{
	Broadcast,
	NotSelf
};

enum class SerializationHeaders
{
	Login,
	Play,
	ChatMessage,
};

enum class Plays
{
	InvalidPlay = 9,
	ValidPlay = 10,
	PlayerOneWon = 11,
	Draw = 12,
	PlayerTwoWon = 13,
};

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