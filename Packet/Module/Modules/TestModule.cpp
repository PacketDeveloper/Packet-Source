#include "../../Module/ModuleManager.h"
#include "TestModule.h"

TestModule::TestModule() : IModule(0, Category::MISC, "Description") {
	registerBoolSetting("AlertBox", &alertBox, alertBox);
	registerBoolSetting("Teleport", &istpMode, istpMode);
	//registerBoolSetting("Teleport2", &istpModeTwo, istpModeTwo);
	registerBoolSetting("Damage", &dmgMode, dmgMode);
	registerBoolSetting("Blink", &blink, blink);
	registerBoolSetting("Test", &test, test);
	registerBoolSetting("AutoDisable", &autoDisable, autoDisable);
	registerFloatSetting("SliderX", &sliderX, sliderX, -20, 20);
	registerFloatSetting("SliderY", &sliderY, sliderY, -20, 20);
	registerFloatSetting("SliderZ", &sliderZ, sliderZ, -20, 20);
	registerBoolSetting("Kow", &kowBool, kowBool);
	registerFloatSetting("KowFloat", &kowFloat, kowFloat, 0.2f, 10.f);
}

const char* TestModule::getModuleName() {
	return "TestModule";
}

// most of this code is old so thats why its messy

void TestModule::onEnable() {
	auto blinkMod = moduleMgr->getModule<Blink>();
	auto speed = moduleMgr->getModule<Speed>();
	auto player = g_Data.getLocalPlayer();
	if (g_Data.getLocalPlayer() == nullptr)
		setEnabled(false);
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
		box->closeTimer = 7;
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
		if (blink)
			testCounter = 0;

		if (kowBool) {
			kowBool2 = true;
			blinkMod->setEnabled(true);
			kowInt2 = 0;
			kowInt1 = 0;
			if (player->onGround)
			player->jumpFromGround();
		}
		if (test) {
			counter = 0;
		}
}

bool TestModule::isFlashMode() {
		return false;
}

void TestModule::onTick(C_GameMode* gm) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	if (input == nullptr) return;
	float calcYaw = (gm->player->yaw + 90) * (PI / 180);
	auto player = g_Data.getLocalPlayer();
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
		//auto player = g_Data.getLocalPlayer();
				//player->velocity.x = 6.f;
				//player->velocity.y = 7.f;
		//*g_Data.getClientInstance()->minecraft->timer = 20.f;
				//player->velocity.z = 5.f;
		//player->animateHurt();
		//player->setSleeping(true);
		//player->setSleeping(true);
		//player->updateWalkAnim();
		//player->getAttackAnim(1);
		//player->setSize(300, 0);
		 //if (player->isInWater()) {
			//gm->player->onGround = true;
			//gm->player->velocity.y = 1;
		//}
		/*auto flight = moduleMgr->getModule<Flight>();
		auto speed = moduleMgr->getModule<Speed>();
		if (counter == 7) {
			//flight->setEnabled(false);
			counter = 1;
		} else {
			counter++;
		}
		if (counter == 1) {
			flight->setEnabled(true);
		} else if (counter == 5) {
			flight->setEnabled(false);
		}*/
		//vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;

	//	vec3_t pos;
		//pos.x = 0.f + pPos.x;
		//pos.y = 300.f + pPos.y;
		//pos.z = 0.f + pPos.z;

		//g_Data.getLocalPlayer()->setPos(pos);
		bool disable = false;
		if (player->damageTime >= 1) {
			player->velocity.x = 0.1;
			player->velocity.z = 0.4;
			player->velocity.y = 0.8;
			disable = true;
		}
		if (disable) {
			setEnabled(false);
		}
	}
			if (istpMode) {
				auto player = g_Data.getLocalPlayer();
				C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
				float yaw = player->yaw;
				if (input->forwardKey && input->backKey && input->rightKey && input->leftKey) {
					gm->player->velocity = vec3_t(0, 0, 0);
				}
			}

			if (kowBool && kowBool2 && kowInt2 <= 3) {
				kowInt1++;
				if (kowInt1 >= 6 && kowInt1 <= 7) {
					blinkMod->setEnabled(false);
					*g_Data.getClientInstance()->minecraft->timer = 4;
				}
				if (kowInt1 >= 8) {
					kowInt1 = 0;
					blinkMod->setEnabled(true);
					kowInt2++;
					*g_Data.getClientInstance()->minecraft->timer = 20;
				}
			}
			if (kowInt2 >= 3) {
				blinkMod->setEnabled(false);
				kowBool2 = false;
				kowInt1 = 0;
				kowInt2 = 0;
				*g_Data.getClientInstance()->minecraft->timer = 20;
			}
			if (GameData::isKeyDown(*input->sneakKey) && kowBool)
				g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
			if (kowBool) {
				if (g_Data.canUseMoveKeys()) {
					if (GameData::isKeyDown(*input->spaceBarKey))
						glideModEffective += kowFloat;
					if (GameData::isKeyDown(*input->sneakKey))
						glideModEffective -= kowFloat;
				}

				float yaw = player->yaw;
				if (input->forwardKey && input->backKey && input->rightKey && input->leftKey)
					gm->player->velocity = vec3_t(0.f, -.0001f, -0.f);
				gm->player->velocity.y = glideModEffective-0.0001f;
				glideModEffective = glideMod;
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

	if (kowBool) {
		auto player = g_Data.getLocalPlayer();
		if (player == nullptr) return;

		vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
		bool pressed = moveVec2d.magnitude() > 0.01f;

		float calcYaw = (player->yaw + 90) * (PI / 180);
		vec3_t moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
		moveVec.x = moveVec2d.x * kowFloat;
		moveVec.y = player->velocity.y;
		moveVec.z = moveVec2d.y * kowFloat;
		if (pressed) player->lerpMotion(moveVec);
		if (g_Data.getLocalPlayer()->velocity.squaredxzlen() > 0.01) {
			C_MovePlayerPacket p = C_MovePlayerPacket(g_Data.getLocalPlayer(), player->getPos()->add(vec3_t(moveVec.x / 1.3f, 0.f, moveVec.z / 1.3f)));
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
			C_MovePlayerPacket p2 = C_MovePlayerPacket(g_Data.getLocalPlayer(), player->getPos()->add(vec3_t(player->velocity.x / 1.3f, 0.f, player->velocity.z / 2.3f)));
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p2);
		}
	}
}

