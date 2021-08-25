#include "HiveFly.h"

#include "../../Module/ModuleManager.h"
#include "TestModule.h"

HiveFly::HiveFly() : IModule(0, Category::MOVEMENT, "FlightModule") {
	registerBoolSetting("TimerBoost", &timerBoost, timerBoost);
	registerBoolSetting("DmgBoost", &dmgBoost, dmgBoost);
	//registerBoolSetting("Boost", &this->boost, this->boost);
	registerBoolSetting("Strafe", &strafeMode, strafeMode);
	registerFloatSetting("Value", &value, value, -0.02f, 0.001f);
	registerFloatSetting("Speed", &speed, speed, 0.10f, 1.f);
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
		speed->setEnabled(false);
		auto boxo = g_Data.addInfoBox("HiveFly: Disabled speed");
		boxo->closeTimer = 6;
		speedWasEnabled = true;
	}
	if (scaffold->isEnabled()) {
		scaffold->setEnabled(false);
		auto boxo = g_Data.addInfoBox("HiveFly: Disabled scaffold");
		boxo->closeTimer = 6;
		scfWasEnabled = true;
	}
	auto player = g_Data.getLocalPlayer();
	if (dmgBoost)
		player->animateHurt();
}

void HiveFly::onTick(C_GameMode* gm) {
	auto scaffoldMod = moduleMgr->getModule<Scaffold>();
	auto bhopMod = moduleMgr->getModule<Speed>();
	scaffoldMod->setEnabled(false);
	bhopMod->setEnabled(false);
	auto player = g_Data.getLocalPlayer();
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	if (counter == 3) {
		*g_Data.getClientInstance()->minecraft->timer = 1.f;
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
		gm->player->velocity = vec3_t(0.f, -0.001f, 0.f);
	}
	/*if (counter >= 5) {
		gm->player->velocity.y += 0.1;
	}
	if (counter == 6) {
		gm->player->velocity.y -= 0.5545697;
		*g_Data.getClientInstance()->minecraft->timer = 9.f;

	}*/
	if (counter == 2) {
		player->velocity.y = value;
	} else if (counter == 3) {
		*g_Data.getClientInstance()->minecraft->timer = 15.f;
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
		}
		if (input->isJumping && counter == 3) {
		}
		if (input->isSneakDown && counter == 3) {
		}
		if (input->forward)
			yaw -= 45.f;
		else if (input->backward)
			yaw += 45.f;
		if (!input->isJumping) {
		}
	}
	if (input->left) {
		yaw -= 90.f;
		if (!input->isJumping) {
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
		moveVec.x = cos(calcYaw) * speed;  // Value
		moveVec.y = player->velocity.y;
		moveVec.z = sin(calcYaw) * speed;  // Value
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
		auto boxo = g_Data.addInfoBox("HiveFly: Re-Enabled speed");
		boxo->closeTimer = 6;
	}
	if (scfWasEnabled == true) {
		scaffold->setEnabled(true);
		scfWasEnabled = false;
		auto boxo = g_Data.addInfoBox("HiveFly: Re-Enabled scaffold");
		boxo->closeTimer = 6;
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
