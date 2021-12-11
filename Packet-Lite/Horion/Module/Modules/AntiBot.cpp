#include "AntiBot.h"

AntiBot::AntiBot() : IModule(0, Category::PLAYER, "Filter bots") {
	registerBoolSetting("Invisible Check", &this->invisibleCheck, this->invisibleCheck);
	registerBoolSetting("EntityID Check", &this->entityIdCheck, this->entityIdCheck);
	registerBoolSetting("Hitbox Check", &this->hitboxCheck, this->hitboxCheck);
	//registerBoolSetting("Name Check", &this->nameCheck, this->nameCheck);
	registerBoolSetting("Other Check", &this->otherCheck, this->otherCheck);
	registerBoolSetting("Extra Check", &this->extraCheck, this->extraCheck);
}

AntiBot::~AntiBot() {
}

const char* AntiBot::getModuleName() {
	return ("AntiBot");
}
