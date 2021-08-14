#include "Packet.h"

Packet::Packet() : IModule(0, Category::EXPLOIT, "Multiply All Actions, Server Sided") {
	registerBoolSetting("NoPacket", &noPacket, noPacket);
	registerBoolSetting("Multiply", &multiply, multiply);
	//registerBoolSetting("Multiply4x", &fourx, fourx);
}

Packet::~Packet() {
}

const char* Packet::getModuleName() {
	return ("Packet");
}

bool Packet::allowAutoStart() {
	return false;
}

void Packet::onEnable() {
	if (!g_Data.isInGame())
		setEnabled(false);
}