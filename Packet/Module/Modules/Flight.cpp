#include "Flight.h"

#include "../../Module/ModuleManager.h"

Flight::Flight() : IModule(0, Category::MOVEMENT, "yes") {
	registerBoolSetting("Damage", &damage, damage);
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Vanilla", 0);
	mode.addEntry("Boost", 1);
	mode.addEntry("BlockFly", 2);
	mode.addEntry("Teleport", 3);
	mode.addEntry("Jetpack", 4);
	mode.addEntry("Hive", 5);
	//registerIntSetting("PlaceDelay", &placeDelay, placeDelay, 2, 20);
	registerFloatSetting("Speed", &speed, speed, 0.3f, 4.f);
	registerFloatSetting("value", &glideMod, glideMod, -0.15f, 0.00);
}

Flight::~Flight() {
	if (mode.getSelectedValue() == 5) {
		getMovePlayerPacketHolder()->clear();
		getPlayerAuthInputPacketHolder()->clear();
	}
}

const char* Flight::getRawModuleName() {
	return "Flight";
}

const char* Flight::getModuleName() {
	if (mode.getSelectedValue() == 0) {
		name = std::string("Flight ") + std::string(GRAY) + std::string("Vanilla");
		return name.c_str();
	} else if (mode.getSelectedValue() == 1) {
		name = std::string("Flight ") + std::string(GRAY) + std::string("Boost");
		return name.c_str();
	} else if (mode.getSelectedValue() == 2) {
		name = std::string("Flight ") + std::string(GRAY) + std::string("BlockFly");
		return name.c_str();
	} else if (mode.getSelectedValue() == 3) {
		name = std::string("Flight ") + std::string(GRAY) + std::string("TP");
		return name.c_str();
	} else if (mode.getSelectedValue() == 4) {
		name = std::string("Flight ") + std::string(GRAY) + std::string("Jetpack");
		return name.c_str();
	} else if (mode.getSelectedValue() == 5) {
		name = std::string("Flight ") + std::string(GRAY) + std::string("Hive");
		return name.c_str();
	}
}

void Flight::onEnable() {
	auto player = g_Data.getLocalPlayer();
	auto scaffold = moduleMgr->getModule<Scaffold>();
	auto speed = moduleMgr->getModule<Speed>();
	if (speed->isEnabled())
		speedWasEnabled = true;
	if (scaffold->isEnabled())
		scfWasEnabled = true;
	if (damage) {
		auto player = g_Data.getLocalPlayer();
		player->animateHurt();
	}
	if (mode.getSelectedValue() == 2) {  // BlockFly
		placeCounter = 1;
		if (g_Data.getLocalPlayer() == nullptr)
			return;
		prevSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
		vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;
		vec3_t pos;
		pos.x = 0.f + pPos.x;
		pos.y = 0.1f + pPos.y;
		pos.z = 0.f + pPos.z;
		g_Data.getLocalPlayer()->setPos(pos);
	}
	if (mode.getSelectedValue() == 5) {
		//auto freeTP = moduleMgr->getModule<FreeTP>();
		auto speed = moduleMgr->getModule<Speed>();
		speed->setEnabled(true);
		//freeTP->setEnabled(true);
		hiveC = 1;
	}
}

bool Flight::isFlashMode() {
	return mode.getSelectedValue() == 4;
}

