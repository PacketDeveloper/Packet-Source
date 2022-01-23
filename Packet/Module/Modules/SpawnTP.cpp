#include "SpawnTP.h"

SpawnTP::SpawnTP() : IModule(0, Category::EXPLOIT, "TPS you to the spawnpoint") {
}

const char* SpawnTP::getModuleName() {
	return "SpawnTP";
}

void SpawnTP::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	auto player = g_Data.getLocalPlayer();
	player->setSleeping(true);
	auto notification = g_Data.addInfoBox("SpawnTP:", "Teleported!");
	notification->closeTimer = 9;
	setEnabled(false);
}

void SpawnTP::onTick(C_GameMode* gm) {
}

void SpawnTP::onDisable() {
}