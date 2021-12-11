#include "Glide.h"

#include "../../Module/ModuleManager.h"

Glide::Glide() : IModule(0, Category::MOVEMENT, "FlightModule") {
	this->registerBoolSetting("Damage", &this->dmgMode, this->dmgMode);
	this->registerBoolSetting("Vanilla", &this->vanillaMode, this->vanillaMode);
	this->registerBoolSetting("Boost", &this->boostMode, this->boostMode);
	this->registerBoolSetting("Hive", &this->hiveBlink, this->hiveBlink);
	this->registerBoolSetting("HiveOld", &this->hiveMode, this->hiveMode);
	registerFloatSetting("Speed", &this->speed, this->speed, 0.7f, 4.f);
	this->registerFloatSetting("value", &this->glideMod, this->glideMod, -0.15f, 0.00);
}

Glide::~Glide() {
}

const char* Glide::getModuleName() {
	if (isEnabled()) {
		static char modName[30];  //Hello i go die now WeEEEEEEEEEE!12111!!!!!!!!1111
		sprintf_s(modName, 30, "Flight", glideModEffective);
		return modName;
	} else
		return ("Flight");
}

const char* Glide::getRawModuleName() {
	return "Flight";
}

void Glide::onEnable() {
	if (hiveBlink) {
		auto player = g_Data.getLocalPlayer();
		player->animateHurt();
		counter = 1;
	}
	if (hiveMode) {
		if (counter <= 500) {
			counter = 0;
		}
		disable = true;
	}
	if (hiveMode) {
		*g_Data.getClientInstance()->minecraft->timer = 2.f;
	}
	auto player = g_Data.getLocalPlayer();
	if (hiveMode) {
		auto bhopMod = moduleMgr->getModule<Bhop>();
		vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;

		vec3_t pos;
		pos.x = 0.f + pPos.x;
		pos.y = 6.3f + pPos.y;
		pos.z = 0.f + pPos.z;

		g_Data.getLocalPlayer()->setPos(pos);
		if (bhopMod->isEnabled()) {
			bhopMod->setEnabled(false);
		}
	}
	if (dmgMode) {
		auto player = g_Data.getLocalPlayer();
		player->animateHurt();
	}
}

bool Glide::isFlashMode() {
	if (jetpackMode) {
		return true;
	}
	if (!jetpackMode) {
		return false;
	}
}

