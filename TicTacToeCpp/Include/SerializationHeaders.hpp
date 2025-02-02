#pragma once

enum class SerializationHeaders
{
	Disconnection, // packet associated => none
	Play, // packet associated => int[1]
	// <int: slot>
	PlayResult, // packet associated => int[3]
	// {
	// <int: the play itself>
	// <int: wether u can play>,
	// <int: who played>
	// }
	ChatMessage, // packet associated => char[sizeSpecifiedInHeader]
	// <char*, the message>
	CatchupPacket, // packet associated => int[9]
	// basically the grid state if u got disconnected
};