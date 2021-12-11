#include "Glide.h"

Glide::Glide() : IModule(0, Category::MOVEMENT, "Works on most servers, Do not glide upwards on The Hive.") {
	this->registerFloatSetting("value", &this->glideMod, this->glideMod, -0.01, 0.00);
}

Glide::~Glide() {
}

const char* Glide::getModuleName() {
	if (isEnabled()) {
		static char modName[30];  //Hello
		sprintf_s(modName, 30, "Glide", glideModEffective);
		return modName;
	} else
		return ("Glide");
}

void Glide::onTick(C_GameMode* gm) {
	glideModEffective = glideMod;
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	if (g_Data.canUseMoveKeys()) {
		if (GameData::isKeyDown(*input->spaceBarKey))
			glideModEffective += 0.0f;
		if (GameData::isKeyDown(*input->sneakKey))
			glideModEffective -= 0.0f;
	}
	gm->player->velocity.y = glideModEffective;
}

const char* Glide::getRawModuleName() {
	return "Glide";
}
