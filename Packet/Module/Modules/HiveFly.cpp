#include "HiveFly.h"

#include "../../Module/ModuleManager.h"
#include "TestModule.h"

HiveFly::HiveFly() : IModule(0, Category::MOVEMENT, "FlightModule") {
	registerBoolSetting("TimerBoost", &timerBoost, timerBoost);
	registerBoolSetting("DmgBoost", &dmgBoost, dmgBoost);
	//registerBoolSetting("Boost", &this->boost, this->boost);
	registerBoolSetting("Strafe", &strafeMode, strafeMode);
	registerFloatSetting("Value", &speed2, speed2, -0.02f, 0.f);
	registerFloatSetting("Speed", &speedA, speedA, 0.10f, 1.f);
}

HiveFly::~HiveFly() {
}

const char* HiveFly::getModuleName() {
	return ("HiveFly");
}

void HiveFly::onEnable() {
	auto scaffold = moduleMgr->getModule<Scaffold>();
	auto speed = moduleMgr->getModule<Speed>();
	if (speed->isEnabled()) {
		speedWasEnabled = true;
	}
	if (scaffold->isEnabled()) {
		scfWasEnabled = true;
	}
	if (boost) {
		counter2 = 1;
	}
	auto player = g_Data.getLocalPlayer();
	if (dmgBoost)
		player->animateHurt();
}

void HiveFly::onTick(C_GameMode* gm) {
	if (boost) {
		if (counter2 == 1000) {
			//g_Data.getGuiData()->displayClientMessageF("[R] Reset counter");
			counter2 = 1;
		} else {
			counter2++;
		}
	}
	auto player = g_Data.getLocalPlayer();
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	if (counter == 10) {
		counter = 1;
	} else {
		counter++;
	}
	if (timerBoost) {  // Boost Mode Slowdown
		*g_Data.getClientInstance()->minecraft->timer = 27.4124367756534f;
	} else {
		*g_Data.getClientInstance()->minecraft->timer = 20.f;
	}
	if (strafeMode) {
		auto killauraMod = moduleMgr->getModule<Killaura>();
		killauraMod->strafe = true;
	}
	float yaw = player->yaw;
	if (counter >= 0) {
		gm->player->velocity = vec3_t(0, 0, 0);
	}
	if (counter == 1) {
		player->velocity.y = speed2;
		if (boost)
			if (boost) {
				auto dmgMod = moduleMgr->getModule<TestModule>();
				if (dmgMod->isEnabled()) {
					dmgMod->setEnabled(false);
				}
			}
		auto scaffoldMod = moduleMgr->getModule<Scaffold>();  // Disable
		if (scaffoldMod->isEnabled()) {
			scaffoldMod->setEnabled(false);
		}
		auto bhopMod = moduleMgr->getModule<Speed>();  // Disable
		if (bhopMod->isEnabled()) {
			bhopMod->setEnabled(false);
		}
	}
}

void HiveFly::onMove(C_MoveInputHandler* input) {
	if (boost) {  // Boost Upwards
		auto player = g_Data.getLocalPlayer();
		vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
		bool pressed = moveVec2d.magnitude() > 0.01f;
		if (player->onGround)
			player->jumpFromGround();
		vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;
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
		player->fallDistance = 0;
		vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;

		vec3_t pos;
		pos.x = 0.f + pPos.x;
		pos.y = 2.f + pPos.y;
		pos.z = 0.f + pPos.z;
		*g_Data.getClientInstance()->minecraft->timer = 5.f;
		g_Data.getLocalPlayer()->setPos(pos);
	}
	if (input->isSneakDown && counter == 3) {
		player->velocity.y -= 0.000000000001f;
		player->fallDistance = -0.1f;
		vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;

		vec3_t pos;
		pos.x = 0.f + pPos.x;
		pos.y = -2.f + pPos.y;
		pos.z = 0.f + pPos.z;
		*g_Data.getClientInstance()->minecraft->timer = 5.f;
		g_Data.getLocalPlayer()->setPos(pos);
	}
	if (input->right) {
		yaw += 90.f;
		if (!input->isJumping) {
			//*g_Data.getClientInstance()->minecraft->timer = 20.f;
		}
		if (input->isJumping && counter == 3) {
			//*g_Data.getClientInstance()->minecraft->timer = 20.f;
		}
		if (input->isSneakDown && counter == 3) {
			//*g_Data.getClientInstance()->minecraft->timer = 20.f;
		}
		if (input->forward)
			yaw -= 45.f;
		else if (input->backward)
			yaw += 45.f;
		if (!input->isJumping) {
			//*g_Data.getClientInstance()->minecraft->timer = 20.f;
		}
		if (input->isJumping && counter == 3) {
			//*g_Data.getClientInstance()->minecraft->timer = 20.f;
		}
		if (input->isSneakDown && counter == 3) {
			//*g_Data.getClientInstance()->minecraft->timer = 20.f;
		}
	}
	if (input->left) {
		yaw -= 90.f;
		if (!input->isJumping) {
			//*g_Data.getClientInstance()->minecraft->timer = 20.f;
		}
		if (input->isJumping && counter == 3) {
			//*g_Data.getClientInstance()->minecraft->timer = 20.f;
		}
		if (input->isSneakDown && counter == 3) {
			//*g_Data.getClientInstance()->minecraft->timer = 20.f;
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
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
	auto player = g_Data.getLocalPlayer();
	// re-enable modules
	auto scaffold = moduleMgr->getModule<Scaffold>();
	auto speed = moduleMgr->getModule<Speed>();
	if (speedWasEnabled == true) {
		speed->setEnabled(true);
		speedWasEnabled = false;
	}
	if (scfWasEnabled == true) {
		scaffold->setEnabled(true);
		scfWasEnabled = false;
	}
	if (!player->onGround) {
		player->velocity.x = 0.f;
		player->velocity.z = 0.f;
	}
	if (strafeMode) {
		auto killauraMod = moduleMgr->getModule<Killaura>();
		killauraMod->strafe = false;
	}
}