void Flight::onTick(C_GameMode* gm) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	if (input == nullptr) return;
	auto blinkMod = moduleMgr->getModule<Blink>();
	float calcYaw = (gm->player->yaw + 90) * (PI / 180);
	float calcYawInvert = (gm->player->yaw - 90) * (PI / 180);
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	auto scaffoldMod = moduleMgr->getModule<Scaffold>();
	auto longjump = moduleMgr->getModule<LongJump>();
	auto speedMod = moduleMgr->getModule<Speed>();
	auto player = g_Data.getLocalPlayer();
	scaffoldMod->setEnabled(false);
	longjump->setEnabled(false);
	speedMod->setEnabled(false);
	if (mode.getSelectedValue() == 3) {  // Teleport
		vec3_t pos = *g_Data.getLocalPlayer()->getPos();
		gm->player->velocity = vec3_t(0, 0, 0);
		if (input == nullptr) return;
		float x = 0.f;
		float z = 0.f;

		if (g_Data.canUseMoveKeys()) {
			if (GameData::isKeyDown(*input->spaceBarKey)) {
				player->setPos(pos.add(vec3_t(x, speed, z)));
			}
			if (GameData::isKeyDown(*input->sneakKey)) {
				player->setPos(pos.add(vec3_t(x, -speed, z)));
			}
		}
	}

	if (GameData::isKeyDown(*input->sneakKey) && (mode.getSelectedValue() == 0 || mode.getSelectedValue() == 1 || mode.getSelectedValue() == 2 || mode.getSelectedValue() == 3))
		g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;

	if (mode.getSelectedValue() == 1) {  // Boost
		float yaw = player->yaw;
		if (input->forwardKey && input->backKey && input->rightKey && input->leftKey)
			gm->player->velocity = vec3_t(0, 0, 0);
		gm->player->velocity.y = glideModEffective;
		glideModEffective = glideMod;
	}

	if (mode.getSelectedValue() == 0) {  // Vanilla
		if (g_Data.canUseMoveKeys()) {
			if (GameData::isKeyDown(*input->spaceBarKey))
				glideModEffective += speed;
			if (GameData::isKeyDown(*input->sneakKey))
				glideModEffective -= speed;
		}
	}
	if (mode.getSelectedValue() == 0) {  // Vanilla
		float yaw = player->yaw;
		if (input->forwardKey && input->backKey && input->rightKey && input->leftKey) {
			gm->player->velocity = vec3_t(0, 0, 0);
		}
		gm->player->velocity.y = glideModEffective;
		glideModEffective = glideMod;
	} else if (mode.getSelectedValue() == 4) {  // Jetpack
		float calcYaw = (gm->player->yaw + 90) * (PI / 180);
		float calcPitch = (gm->player->pitch) * -(PI / 180);
		vec3_t moveVec;
		moveVec.x = cos(calcYaw) * cos(calcPitch) * speed;
		moveVec.y = sin(calcPitch) * speed;
		moveVec.z = sin(calcYaw) * cos(calcPitch) * speed;

		gm->player->lerpMotion(moveVec);
	} else if (mode.getSelectedValue() == 2) {  // BlockFly
		static auto clickGUI = moduleMgr->getModule<ClickGuiMod>();
		glideMod = -0.00034065544605255127;
		if (!player->onGround) {
			auto box = g_Data.addInfoBox("Flight: You must be on the ground");
			box->closeTimer = 10;
			this->setEnabled(false);
		}
		gm->player->velocity.y = glideModEffective;
		static bool restored = false;
		if (clickGUI->isEnabled()) {
			auto box = g_Data.addInfoBox("Flight: Disabled to prevent flags/errors");
			box->closeTimer = 10;
			setEnabled(false);
		}
		if (placeCounter == placeDelay /*8*/) {
			placeCounter = 1;
		} else {
			placeCounter++;
		}
		if (placeCounter == 3)
			*g_Data.getClientInstance()->minecraft->timer = 20.f;
		if (selectBlock() == false) {
			if (!restored) {
				restored = true;
				g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prevSlot;
				if (restored == true)
					setEnabled(false);
			}
			return;
		}
		restored = false;
		auto player = g_Data.getLocalPlayer();
		float yaw = player->yaw;
		if (input->forwardKey && input->backKey && input->rightKey && input->leftKey) {
			gm->player->velocity = vec3_t(0, 0, 0);
		}
		if (placeCounter == 2) {
			*g_Data.getClientInstance()->minecraft->timer = 16;
			g_Data.getLocalPlayer()->velocity = vec3_t(0, 0, 0);
			if (speed > 0.05f) {
			}
			if (g_Data.getLocalPlayer() == nullptr)
				return;
			if (!g_Data.canUseMoveKeys())
				return;

			auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();
			if ((selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr || !selectedItem->getItem()->isBlock()))  // Block in hand?
				return;

			// Adjustment by velocity
			float speed = g_Data.getLocalPlayer()->velocity.magnitudexz();
			vec3_t vel = g_Data.getLocalPlayer()->velocity;
			vel = vel.normalize();                                 // Only use values from 0 - 1
			vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block below the player
			blockBelow.y -= g_Data.getLocalPlayer()->height;
			blockBelow.y -= 0.5f;

			if (!buildBlock(blockBelow)) {
				if (speed > 0.05f) {  // Are we actually walking?
					blockBelow.z -= vel.z * 0.4f;
					if (!buildBlock(blockBelow)) {
						blockBelow.x -= vel.x * 0.4f;
						if (!buildBlock(blockBelow) && g_Data.getLocalPlayer()->isSprinting()) {
							blockBelow.z += vel.z;
							blockBelow.x += vel.x;
							buildBlock(blockBelow);
						}
					}
				}
			}
		}
	} else if (mode.getSelectedValue() == 5) {  // Hive
		gm->player->velocity = vec3_t(0, 0, 0);
		if (input == nullptr) return;
		if (hiveC == 10) {
			hiveC = 1;
		} else {
			hiveC++;
		}
		if (hiveC == 5) {
			blink = true;
		} else {
			blink = false;
		}
	}
}

