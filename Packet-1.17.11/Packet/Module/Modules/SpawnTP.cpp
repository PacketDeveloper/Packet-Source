#include "SpawnTP.h"

SpawnTP::SpawnTP() : IModule(0, Category::EXPLOIT, "TPS you to the spawnpoint") {
}

const char* SpawnTP::getModuleName() {
	return "SpawnTP";
}

void SpawnTP::onEnable() {
	auto player = g_Data.getLocalPlayer();
	player->setSleeping(true); // lol
	//setEnabled(false);
}

void SpawnTP::onTick(C_GameMode* gm) {
	setEnabled(false);
}

void SpawnTP::onDisable() {
	auto player = g_Data.getLocalPlayer();
	player->setSleeping(false);  // lol2
	auto box = g_Data.addInfoBox("Teleported!");
	box->closeTimer = 15;
}