#include "AntiVoid.h"

AntiVoid::AntiVoid() : IModule(0, Category::MOVEMENT, "Teleports you back up if you fall in the void") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Lagback", 0);
	mode.addEntry("Hive", 1);
	registerBoolSetting("VoidCheck", &voidCheck, voidCheck);
	registerIntSetting("Distance", &distance, distance, 3, 10);
}

AntiVoid::~AntiVoid() {
}

const char* AntiVoid::getModuleName() {
	return ("AntiVoid");
}

void AntiVoid::onEnable() {
	auto player = g_Data.getLocalPlayer();
	savedPos = *player->getPos();
	tick = 0;
}

void AntiVoid::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;
	blockBelow.y -= g_Data.getLocalPlayer()->height;
	blockBelow.y -= 0.5f;

	for (int i = 0; i < 40; i++) {
		if (player->onGround  && player->region->getBlock(blockBelow)->blockLegacy->blockId != 0) {
			savedPos = blockBelow;
			savedPos.y += player->height;
			savedPos.y += 0.5f;
		}
	}

	if (player->fallDistance >= distance) {
		tick++;
		if (!player->onGround && tick >= 5) { // fail safe
			player->velocity.y += 0.056;
		}

		vec3_t pos = savedPos;
		float dist2 = gm->player->getPos()->dist(pos);
		if (mode.getSelectedValue() == 0) player->setPos(savedPos);  // Lagback
		if (mode.getSelectedValue() == 1) { // Hive
			player->lerpTo(pos, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
			*g_Data.getClientInstance()->minecraft->timer = 5.f;
			blink = true;
		}
	} else {
		*g_Data.getClientInstance()->minecraft->timer = 20.f;
		blink = false;
	}
}

void AntiVoid::onDisable() {
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
}