#include "../../Module/ModuleManager.h"

FastStop::FastStop() : IModule(0, Category::MOVEMENT, "Stops fast yes") {
}

FastStop::~FastStop() {
}

const char* FastStop::getModuleName() {
	return ("FastStop");
}

void FastStop::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	vec2_t move = {input->forwardMovement, -input->sideMovement};
	bool pressed = move.magnitude() > 0.f;
	if (~player->onGround)
		if (!pressed) {
			player->velocity.x = 0;
			player->velocity.z = 0;
		}
}