void Glide::onTick(C_GameMode* gm) {
	if (hiveBlink) {
		if (counter == 1000) {
			//g_Data.getGuiData()->displayClientMessageF("[R] Reset counter");
			counter = 1;
		} else {
			counter++;
		}
	}
	if (hiveBlink) {
		testTimer++;
		if (testTimer == 11) {
			auto blinkMod = moduleMgr->getModule<Blink>();
			blinkMod->setEnabled(false);
		}
		if (testTimer >= 12) {
			auto blinkMod = moduleMgr->getModule<Blink>();
			blinkMod->setEnabled(true);
			testTimer = 0;
		}
		if (testTimer <= 10 || testTimer >= 12) {
			auto blinkMod = moduleMgr->getModule<Blink>();
			blinkMod->setEnabled(true);
		}
	}
	if (hiveBlink) {
		C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
		if (!GameData::isKeyDown(*input->spaceBarKey && GameData::isKeyDown(*input->sneakKey))) {
			C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
			localPlayer->velocity.y = -0.00003f;
			if (counter == 65) {
				localPlayer->velocity.y = -0.00045f;
				g_Data.getGuiData()->displayClientMessageF("[65] Changed Velocity");
			}
			if (counter == 145) {
				localPlayer->velocity.y = -0.00070f;
				g_Data.getGuiData()->displayClientMessageF("[145] Changed Velocity");
			}
			if (counter == 184) {
				g_Data.getGuiData()->displayClientMessageF("[184] Changed Velocity");
				localPlayer->velocity.y = -0.00032f;
			}
			if (counter == 200) {
				g_Data.getGuiData()->displayClientMessageF("[200] Changed Velocity");
				localPlayer->velocity.y = -0.00013f;
				//*g_Data.getClientInstance()->minecraft->timer = 40.f;
				//this->setEnabled(false);
			}
			if (!GameData::isKeyDown(*input->spaceBarKey)) {
				localPlayer->onGround = true;
			}
		}
	}
	if (hiveBlink) { // Boost Mode Slowdown
		if (counter == 5) {
			//g_Data.getGuiData()->displayClientMessageF("[5] Timer = 66");
			*g_Data.getClientInstance()->minecraft->timer = 66.f;
		}
		if (counter == 23) {
			//g_Data.getGuiData()->displayClientMessageF("[23] Timer = 57");
			*g_Data.getClientInstance()->minecraft->timer = 57.f;
		}
		if (counter == 35) {
			//g_Data.getGuiData()->displayClientMessageF("[35] Timer = 43");
			*g_Data.getClientInstance()->minecraft->timer = 43.f;
		}
		if (counter == 45) {
			//g_Data.getGuiData()->displayClientMessageF("[45] Timer = 40");
			*g_Data.getClientInstance()->minecraft->timer = 40.f;
		}
		if (counter == 65) {
			//g_Data.getGuiData()->displayClientMessageF("[65] Timer = 36");
			*g_Data.getClientInstance()->minecraft->timer = 36.f;
		}
		if (counter == 130) {
			//g_Data.getGuiData()->displayClientMessageF("[150] Timer = 34");
			*g_Data.getClientInstance()->minecraft->timer = 32.f;
		}
		if (counter == 152) {
			//g_Data.getGuiData()->displayClientMessageF("[152] Timer = 27");
			*g_Data.getClientInstance()->minecraft->timer = 27.f;
		}
	}
	if (hiveBlink) {
		auto scaffoldMod = moduleMgr->getModule<Scaffold>();
		auto bhopMod = moduleMgr->getModule<Bhop>();
		scaffoldMod->setEnabled(false);
		bhopMod->setEnabled(false);
	}

if (!hiveBlink) {
		if (hiveMode) {
			auto player = g_Data.getLocalPlayer();
			if (counter >= 500) {
				counter = 499;
			}
			if (player->onGround) {
				counter = 0;
			} else if (!player->onGround) {
				counter++;
			}
		}
		C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
		if (hiveMode && GameData::isKeyDown(*input->sneakKey)) {
			g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
		}
		if (!hiveMode) {
			*g_Data.getClientInstance()->minecraft->timer = 20.f;
		}
		if (hiveMode) {
			auto breakerMod = moduleMgr->getModule<Fucker>();
			auto stepMod = moduleMgr->getModule<Step>();
			auto player = g_Data.getLocalPlayer();
			*g_Data.getClientInstance()->minecraft->timer = static_cast<float>(this->timer);
			glideMod = -0.11;
			boostMode = false;
			vanillaMode = false;
			strafeMode = false;
			stepMod->height = 0.5625f;
			if (breakerMod->isEnabled()) {
				breakerMod->setEnabled(false);
			}
		}
		if (hiveMode) {
			auto player = g_Data.getLocalPlayer();
			if (!player->onGround && counter <= 100) {
				disable = false;
			}
		}
		if (hiveMode) {
			auto hivefly = moduleMgr->getModule<Glide>();
			auto player = g_Data.getLocalPlayer();
			if (player->onGround && disable == false) {
				hivefly->setEnabled(false);
				player->velocity.x = 0.f;
				player->velocity.z = 0.f;
			}
		}

		auto scaffoldMod = moduleMgr->getModule<Scaffold>();
		if (scaffoldMod->isEnabled()) {
			scaffoldMod->setEnabled(false);
		}
		if (onkeydisableMode) {
			auto targetstrafeMod = moduleMgr->getModule<TargetStrafe>();
			{
				if (targetstrafeMod->spacekeyMode = true)
					;
				targetstrafeMod->spacekeyMode = false;
			}
		}
		if (!onkeydisableMode) {
			auto targetstrafeMod = moduleMgr->getModule<TargetStrafe>();
			{
				targetstrafeMod->spacekeyMode = true;
			}
		}
		if (boostMode) {
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
		glideModEffective = glideMod;
		if (boostMode && GameData::isKeyDown(*input->sneakKey)) {
			g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
		}

		if (g_Data.canUseMoveKeys()) {
			if (GameData::isKeyDown(*input->spaceBarKey))
				glideModEffective += 0.0f;
			if (GameData::isKeyDown(*input->sneakKey))
				glideModEffective -= 0.0f;
		}
		if (vanillaMode && GameData::isKeyDown(*input->sneakKey)) {
			if (!strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "mco.mineplex.com") == 0) {  //5 Mineplex
				g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
			}
		}
		if (vanillaMode) {
			auto player = g_Data.getLocalPlayer();
			C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
			float yaw = player->yaw;
			if (input->forwardKey && input->backKey && input->rightKey && input->leftKey) {
				gm->player->velocity = vec3_t(0, 0, 0);
			}
			if (!strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "mco.mineplex.com") == 0) {  //5 Mineplex
				if (GameData::isKeyDown(*input->spaceBarKey))
					glideModEffective += speed;
				if (GameData::isKeyDown(*input->sneakKey))
					glideModEffective -= speed;
			}
		}
		gm->player->velocity.y = glideModEffective;
		auto bhopMod = moduleMgr->getModule<Bhop>();
		if (bhopMod->isEnabled()) {
			bhopMod->setEnabled(false);
		}
	}
}

