#include "Speed.h"
Speed::Speed() : IModule(0, Category::MOVEMENT, "sped") {
	registerEnumSetting("Mode", &this->mode, 0);
	mode.addEntry("Vanilla", 0);
	mode.addEntry("Hive", 1);
	mode.addEntry("HiveGround", 2);
	registerIntSetting("TimerBoost", &this->timer, this->timer, 20, 35);
	registerFloatSetting("Height", &this->height, this->height, 0.000001f, 0.40f);
	registerFloatSetting("Speed", &this->speed, this->speed, 0.2f, 2.f);
}

Speed::~Speed() {
}

const char* Speed::getModuleName() {
	return ("Speed");
}

void Speed::onEnable() {
}

void Speed::onTick(C_GameMode* gm) {
	*g_Data.getClientInstance()->minecraft->timer = static_cast<float>(this->timer);
	auto scaffold = moduleMgr->getModule<Scaffold>();
	if (scaffold->speedLockY) {
		scaffold->lockY = true;
	} else {
		scaffold->lockY = false;
	}
}

void Speed::onMove(C_MoveInputHandler* input) {
	vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.f;
	auto player = g_Data.getLocalPlayer();
	if (mode.getSelectedValue() == 0) {  // Vanilla
		if (!lowMode) {
			if (player->onGround && pressed)
				player->jumpFromGround();
			player->velocity.y = height;
			player->velocity.y -= height / 3;
		} else if (player->onGround && pressed)
			player->velocity.y = height;
		if (!pressed)
			player->velocity.x *= 0;
		player->velocity.z *= 0;
		float calcYaw = (player->yaw + 90) * (PI / 180);
		vec3_t moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
		moveVec.x = moveVec2d.x * speed;
		moveVec.y = player->velocity.y;
		moveVec.z = moveVec2d.y * speed;
		if (pressed) player->lerpMotion(moveVec);
	}
	if (mode.getSelectedValue() == 1) {  // Hive
		/*auto player = g_Data.getLocalPlayer();
		vec2_t movement = {input->forwardMovement, -input->sideMovement};
		bool pressed = movement.magnitude() > 0.f;
		if (player->onGround && pressed)
			player->jumpFromGround();
		float calcYaw = (player->yaw + 90) * (PI / 180);
		vec3_t moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		movement = {movement.x * c - movement.y * s, movement.x * s + movement.y * c};
		moveVec.x = movement.x * speed;
		moveVec.y = player->velocity.y;
		moveVec.z = movement.y * speed;
		if (pressed)
			player->lerpMotion(moveVec);
		else if (input->sideMovement) {
			float calcYaw = (player->yaw + 90) * (PI / 180);
			vec3_t moveVec;
			float c = cos(calcYaw);
			float s = sin(calcYaw);
			movement = {movement.x * c - movement.y * s, movement.x * s + movement.y * c};
			moveVec.x = movement.x * 0.4;
			moveVec.y = player->velocity.y;
			moveVec.z = movement.y * 0.4;
			if (pressed) player->lerpMotion(moveVec);
		}
		vec2_t movement = {input->forwardMovement, -input->sideMovement};
		bool pressed = movement.magnitude() > 0.f;
		if (jumpMode && !lowMode) {
			if (player->onGround && pressed)
				player->jumpFromGround();
		} else {
			if (player->onGround && pressed)
				player->velocity.y += height /+ 1;
			player->velocity.y += height / 4;

			float calcYaw = (player->yaw + 90) * (PI / 180);
			vec3_t moveVec;
			float c = cos(calcYaw);
			float s = sin(calcYaw);
			movement = {movement.x * c - movement.y * s, movement.x * s + movement.y * c};
			moveVec.x = movement.x *=+ speed;
			moveVec.y = player->velocity.y;
			moveVec.z = movement.y *=+ speed;
			if (pressed)
				player->lerpMotion(moveVec);
		}*/
		auto player = g_Data.getLocalPlayer();
		vec2_t movement = {input->forwardMovement, -input->sideMovement};
		bool pressed = movement.magnitude() > 0.f;
		float calcYaw = (player->yaw + 90) * (PI / 180);
		vec3_t moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		movement = {movement.x * c - movement.y * s, movement.x * s + movement.y * c};
		if (player->onGround && pressed) player->jumpFromGround();
		moveVec.x = movement.x *= 0.315;
		moveVec.y = player->velocity.y;
		moveVec.z = movement.y *= 0.315;
		if (pressed) player->lerpMotion(moveVec);
	}
	if (mode.getSelectedValue() == 2) {  // HiveGround
		auto scaffold = moduleMgr->getModule<Scaffold>();
		float calcYaw = (player->yaw + 90) * (PI / 180);
		vec3_t moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
		if (input->isJumping && !scaffold->isEnabled()) {
			moveVec.x = moveVec2d.x * 0.30;
			moveVec.y = player->velocity.y;
			moveVec.z = moveVec2d.y * 0.30;
			if (pressed) player->lerpMotion(moveVec);
		} else {
			moveVec.x = moveVec2d.x * speed;
			moveVec.y = player->velocity.y;
			moveVec.z = moveVec2d.y * speed;
			if (pressed) player->lerpMotion(moveVec);
			if (!pressed) {
				player->velocity.x *= 0;
				player->velocity.z *= 0;
			}
			if (input->right) {
				*g_Data.getClientInstance()->minecraft->timer = 19.f;
			}
			if (input->left) {
				*g_Data.getClientInstance()->minecraft->timer = 19.f;
			}
		}
	}
}

void Speed::onDisable() {
	auto scaffold = moduleMgr->getModule<Scaffold>();
	auto player = g_Data.getLocalPlayer();
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
	if (!player->onGround) {
		player->velocity.x = 0.f;
		player->velocity.z = 0.f;
	}
	if (scaffold->speedLockY) {
		scaffold->lockY = false;
	}
}