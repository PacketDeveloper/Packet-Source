#include "HiveFly.h"

#include "../../Module/ModuleManager.h"

HiveFly::HiveFly() : IModule(0, Category::MOVEMENT, "Turkey go falling off cliff wee!1!1!!") {
	//registerFloatSetting("Little SHIT", &this->Test, this->Test, 1.f, 50.f);
	//registerBoolSetting("TimerBoost", &this->timerMode, this->timerMode);
	registerBoolSetting("TimerBoost", &this->timerMode, this->timerMode);
	registerBoolSetting("Strafe", &this->strafeMode, this->strafeMode);
	registerBoolSetting("Boost", &this->boostMode, this->boostMode);
	//registerBoolSetting("Old", &this->oldMode, this->oldMode);
	registerFloatSetting("Value", &this->speed2, this->speed2, -0.02f, 0.f);
	registerFloatSetting("Speed", &this->speedA, this->speedA, 0.10f, 1.f);
	registerIntSetting("Boost", &this->timer, this->timer, 10.f, 150.f);
}

HiveFly::~HiveFly() {
}

const char* HiveFly::getModuleName() {
	return "HiveFly";
}

void HiveFly::onEnable() {
	if (boostMode) {
		auto player = g_Data.getLocalPlayer();
		player->animateHurt();
	}
	auto stepMod = moduleMgr->getModule<Step>();
	{
		stepMod->height = 0.5625f;
		if (boostMode) {
			*g_Data.getClientInstance()->minecraft->timer = static_cast<float>(this->timer);
		}
		if (damageMode) {
			auto hfMod = moduleMgr->getModule<HiveFly>();
			if (hfMod->isEnabled()) {
				hfMod->setEnabled(false);
			}
			{
				vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;

				vec3_t pos;
				pos.x = 0.f + pPos.x;
				pos.y = 0.f + pPos.y;
				pos.z = 0.f + pPos.z;

				g_Data.getLocalPlayer()->setPos(pos);
				*g_Data.getClientInstance()->minecraft->timer = 6000.f;
			}
			auto scaffoldMod = moduleMgr->getModule<Scaffold>();
			if (scaffoldMod->isEnabled()) {
				scaffoldMod->setEnabled(false);
			}
			auto bhopMod = moduleMgr->getModule<Bhop>();
			if (bhopMod->isEnabled()) {
				bhopMod->setEnabled(false);
			}
			auto player = g_Data.getLocalPlayer();
		}
		if (oldMode) {
			vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;

			vec3_t pos;
			pos.x = 0.f + pPos.x;
			pos.y = 0.f + pPos.y;
			pos.z = 0.f + pPos.z;

			g_Data.getLocalPlayer()->setPos(pos);
		}
	}
}

bool HiveFly::isFlashMode() {
	return false;
}

void HiveFly::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	if (counter == 10) {
		counter = 1;
	} else {
		counter++;
	}
	if (!timerMode) {
		*g_Data.getClientInstance()->minecraft->timer = 20.f;
	}
	if (timerMode) {
		*g_Data.getClientInstance()->minecraft->timer = 27.4124367756534f;  // Timer 1
	}
	if (strafeMode) {
		auto kaMod = moduleMgr->getModule<Killaura>();
		kaMod->cock = true;
	}
	/*if (!strafeMode) {
		auto kaMod = moduleMgr->getModule<Killaura>();
		kaMod->cock = false;
	}*/
	float yaw = player->yaw;
	if (counter >= 0) {
		gm->player->velocity = vec3_t(0, 0, 0);
	}
	if (counter == 1) {
		player->velocity.y = speed2;
		if (timerMode)
			*g_Data.getClientInstance()->minecraft->timer = 26.23446256676f;  // Timer 2
	}
	if (timerMode) {
		auto dmgMod = moduleMgr->getModule<TestModule>();
		if (dmgMod->isEnabled()) {
			dmgMod->setEnabled(false);
		}
	}
	auto scaffoldMod = moduleMgr->getModule<Scaffold>();  // Disable
	if (scaffoldMod->isEnabled()) {
		scaffoldMod->setEnabled(false);
	}
	auto bhopMod = moduleMgr->getModule<Bhop>();  // Disable
	if (bhopMod->isEnabled()) {
		bhopMod->setEnabled(false);
	}
}

void HiveFly::onMove(C_MoveInputHandler* input) {
	if (boostMode) {  // Boost Upwards
		auto player = g_Data.getLocalPlayer();
		vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
		bool pressed = moveVec2d.magnitude() > 0.01f;
		if (player->onGround && pressed)
			player->jumpFromGround();
	}

	auto player = g_Data.getLocalPlayer();
	float yaw = player->yaw;
	if (player == nullptr) return;
	if (player->isSneaking())
		return;
	vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.01f;
	if (input->isJumping && counter == 3) {
		player->velocity.y -= 0.000000000001f;
		player->fallDistance = -0.1f;
		vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;

		vec3_t pos;
		pos.x = 0.f + pPos.x;
		pos.y = 4.f + pPos.y;
		pos.z = 0.f + pPos.z;

		g_Data.getLocalPlayer()->setPos(pos);
		*g_Data.getClientInstance()->minecraft->timer = 3.f;
	}
	if (input->isSneakDown && counter == 3) {
		player->velocity.y -= 0.000000000001f;
		player->fallDistance = -0.1f;
		vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;

		vec3_t pos;
		pos.x = 0.f + pPos.x;
		pos.y = -4.f + pPos.y;
		pos.z = 0.f + pPos.z;

		g_Data.getLocalPlayer()->setPos(pos);
		*g_Data.getClientInstance()->minecraft->timer = 3.f;
	}
	if (input->right) {
		yaw += 90.f;
		if (!input->isJumping) {
			*g_Data.getClientInstance()->minecraft->timer = 18.f;
		}
		if (input->isJumping && counter == 3) {
			*g_Data.getClientInstance()->minecraft->timer = 3.f;
		}
		if (input->isSneakDown && counter == 3) {
			*g_Data.getClientInstance()->minecraft->timer = 3.f;
		}
		if (input->forward)
			yaw -= 45.f;
		else if (input->backward)
			yaw += 45.f;
		if (!input->isJumping) {
			*g_Data.getClientInstance()->minecraft->timer = 18.f;
		}
		if (input->isJumping && counter == 3) {
			*g_Data.getClientInstance()->minecraft->timer = 3.f;
		}
		if (input->isSneakDown && counter == 3) {
			*g_Data.getClientInstance()->minecraft->timer = 3.f;
		}
	}
	if (input->left) {
		yaw -= 90.f;
		if (!input->isJumping) {
			*g_Data.getClientInstance()->minecraft->timer = 18.f;
		}
		if (input->isJumping && counter == 3) {
			*g_Data.getClientInstance()->minecraft->timer = 3.f;
		}
		if (input->isSneakDown && counter == 3) {
			*g_Data.getClientInstance()->minecraft->timer = 3.f;
		}
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
		moveVec.x = cos(calcYaw) * speedA;  // Value
		moveVec.y = player->velocity.y;
		moveVec.z = sin(calcYaw) * speedA;  // Value
		if (pressed) player->lerpMotion(moveVec);
	}
}

void HiveFly::onDisable() {
	auto stepMod = moduleMgr->getModule<Step>();
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
	if (strafeMode) {
		auto killauraMod = moduleMgr->getModule<Killaura>();
		killauraMod->cock = false;
	}
	{
		stepMod->height = 1.5f;
		auto player = g_Data.getLocalPlayer();
		if (!player->onGround) {
			player->velocity.x = 0.f;
			player->velocity.z = 0.f;
		}
	}
}