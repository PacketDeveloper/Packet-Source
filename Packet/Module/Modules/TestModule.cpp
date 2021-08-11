#include "../../Module/ModuleManager.h"
#include "TestModule.h"

TestModule::TestModule() : IModule(0, Category::MISC, "Description") {
	this->registerBoolSetting("AlertHiveStaff", &this->banUMode, this->banUMode);
	this->registerBoolSetting("AlertBox", &this->alertBox, this->alertBox);
	this->registerBoolSetting("Teleport", &this->istpMode, this->istpMode);
	//this->registerBoolSetting("Teleport2", &this->istpModeTwo, this->istpModeTwo);
	this->registerBoolSetting("Damage", &this->dmgMode, this->dmgMode);
	this->registerBoolSetting("Blink", &this->blink, this->blink);
	this->registerBoolSetting("Test", &this->test, this->test);
	this->registerBoolSetting("AutoDisable", &this->autoDisable, this->autoDisable);
	//this->registerBoolSetting("HiveDmgFly", &this->dmgflyMode, this->dmgflyMode);
}

const char* TestModule::getModuleName() {
	return "TestModule";
}

// most of this code is old so thats why its messy

void TestModule::onEnable() {
	if (banUMode) {
		if (counter == 5) {
			counter = 0;
		} else {
			counter++;
			*g_Data.getClientInstance()->minecraft->timer = 2.f;
			//auto bhopMod = moduleMgr->getModule<Bhop>();
			vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;

			vec3_t pos;
			if (counter >= 0) {
				pos.x = 0.00000000000005f;
				pos.y = -0.000000000000001f + pPos.y;
				pos.z = 0.00000000000005f;
				g_Data.getLocalPlayer()->setPos(pos);
			}
		}
	}

	if (alertBox) {
		auto box = g_Data.addInfoBox("Notification");
		box->closeTimer = 30;
	}
	if (dmgMode) {
		auto speedMod = moduleMgr->getModule<Speed>();
		speedMod->setEnabled(false);
	}
	if (dmgMode) {
		vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;

		vec3_t pos;
		pos.x = 0.f + pPos.x;
		pos.y = 4.5f + pPos.y;
		pos.z = 0.f + pPos.z;

		g_Data.getLocalPlayer()->setPos(pos);
		*g_Data.getClientInstance()->minecraft->timer = 6000.f;
	}
		if (blink) {
			testCounter = 0;
		}
}

bool TestModule::isFlashMode() {
		return false;
}

void TestModule::onTick(C_GameMode* gm) {
	auto blinkMod = moduleMgr->getModule<Blink>();
	if (autoDisable) {
		auto testmoduleMod = moduleMgr->getModule<TestModule>();
		if (testmoduleMod->isEnabled()) {
			*g_Data.getClientInstance()->minecraft->timer = 20.f;
			testmoduleMod->setEnabled(false);
		}
	}
	if (blink) {
		if (testCounter == 60) {
			blinkMod->setEnabled(false);
			testCounter = 1;
		} else {
			testCounter++;
		}
		if (testCounter == 2) {
			blinkMod->setEnabled(true);
		}
	}
	if (test) {
		//g_Data.getLocalPlayer()->setGameModeType(3);
		auto player = g_Data.getLocalPlayer();
				player->velocity.x = 6.f;
				player->velocity.y = 7.f;
		*g_Data.getClientInstance()->minecraft->timer = 20.f;
				player->velocity.z = 5.f;
		//player->animateHurt();
	}
			if (istpMode) {
				auto player = g_Data.getLocalPlayer();
				C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
				float yaw = player->yaw;
				if (input->forwardKey && input->backKey && input->rightKey && input->leftKey) {
					gm->player->velocity = vec3_t(0, 0, 0);
				}
				if (GameData::isKeyDown(*input->spaceBarKey))
					glideModEffective += 0.0f;
				if (GameData::isKeyDown(*input->sneakKey))
					glideModEffective -= 0.0f;
			}
			if (dmgMode) {
				*g_Data.getClientInstance()->minecraft->timer = 20.f;
				auto speedMod = moduleMgr->getModule<Speed>();
				speedMod->setEnabled(false);
			}
		}

void TestModule::onMove(C_MoveInputHandler* input) {
	if (istpMode) {
		auto player = g_Data.getLocalPlayer();
		if (player == nullptr) return;

		vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
		bool pressed = moveVec2d.magnitude() > 0.01f;

		float calcYaw = (player->yaw + 90) * (PI / 180);
		vec3_t moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
		moveVec.x = moveVec2d.x * tpSpeed;
		moveVec.y = player->velocity.y;
		moveVec.z = moveVec2d.y * tpSpeed;
		if (pressed) player->lerpMotion(moveVec);
	}
}

void TestModule::onDisable() {
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
	if (istpMode) {
		auto player = g_Data.getLocalPlayer(); {
			if (!player->onGround) {
				player->velocity.x = 0.f;
				player->velocity.z = 0.f;
			}
		}
	}
	if (test) {
		//g_Data.getLocalPlayer()->setGameModeType(0);
	}
	if (blink) {
		auto blinkMod = moduleMgr->getModule<Blink>();
		blinkMod->setEnabled(false);
	}
}