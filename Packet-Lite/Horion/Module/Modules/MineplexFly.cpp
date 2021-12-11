#include "MineplexFly.h"

MineplexFly::MineplexFly() : IModule(0, Category::MOVEMENT, "Bypassing fly for Mineplex") {
	this->glideMod = 0.00f;
}

MineplexFly::~MineplexFly() {
}

const char* MineplexFly::getModuleName() {
	if (isEnabled()) {
		static char modName[30];  //Hello world! Goodbye im commiting suiicide. 
		sprintf_s(modName, 30, "MineplexFly", glideModEffective);
		return modName;
	} else
		return ("MineplexFly");
}

void MineplexFly::onTick(C_GameMode* gm) {
	glideModEffective = glideMod;
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	if (g_Data.canUseMoveKeys()) {
		if (GameData::isKeyDown(*input->spaceBarKey))
			glideModEffective += 0.0f;
		if (GameData::isKeyDown(*input->sneakKey))
			glideModEffective -= 0.0f;
	}
	gm->player->velocity.y = glideModEffective;

	//timer code
	*g_Data.getClientInstance()->minecraft->timer = static_cast<float>(120);

}
void MineplexFly::onDisable() {
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
}

const char* MineplexFly::getRawModuleName() {
	return "MineplexFly";
}

