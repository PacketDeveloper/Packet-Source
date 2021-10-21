#include "AntiVoid.h"

AntiVoid::AntiVoid() : IModule(0, Category::MOVEMENT, "Teleports you back up if you fall in the void") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Lagback", 0);
	mode.addEntry("Up", 1);
	registerBoolSetting("VoidCheck", &voidCheck, voidCheck);
	registerIntSetting("Distance", &distance, distance, 3, 10);
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
	blockBelow.y -= 0.5;

	if (player->onGround && ((player->region->getBlock(blockBelow)->blockLegacy))->blockId != 0) {
		savedPos = blockBelow;
		savedPos.y += player->height;
	}		savedPos.y += 0.5f;

    /*if (voidCheck) {
		vec3_t* pos = gm->player->getPos();
			for (int y = (int)pos->y - 255; y < pos->y; y++) {
			if (((player->region->getBlock(vec3_t{blockBelow})->blockLegacy))->blockId == 0) {
				foundVoid = true;
			} else {
				foundVoid = false;
			}
		}
	}
	if (foundVoid) {
		//clientMessageF("found void");
	}*/

	if (player->fallDistance >= distance) {
		//if (voidCheck && foundVoid)
			//return;
		if (mode.getSelectedValue() == 0) player->setPos(savedPos); // Lagback
	}
}

void AntiVoid::onDisable() {
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
}
