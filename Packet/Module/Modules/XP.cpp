#include "XP.h"

XP::XP() : IModule(0, Category::MISC, "Gives you experience levels") {
	registerBoolSetting("Levels", &levels, levels);
	registerIntSetting("Amount", &amount, amount, 1, 250);
}

const char* XP::getModuleName() {
	return "XP";
}

void XP::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (levels) {
		player->addLevels(amount);
	} else {
		player->addExperience(amount);
	}
}