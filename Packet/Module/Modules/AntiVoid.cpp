#include "AntiVoid.h"

AntiVoid::AntiVoid() : IModule(0, Category::MOVEMENT, "Automatically teleports you back up if you fall down more than X blocks") {
	//registerBoolSetting("Hive", &hiveMode, hiveMode);
	//registerIntSetting("distance", &distance, distance, 1, 10);
}

AntiVoid::~AntiVoid() {
}

const char* AntiVoid::getModuleName() {
	return ("AntiVoid");
}

void AntiVoid::onTick(C_GameMode* gm) {
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	vec3_t blockBelow = player->eyePos0;
	blockBelow.y -= player->height;
	blockBelow.y -= 0.5f;

	if (((player->region->getBlock(blockBelow)->blockLegacy))->blockId != 0 && ((player->region->getBlock(blockBelow)->blockLegacy))->material->isSolid) {
		savepos = blockBelow;
		savepos.y += player->height;
		savepos.y += 0.5f;
	}

	if (player->fallDistance >= distance) {
		//player->setPos(savepos);
		g_Data.getGuiData()->displayClientMessageF("u fell 7 blocks!!!");
		if (hiveMode) {
			if (player->onGround) {
				player->jumpFromGround();
			}
			vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;

			vec3_t pos;
			pos.x = 0.f + pPos.x;
			pos.y = 6.f + pPos.y;
			pos.z = 0.f + pPos.z;

			g_Data.getLocalPlayer()->setPos(pos);
			*g_Data.getClientInstance()->minecraft->timer = 2.f;
		}
		if (player->onGround) {
			auto antivoidMod = moduleMgr->getModule<AntiVoid>();
			if (antivoidMod->isEnabled()) {
				antivoidMod->setEnabled(false);
			}
		}
	}
}

void AntiVoid::onDisable() {
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
}
