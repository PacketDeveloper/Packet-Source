#include "Step.h"

Step::Step() : IModule(0, Category::MOVEMENT, "Increases your step height") {
	registerBoolSetting("Reverse", &reverse, reverse);
	registerFloatSetting("Height", &height, height, 1.f, 2.f);
}

Step::~Step() {
}

const char* Step::getModuleName() {
	if (isEnabled()) {
		static char modName[30];  // This is kinda ghetto rn, there should be a better way to make this...
		sprintf_s(modName, 30, "Step %.1f", height);
		return modName;
	} else
		return ("Step");
}

void Step::onTick(C_GameMode* gm) {
	auto clickGUI = moduleMgr->getModule<ClickGuiMod>();
	if (clickGUI->isEnabled()) {
		clickGUIE = true;
	} else {
		clickGUIE = false;
	}
	gm->player->stepHeight = height;
	if (reverse) {
		if (gm->player->onGround && !gm->player->isInWater() && !gm->player->isInLava()) {
			gm->player->velocity.y = -1;
		}
	}
}

void Step::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr)
		g_Data.getLocalPlayer()->stepHeight = 0.5625f;
}