void TestModule::onDisable() {
	auto blinkMod = moduleMgr->getModule<Blink>();
	auto flight = moduleMgr->getModule<Flight>();
	auto player = g_Data.getLocalPlayer();
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
	if (istpMode) {
			if (!player->onGround) {
				player->velocity.x = 0.f;
				player->velocity.z = 0.f;
			}
		}
	if (test) {
			auto speed = moduleMgr->getModule<Speed>();
			flight->setEnabled(false);
			speed->setEnabled(false);
		//g_Data.getLocalPlayer()->setGameModeType(0);
	}
	if (blink)
		blinkMod->setEnabled(false);
	if (kowBool) {
		player->velocity = vec3_t(0.f, .01f, 0.f);
		blinkMod->setEnabled(false);
	}
}

void TestModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	/*vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	auto player = g_Data.getLocalPlayer();
	auto raknet = g_Data.getRakNetInstance();
	if (GameData::canUseMoveKeys()) {
		if (!(g_Data.getLocalPlayer() == nullptr)) {
			if (raknet == nullptr) return;

			std::string servIp = raknet->serverIp.getText();
			std::string servPort = std::to_string(raknet->serverPort);

			vec2_t textPos = vec2_t(g_Data.getClientInstance()->getGuiData()->windowSize.y / 30.f + -16.f, g_Data.getClientInstance()->getGuiData()->windowSize.x - 12.1f);

			if (raknet->serverIp.getTextLength() < 1 || raknet->serverIp.getText() == nullptr)
				servIp = "localhost";
			if (raknet->serverIp.getText() == 0)
				servIp = "0.0.0.0";
			if (raknet->serverPort == 0)
				servPort = "0";
			if (servPort.c_str() == nullptr)
				servPort = "localhost";

			std::string serverInfo = "IP: " + servIp + " " + "Port: " + servPort;

			float x = windowSize.x / 30.f + -16.f;
			float y = windowSize.y - 12.1f;

			DrawUtils::drawText(vec2_t(x, y), &serverInfo, MC_Color(255, 255, 255), 1, 1, true);
		}
	}*/
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	auto player = g_Data.getLocalPlayer();
	if (test) {
		float x = windowSize.x / 114 + 1;
		float y = windowSize.y - 20;

		float x2 = windowSize.x / 114 + 1;
		float y2 = windowSize.y - 30;

		float x3 = windowSize.x / 114 + 1;
		float y3 = windowSize.y - 40;

		std::string fpsText = "Player Pitch: " + std::to_string(player->pitch);
		std::string fpsText2 = "Player Yaw: " + std::to_string(player->yaw);
		std::string fpsText3 = "Body Yaw: " + std::to_string(player->bodyYaw);
		DrawUtils::drawText(vec2_t(x, y), &fpsText, MC_Color(255, 255, 255), 1, 1, true);
		DrawUtils::drawText(vec2_t(x2, y2), &fpsText2, MC_Color(255, 255, 255), 1, 1, true);
		DrawUtils::drawText(vec2_t(x3, y3), &fpsText2, MC_Color(255, 255, 255), 1, 1, true);
	}
}
