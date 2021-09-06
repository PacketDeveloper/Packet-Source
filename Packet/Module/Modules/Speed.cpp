#include "Speed.h"
Speed::Speed() : IModule(0, Category::MOVEMENT, "sped lol") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Vanilla", 0);
	mode.addEntry("Hive", 1);
	mode.addEntry("KowSpecial", 3);
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
		auto box = g_Data.addInfoBox("Speed: Gamer Mode Enabled!");
		box->closeTimer = 5;
	}

}

void Speed::onTick(C_GameMode* gm) {
	*g_Data.getClientInstance()->minecraft->timer = static_cast<float>(timer);
}

void Speed::onMove(C_MoveInputHandler* input) {
	vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
	//auto targetstrafe = moduleMgr->getModule<TargetStrafeOld>();
	bool pressed = moveVec2d.magnitude() > 0.f;
	auto player = g_Data.getLocalPlayer();
	if (mode.getSelectedValue() == 0) {  // Vanilla
		static bool velocity = false;
		if (height >= 0.385) { // very stupid code - yes
			if (player->onGround && pressed) player->jumpFromGround();
			velocity = false;
		} else {
			velocity = true;
		}
		if (height <= 0.04 && !input->isJumping) {
			player->velocity.y += height;
			velocity = false;
		}
		//if (height >= 0.385 || height <= 0.04) {
			//clientMessageF("velocity = true");
			//velocity = true;
		//}
		if (player->onGround && pressed && !input->isJumping && velocity)
			player->velocity.y = height;
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
		}*/
		/*vec2_t movement = {input->forwardMovement, -input->sideMovement};
		bool pressed = movement.magnitude() > 0.f;
		if (height >= 0.385) {
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
		moveVec.z = movement.y *= 0.315; // 0.315
		if (pressed) player->lerpMotion(moveVec);
		if (g_Data.getLocalPlayer()->velocity.squaredxzlen() > 0.01) {
			C_MovePlayerPacket p = C_MovePlayerPacket(g_Data.getLocalPlayer(), player->getPos()->add(vec3_t(moveVec.x / 5.f, 0.f, moveVec.z / 5.f)));
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
			C_MovePlayerPacket p2 = C_MovePlayerPacket(g_Data.getLocalPlayer(), player->getPos()->add(vec3_t(player->velocity.x / 1.3f, 0.f, player->velocity.z / 2.3f)));
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p2);
		}
	}
	if (mode.getSelectedValue() == 2) {  // HiveGround - unused
		auto scaffold = moduleMgr->getModule<Scaffold>();
		float calcYaw = (player->yaw + 90) * (PI / 180);
		vec3_t moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
		if (!player->onGround && !scaffold->isEnabled()) {
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
			if (input->right || input->left)
				*g_Data.getClientInstance()->minecraft->timer = 19.f;
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

		if (player->onGround) {
			player->fallDistance = 0;
			preventKick = false;
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
		if (input->right || input->left)
			*g_Data.getClientInstance()->minecraft->timer = 19.f;

		if (g_Data.getLocalPlayer()->velocity.squaredxzlen() > 0.01) {
			C_MovePlayerPacket p = C_MovePlayerPacket(g_Data.getLocalPlayer(), player->getPos()->add(vec3_t(moveVec.x / 1.3f, 0.f, moveVec.z / 1.3f)));
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
			C_MovePlayerPacket p2 = C_MovePlayerPacket(g_Data.getLocalPlayer(), player->getPos()->add(vec3_t(player->velocity.x / 1.3f, 0.f, player->velocity.z / 2.3f)));
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p2);
		}

		if (player->fallDistance >= 3 && !preventKick) {
			player->velocity = vec3_t(0, -1, 0);
			player->fallDistance = 0;
			preventKick = true;
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
	preventKick = false;
}

void Speed::onSendPacket(C_Packet* packet) {
	auto player = g_Data.getLocalPlayer();
	if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr && mode.getSelectedValue() == 3 && g_Data.isInGame()) {
		auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);//poggies
		float myPitchq = player->pitch;
		float myYawq = player->yaw;
		float bodyYawq = player->bodyYaw;
		//movePacket->pitch = myPitchq;
		//movePacket->headYaw = myYawq;

		C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

		if (input == nullptr)
			return;

		float yaw = player->yaw;

		if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->backKey))
			return;
		else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
			yaw += 45.f;
		} else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
			yaw -= 45.f;
		} else if (GameData::isKeyDown(*input->backKey) && GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
			yaw += 135.f;
		} else if (GameData::isKeyDown(*input->backKey) && GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
			yaw -= 135.f;
		} else if (GameData::isKeyDown(*input->forwardKey)) {
		} else if (GameData::isKeyDown(*input->backKey)) {
			yaw += 180.f;
		} else if (GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
			yaw += 90.f;
		} else if (GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
			yaw -= 90.f;
		}
		if (yaw >= 180)
			yaw -= 360.f;
		float calcYaw = (yaw + 90) * (PI / 180);

		movePacket->headYaw = yaw;
	}
}