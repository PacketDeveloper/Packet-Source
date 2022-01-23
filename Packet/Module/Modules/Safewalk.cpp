#include "Safewalk.h"

Safewalk::Safewalk() : IModule(0, Category::MOVEMENT, "SafeWalk") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Normal", 0);
	mode.addEntry("Crouch", 1);
}

const char* Safewalk::getModuleName() {
	return "SafeWalk";
}

void Safewalk::onTick(C_GameMode* gm) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	vec3_t* pos = gm->player->getPos();
	auto player = g_Data.getLocalPlayer();
	vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;
	blockBelow.y -= g_Data.getLocalPlayer()->height;
	blockBelow.y -= 0.5;

	if (player->region->getBlock(blockBelow)->toLegacy()->blockId == 0) {
		if (mode.getSelectedValue() == 1) g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = true;
	} else {
		if (mode.getSelectedValue() == 1) g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
	}

	if (onEdge) {
		if (mode.getSelectedValue() == 0) {
			player->velocity.x = 0;
			player->velocity.z = 0;
		}
		//if (mode.getSelectedValue() == 1) g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = true;
	} else if (!GameData::isKeyDown(*input->sneakKey)) {
		//if (mode.getSelectedValue() == 1) g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
	}
}

void Safewalk::onDisable() {
	if (mode.getSelectedValue() == 1) g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
}