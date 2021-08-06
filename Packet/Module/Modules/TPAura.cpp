#include "TPAura.h"

TPAura::TPAura() : IModule(0, Category::COMBAT, "TPS to the closest entity") {
	this->registerBoolSetting("Silent", &this->silent, this->silent);
	this->registerBoolSetting("Push", &this->push, this->push);
	this->registerIntSetting("TP Delay", &this->delay, this->delay, 0, 10);
	this->registerFloatSetting("Range", &this->range, this->range, 1, 250);
}

const char* TPAura::getModuleName() {
	return ("TPAura");
}

TPAura::~TPAura() {
}

void TPAura::onEnable() {

}

void TPAura::onTick(C_GameMode* gm) {

}

void TPAura::onSendPacket(C_Packet* packet) {
}