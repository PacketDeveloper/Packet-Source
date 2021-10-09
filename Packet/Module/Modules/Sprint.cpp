#include "../../Module/ModuleManager.h"
#include "Sprint.h"

Sprint::Sprint() : IModule(0, Category::MOVEMENT, "Sprints automatically.") {
}

Sprint::~Sprint() {
}

const char* Sprint::getModuleName() {
	return ("Sprint");
}

void Sprint::onTick(C_GameMode* gm) {
	if (useSprint) {
		if (!gm->player->isSprinting() && gm->player->velocity.magnitudexz() > 0.01f) {
			C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
			if (omniSprint || GameData::isKeyDown(*input->forwardKey))
				gm->player->setSprinting(true);
		}
	}
}
