#include "Emote.h"

Emote::Emote() : IModule(0, Category::MISC, "Multiply All Actions, Server Sided") {
	registerBoolSetting("4x", &fourx, fourx);
}

Emote::~Emote() {
}

const char* Emote::getModuleName() {
	return ("PacketMultiplier");
}