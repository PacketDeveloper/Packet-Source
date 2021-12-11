#include "Bhop.h"

Bhop::Bhop() : IModule(0, Category::MOVEMENT, "Its ts nabutp, itst its natio its nputp,tp,spd napeee nsd") {
	registerBoolSetting("Hive", &this->test, this->test);
	this->registerIntSetting("TimerBoost", &this->timer, this->timer, 20, 35);
	registerFloatSetting("Amount", &this->speed, this->speed, 0.3f, 2.f);
		registerFloatSetting("Height", &this->height, this->height, 0.15f, 0.60f);
	}

Bhop::~Bhop() {
}

const char* Bhop::getModuleName() {
	return ("Speed");
}

void Bhop::onEnable() {
}

void Bhop::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	if (player->isSneaking())
		return;

	vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.1f;

	if (jumpMode && !lowMode) {
		if (player->onGround && pressed)
			player->jumpFromGround();
	} else {
		if (player->onGround && pressed)
			player->velocity.y += height;
	}
	if (test) {
		if (player->onGround) {
			float yaw = player->yaw;
			if (input->right) {
				yaw += 90.f;
				*g_Data.getClientInstance()->minecraft->timer = 10.f;
				if (input->forward)
					yaw -= 45.f;
				else if (input->backward)
					yaw += 45.f;
				*g_Data.getClientInstance()->minecraft->timer = 10.f;
			}
			if (input->left) {
				yaw -= 90.f;
				*g_Data.getClientInstance()->minecraft->timer = 10.f;
				if (input->forward)
					yaw += 45.f;
				else if (input->backward)
					yaw -= 45.f;
				*g_Data.getClientInstance()->minecraft->timer = 10.f;
			}
			if (input->backward && !input->left && !input->right) {
				yaw += 180.f;
				*g_Data.getClientInstance()->minecraft->timer = 10.f;
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
	}
	if (!test) {
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
}

void Bhop::onTick(C_GameMode* gm) {
	*g_Data.getClientInstance()->minecraft->timer = static_cast<float>(this->timer);
	if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "geo.hivebedrock.network") == 0) {  // Only on The Hive
		if (lowMode) {
		}
		auto stepMod = moduleMgr->getModule<Step>();
		auto kaMod = moduleMgr->getModule<Killaura>();
		auto scaffoldMod = moduleMgr->getModule<Scaffold>();
		if (scaffoldMod->isEnabled()) {
			scaffoldMod->lockY = true;
		}
	}
}

void Bhop::onDisable() {
	auto stepMod = moduleMgr->getModule<Step>();
	{
		stepMod->height = 1.5f;
	}
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
	auto scaffoldMod = moduleMgr->getModule<Scaffold>();
	{
		scaffoldMod->lockY = false;
	}
}