void Glide::onMove(C_MoveInputHandler* input) {
	if (hiveBlink) {
		auto player = g_Data.getLocalPlayer();
		if (input->isJumping) {
			//*g_Data.getClientInstance()->minecraft->timer = 50.f;
			//player->velocity.y -= 0.000000000001f;
			player->fallDistance = -0.1f;
			vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;

			vec3_t pos;
			pos.x = 0.f + pPos.x;
			pos.y = 0.4f + pPos.y;
			pos.z = 0.f + pPos.z;

			g_Data.getLocalPlayer()->setPos(pos);
		}
	}

	if (!hiveBlink) {
		auto player = g_Data.getLocalPlayer();
		if (hiveMode) {
			vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
		}
		if (boostMode) {
			auto player = g_Data.getLocalPlayer();
			if (player == nullptr) return;

			if (player->isSneaking())
				return;

			vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
			bool pressed = moveVec2d.magnitude() > 0.01f;

			if (player->onGround && pressed)
				player->jumpFromGround();

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
		if (vanillaMode) {
			auto player = g_Data.getLocalPlayer();
			if (player == nullptr) return;

			vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
			bool pressed = moveVec2d.magnitude() > 0.01f;

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
}

void Glide::onDisable() {
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
	auto player = g_Data.getLocalPlayer();
	player->velocity.x = 0.f;
	player->velocity.y = 0.f;
	player->velocity.z = 0.f;
	if (hiveMode) {
		auto stepMod = moduleMgr->getModule<Step>();
		stepMod->height = 1.5f;
		*g_Data.getClientInstance()->minecraft->timer = 20.f;
		auto box = g_Data.addInfoBox("Flight", "Do not fly for 2-3s");
		box->closeTimer = 16;
	}
	auto targetstrafeMod = moduleMgr->getModule<TargetStrafe>(); {
		targetstrafeMod->packetMode = false;
		targetstrafeMod->spacekeyMode = true;
	}
	if (hiveBlink) {
		auto blinkMod = moduleMgr->getModule<Blink>();
		blinkMod->setEnabled(false);
	}
}