#include "Freecam.h"

#include "../../Module/ModuleManager.h"

Freecam::Freecam() : IModule(0, Category::MISC, "burn") {
	registerFloatSetting("Speed", &this->speed, this->speed, 0.5f, 3.f);
}

Freecam::~Freecam() {
}

const char* Freecam::getModuleName() {
	return ("Freecam");
}

void Freecam::onEnable() {
	if (g_Data.getLocalPlayer() != nullptr) {
		g_Data.getLocalPlayer()->setGameModeType(3);
	}
	oldPos = *g_Data.getLocalPlayer()->getPos();
}

void Freecam::onTick(C_GameMode* gm) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	gm->player->aabb.upper.y = gm->player->aabb.lower.y;
	gm->player->fallDistance = 0.f;
	gm->player->aabb.upper.y = 0.f;
	auto player = g_Data.getLocalPlayer();
	float yaw = player->yaw;
	if (input->forwardKey && input->backKey && input->rightKey && input->leftKey) {
		gm->player->velocity = vec3_t(0, 0, 0);
	}
		player->velocity.y = 0;
	auto speedMod = moduleMgr->getModule<Speed>();
	auto flyMod = moduleMgr->getModule<Flight>();
	if (speedMod->isEnabled()) {
		speedMod->setEnabled(false);
	}
	if (flyMod->isEnabled()) {
		flyMod->setEnabled(false);
	}
}

void Freecam::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	float yaw = player->yaw;
	if (player == nullptr) return;
	if (player->isSneaking())
		return;
	vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.01f;
	if (input->right) {
		yaw += 90.f;
		if (input->forward)
			yaw -= 45.f;
		else if (input->backward)
			yaw += 45.f;
	}
	if (input->left) {
		yaw -= 90.f;
		if (input->forward)
			yaw += 45.f;
		else if (input->backward)
			yaw -= 45.f;
	}
	if (input->backward && !input->left && !input->right)
		yaw += 180.f;
	if (pressed) {
		float calcYaw = (yaw + 90.f) * (PI / 180.f);
		vec3_t moveVec;
		moveVec.x = cos(calcYaw) * speed;
		moveVec.y = player->velocity.y;
		moveVec.z = sin(calcYaw) * speed;
		if (pressed) player->lerpMotion(moveVec);
	}
}

void Freecam::onDisable() {
	auto player = g_Data.getLocalPlayer();
	if (g_Data.getLocalPlayer() != nullptr) {
		if (g_Data.getLocalPlayer()->gamemode != 1) {
			g_Data.getLocalPlayer()->setGameModeType(0);
		}
	}
	g_Data.getLocalPlayer()->setPos(oldPos);
	player->velocity.x = 0.f;
	player->velocity.y = 0.f;
	player->velocity.z = 0.f;
}