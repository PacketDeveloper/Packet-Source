#include "HiveFly.h"

#include "../../Module/ModuleManager.h"

HiveFly::HiveFly() : IModule(0, Category::MOVEMENT, "Turkey go falling off cliff wee!1!1!!") {
	registerBoolSetting("Strafe", &this->strafeMode, this->strafeMode);
	registerBoolSetting("Boost", &this->boostMode, this->boostMode);
	registerBoolSetting("V2", &this->V2, this->V2);
	registerFloatSetting("Value", &this->speed2, this->speed2, -0.02f, 0.f);
	registerFloatSetting("Speed", &this->speedA, this->speedA, 0.10f, 1.f);
}

HiveFly::~HiveFly() {
}

const char* HiveFly::getModuleName() {
	return "HiveFly";
}

void HiveFly::onEnable() {
	auto scaffoldMod = moduleMgr->getModule<Scaffold>();
	auto speedMod = moduleMgr->getModule<Speed>();
	auto player = g_Data.getLocalPlayer();
	scaffoldMod->setEnabled(false);
	speedMod->setEnabled(false);
	if (boostMode) {
		player->animateHurt();
		boostCounter = 1;
	}
	if (V2) {
		V2Counter = 1;
	}
}

void HiveFly::onTick(C_GameMode* gm) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	auto scaffoldMod = moduleMgr->getModule<Scaffold>();
	auto blinkMod = moduleMgr->getModule<Blink>();
	auto speedMod = moduleMgr->getModule<Speed>();
	auto player = g_Data.getLocalPlayer();
	scaffoldMod->setEnabled(false);
	speedMod->setEnabled(false);
	if (strafeMode) {
		auto kaMod = moduleMgr->getModule<Killaura>();
		kaMod->strafe = true;
	}
	if (!V2) {
		*g_Data.getClientInstance()->minecraft->timer = 20.f;
		if (boostMode) {
			if (boostCounter == 1e+300 * 1e+300) {
				boostCounter = 1;
			} else {
				boostCounter++;
			}
			if (boostCounter == 2) {
				speedA = 1.f;
			} else if (boostCounter == 4) {
				speedA = 0.75f;
			} else if (boostCounter == 7) {
				speedA = 0.60f;
			} else if (boostCounter == 10) {
				speedA = 0.55f;
			} else if (boostCounter == 13) {
				speedA = 0.50f;
			} else if (boostCounter == 16) {
				speedA = 0.48f;
			} else if (boostCounter == 20) {
				speedA = 0.45f;
			} else if (boostCounter == 35) {
				speedA = 0.32f;
			}
		}
		if (counter == 10) {
			counter = 1;
		} else {
			counter++;
		}
		float yaw = player->yaw;
		if (counter >= 0) {
			gm->player->velocity = vec3_t(0, 0, 0);
		}
		if (counter == 1) {
			player->velocity.y = speed2;
		}
	} else {
		if (V2Counter == 1000) {
			V2Counter = 1;
		} else {
			V2Counter++;
		}
			testTimer++;
			if (testTimer == 11) {
				blinkMod->setEnabled(false);
			}
			if (testTimer >= 12) {
				blinkMod->setEnabled(true);
				testTimer = 0;
			}
			if (testTimer <= 10 || testTimer >= 14) {
				blinkMod->setEnabled(true);
		}
		if (GameData::isKeyDown(*input->sneakKey)) {
			g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
		}
		if (velCounter == 10) {
			velCounter = 1;
		} else {
			velCounter++;
		}
		if (velCounter >= 0) {
			player->velocity.y = 0;
		}
		if (velCounter == 1) {
			player->velocity.y = -0.004993199370801449;
		}
		if (V2Counter != 2) {
			if (!GameData::isKeyDown(*input->spaceBarKey)) {
				localPlayer->onGround = true;
			}
		}
		/*if (GameData::isKeyDown(*input->spaceBarKey)) {
				vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;

				vec3_t pos;
				pos.x = 0.f + pPos.x;
				pos.y = 0.3f + pPos.y;
				pos.z = 0.f + pPos.z;

				g_Data.getLocalPlayer()->setPos(pos);
		}*/
		if (V2Counter == 2) {
			if (player->onGround)
				player->jumpFromGround();
		}
		if (V2Counter == 5) {
			*g_Data.getClientInstance()->minecraft->timer = 180.f;
		}
		if (V2Counter == 15) {
			*g_Data.getClientInstance()->minecraft->timer = 114.f;
		}
		if (V2Counter == 23) {
			*g_Data.getClientInstance()->minecraft->timer = 70.f;
		}
		if (V2Counter == 35) {
			*g_Data.getClientInstance()->minecraft->timer = 83.f;
		}
		if (V2Counter == 45) {
			*g_Data.getClientInstance()->minecraft->timer = 58.f;
		}
		if (V2Counter == 65) {
			*g_Data.getClientInstance()->minecraft->timer = 47.f;
		}
		if (V2Counter == 130) {
			*g_Data.getClientInstance()->minecraft->timer = 39.f;
		}
		if (V2Counter == 170) {
			*g_Data.getClientInstance()->minecraft->timer = 40.f;
		}
		if (V2Counter == 215) {
			*g_Data.getClientInstance()->minecraft->timer = 29.f;
		}
	}
}

void HiveFly::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	if (!V2) {
		if (boostMode) {
			vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
			bool pressed = moveVec2d.magnitude() > 0.01f;
			if (player->onGround && pressed)
				player->jumpFromGround();
		}
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
}

void HiveFly::onDisable() {
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
	auto player = g_Data.getLocalPlayer();
	if (!player->onGround) {
		player->velocity.x = 0.f;
		player->velocity.z = 0.f;
	}
	if (strafeMode) {
		auto killauraMod = moduleMgr->getModule<Killaura>();
		killauraMod->strafe = false;
	}
	if (V2) {
		auto blinkMod = moduleMgr->getModule<Blink>();
		blinkMod->setEnabled(false);
	}
}