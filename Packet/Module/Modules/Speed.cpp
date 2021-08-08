#include "Speed.h"
Speed::Speed() : IModule(0, Category::MOVEMENT, "sped lol") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Vanilla", 0);
	mode.addEntry("Hive", 1);
#ifdef _DEBUG
	mode.addEntry("HiveGround", 2);
	mode.addEntry("KowSpecial", 3);
#endif
	registerIntSetting("TimerBoost", &timer, timer, 20, 35);
	registerFloatSetting("Height", &height, height, 0.000001f, 0.40f);
	registerFloatSetting("Speed", &speed, speed, 0.2f, 2.f);
}

Speed::~Speed() {
}

const char* Speed::getModuleName() {
	return ("Speed");
}

void Speed::onEnable() {
	if (mode.getSelectedValue() == 3) {
		auto box = g_Data.addInfoBox("Speed", "Gamer Mode Enabled!");
		box->closeTimer = 5.f;
	}
}

void Speed::onTick(C_GameMode* gm) {
	*g_Data.getClientInstance()->minecraft->timer = static_cast<float>(timer);
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
		static bool velocity = false;
		if (height >= 0.385) { // very stupid code
			if (player->onGround && pressed) player->jumpFromGround();
			velocity = false;
		} else {
			velocity = true;
		}
		if (player->onGround && pressed && !input->isJumping && velocity) {
			player->velocity.y = height;
		}
		if (!pressed && player->damageTime == 0) {
			player->velocity.x *= 0;
			player->velocity.z *= 0;
		}
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
		if (!pressed && player->damageTime == 0) {
			player->velocity.x = 0;
			player->velocity.z = 0;
		}
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
			if (input->right || input->left) {
				*g_Data.getClientInstance()->minecraft->timer = 19.f;
			}
		}
	}
	if (mode.getSelectedValue() == 3 && g_Data.isInGame()) {
		if (player == nullptr) return;

		vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
		bool pressed = moveVec2d.magnitude() > 0.01f;

		if (player->onGround && pressed) {
			player->jumpFromGround();
			player->velocity.y = -0.1f;
			player->velocity.y = 0.23;
		}
			

		float calcYaw = (player->yaw + 90) * (PI / 180);
		vec3_t moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
		moveVec.x = moveVec2d.x * speed;
		moveVec.y = player->velocity.y;
		moveVec.z = moveVec2d.y * speed;
		if (pressed) player->lerpMotion(moveVec);
		if (input->right || input->left) {
			*g_Data.getClientInstance()->minecraft->timer = 19.f;
		}

		if (g_Data.getLocalPlayer()->velocity.squaredxzlen() > 0.01) {
			C_MovePlayerPacket p = C_MovePlayerPacket(g_Data.getLocalPlayer(), player->getPos()->add(vec3_t(moveVec.x / 1.3f, 0.f, moveVec.z / 1.3f)));
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
			C_MovePlayerPacket p2 = C_MovePlayerPacket(g_Data.getLocalPlayer(), player->getPos()->add(vec3_t(player->velocity.x / 1.3f, 0.f, player->velocity.z / 2.3f)));
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p2);
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

void Speed::onSendPacket(C_Packet* packet) {
	auto player = g_Data.getLocalPlayer();
	if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr && mode.getSelectedValue() == 3 && g_Data.isInGame()) {
		auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
		float myPitchq = player->pitch;
		float myYawq = player->yaw;
		float bodyYawq = player->bodyYaw;
		movePacket->pitch = myPitchq;
		movePacket->headYaw = myYawq;
	}
}