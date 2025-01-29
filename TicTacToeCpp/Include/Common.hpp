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
#include <functional>
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