void Flight::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.01f;
	if (mode.getSelectedValue() == 0) {  // Vanilla
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
	} else if (mode.getSelectedValue() == 1) {  // Boost
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
	} else if (mode.getSelectedValue() == 2) {  // BlockFly
		if (placeCounter != 2) {
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
	} else if (mode.getSelectedValue() == 3) {  // Teleport
		vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
		bool pressed = moveVec2d.magnitude() > 0.01f;
		vec3_t pos = *g_Data.getLocalPlayer()->getPos();

		float calcYaw = (player->yaw + 90) * (PI / 180);
		vec3_t moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
		moveVec.x = moveVec2d.x * speed;
		moveVec.y = -0.0f;
		moveVec.z = moveVec2d.y * speed;
		player->setPos(pos.add(vec3_t(moveVec.x, moveVec.y, moveVec.z)));
	} else if (mode.getSelectedValue() == 5) { // Hive
		if (player->onGround && pressed)
			player->jumpFromGround();
		vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
		bool pressed = moveVec2d.magnitude() > 0.01f;
		vec3_t pos = *g_Data.getLocalPlayer()->getPos();

		float calcYaw = (player->yaw + 90) * (PI / 180);
		vec3_t moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
		moveVec.x = moveVec2d.x * speed;
		moveVec.y = -0.0f;
		moveVec.z = moveVec2d.y * speed;
		if (hiveC == 5) {
			player->setPos(pos.add(vec3_t(moveVec.x, moveVec.y, moveVec.z)));
		}
	}
}

bool Flight::buildBlock(vec3_t blockBelow) {
	blockBelow = blockBelow.floor();
	int i = 0;

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blockBelow));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blockBelow);

		g_Data.getCGameMode()->buildBlock(&blok, i);
		return true;
	}
	return false;
}

bool Flight::selectBlock() {
	C_PlayerInventoryProxy* inv = g_Data.getLocalPlayer()->getSupplies();
	for (int i = 0; i < 9; i++) {
		C_Block* block = *(C_Block**)(((char*)&inv->inventory->getItemStack(i)->tag) + 8);
		if (block != nullptr && block->toLegacy()->material->isSolid) {

			inv->selectedHotbarSlot = i;
			return true;
		}
	}
	return false;
}

void Flight::onSendPacket(C_Packet* packet) {
	auto player = g_Data.getLocalPlayer();
	if (mode.getSelectedValue() == 6) {  // Hive
		if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*g_Data.getLocalPlayer()->getPos());
			float myPitchq = player->pitch;
			float myYawq = player->yaw;
			float bodyYawq = player->bodyYaw;
			movePacket->pitch = myPitchq;
			movePacket->headYaw = myYawq;
			//movePacket->yaw = bodyYawq;
		}
	}
}


void Flight::onDisable() {
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
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
	if (mode.getSelectedValue() != 2 || mode.getSelectedValue() != 5 || mode.getSelectedValue() != 6) {
		g_Data.getLocalPlayer()->velocity = vec3_t(0, 0, 0);
	}
	if (mode.getSelectedValue() == 2) {  // BlockFly
		if (g_Data.getLocalPlayer() == nullptr)
			return;
		__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
		g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prevSlot;
	}
	if (mode.getSelectedValue() == 6) { // Hive
		auto freeTP = moduleMgr->getModule<FreeTP>();
		auto freecam = moduleMgr->getModule<Freecam>();
		*g_Data.getClientInstance()->minecraft->timer = 20.f;
		blink = false;
		freeze = false;
		blink2 = false;
		auto speed = moduleMgr->getModule<Speed>();
		speed->setEnabled(false);
		//freeTP->setEnabled(false);
		//freecam->setEnabled(false);
	}
}