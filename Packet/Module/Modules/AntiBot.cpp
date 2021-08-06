#include "AntiBot.h"

AntiBot::AntiBot() : IModule(0, Category::PLAYER, "Enable this module to filter bots!") {
	registerBoolSetting("Invisible Check", &invisibleCheck, invisibleCheck);
	registerBoolSetting("EntityID Check", &entityIdCheck, entityIdCheck);
	registerBoolSetting("Hitbox Check", &hitboxCheck, hitboxCheck);
	registerBoolSetting("Other Check", &otherCheck, otherCheck);
	registerBoolSetting("Extra Check", &extraCheck, extraCheck);
	registerBoolSetting("Name Check", &nameCheck, nameCheck);
}

AntiBot::~AntiBot() {
}

const char* AntiBot::getModuleName() {
	return ("AntiBot");
}
