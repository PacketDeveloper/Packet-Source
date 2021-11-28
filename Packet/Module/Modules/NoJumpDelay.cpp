#include "NoJumpDelay.h"

NoJumpDelay::NoJumpDelay() : IModule(0, Category::PLAYER, "No delay between jumps") {
}
NoJumpDelay::~NoJumpDelay() {
}

void NoJumpDelay::onTick(C_GameMode* gm) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;

	if (gm->player->onGround && GameData::isKeyDown(*input->spaceBarKey)) {
		gm->player->jumpFromGround();
	}
}

const char* NoJumpDelay::getModuleName() {
	return "NoJumpDelay";
}