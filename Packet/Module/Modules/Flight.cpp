#include "Flight.h"

#include "../../Module/ModuleManager.h"

Flight::Flight() : IModule(0, Category::MOVEMENT, "Allows you to fly") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Vanilla", 0);
	mode.addEntry("Airwalk", 1);
	mode.addEntry("Teleport", 2);
	mode.addEntry("Jetpack", 3);
	mode.addEntry("BlockFly", 4);
	mode.addEntry("Jump", 5);
	registerBoolSetting("Damage", &damage, damage);
	registerBoolSetting("Boost", &boost, boost);
	registerFloatSetting("Speed", &speed, speed, 0.2f, 4.f);
	registerFloatSetting("value", &value, value, -0.15f, 0.00);
	//registerIntSetting("HiveMS", &timing, timing, 0, 10);
}

Flight::~Flight() {
	if (mode.getSelectedValue() == 6 || mode.getSelectedValue() == 7) {
		getMovePlayerPacketHolder()->clear();
		getPlayerAuthInputPacketHolder()->clear();
	}
}

const char* Flight::getRawModuleName() {
	auto player = g_Data.getLocalPlayer();
	player->jumpFromGround();
	return "Flight";
}

const char* Flight::getModuleName() {
	if (mode.getSelectedValue() == 0) {
		name = std::string("Flight ") + std::string(GRAY) + std::string("Vanilla");
		return name.c_str();
	} else if (mode.getSelectedValue() == 1) {
		name = std::string("Flight ") + std::string(GRAY) + std::string("Airwalk");
		return name.c_str();
	} else if (mode.getSelectedValue() == 2) {
		name = std::string("Flight ") + std::string(GRAY) + std::string("Teleport");
		return name.c_str();
	} else if (mode.getSelectedValue() == 3) {
		name = std::string("Flight ") + std::string(GRAY) + std::string("Jetpack");
		return name.c_str();
	} else if (mode.getSelectedValue() == 4) {
		name = std::string("Flight ") + std::string(GRAY) + std::string("BlockFly");
		return name.c_str();
	} else if (mode.getSelectedValue() == 5) {
		name = std::string("Flight ") + std::string(GRAY) + std::string("Jump");
		return name.c_str();
	} else if (mode.getSelectedValue() == 6 || mode.getSelectedValue() == 7) {
		name = std::string("Flight ") + std::string(GRAY) + std::string("Hive");
		return name.c_str();
	}
}

void Flight::onEnable() {
	auto player = g_Data.getLocalPlayer();
	auto speed = moduleMgr->getModule<Speed>();
	if (speed->isEnabled()) speedWasEnabled = true;
	if (damage && mode.getSelectedValue() != 7) {
		if (g_Data.getLocalPlayer() == nullptr)
			return;
		auto player = g_Data.getLocalPlayer();
		player->animateHurt();
	}
	if (mode.getSelectedValue() == 2) {  // BlockFly
		placeCounter = 1;
	}
	if (mode.getSelectedValue() == 5) {
		if (g_Data.getLocalPlayer() == nullptr)
			return;
		vec3_t* pos = g_Data.getLocalPlayer()->getPos();
		jumpHeight = floorf(pos->y);
	}
	if (mode.getSelectedValue() == 6) { // Hive
		prevSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
		auto player = g_Data.getLocalPlayer();
		throwPearl = true;
	}
	tick = 0;
}

bool Flight::isFlashMode() {
	return mode.getSelectedValue() == 3;
}

void Flight::onTick(C_GameMode* gm) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	auto player = g_Data.getLocalPlayer();
	float calcYaw = (gm->player->yaw + 90) * (PI / 180);
	float calcYawInvert = (gm->player->yaw - 90) * (PI / 180);

	auto longjump = moduleMgr->getModule<LongJump>();
	auto speedMod = moduleMgr->getModule<Speed>();
	if (longjump->isEnabled()) {
		auto notification = g_Data.addInfoBox("Flight:", "Disabled LongJump to prevent flags/errors");
		notification->closeTimer = 20;
		longjump->setEnabled(false);
	}
	if (speedMod->isEnabled()) {
		auto notification = g_Data.addInfoBox("Flight:", "Disabled Speed to prevent flags/errors");
		notification->closeTimer = 20;
		speedMod->setEnabled(false);
	}

	if (input == nullptr && mode.getSelectedValue() != 5) return;
	g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;

	if (mode.getSelectedValue() == 0) {  // Vanilla
		if (g_Data.canUseMoveKeys()) {
			if (GameData::isKeyDown(*input->spaceBarKey))
				effectiveValue += speed;
			if (GameData::isKeyDown(*input->sneakKey))
				effectiveValue -= speed;
		}
		float yaw = player->yaw;
		if (input->forwardKey && input->backKey && input->rightKey && input->leftKey) {
			gm->player->velocity = vec3_t(0, 0, 0);
		}
		gm->player->velocity.y = effectiveValue;
		effectiveValue = value;
	} else if (mode.getSelectedValue() == 1) {  // Airwalk
		if (speedMod->isEnabled() || GameData::isKeyDown(*input->spaceBarKey)) {
			player->onGround = false;
		} else {
			player->onGround = true;
		}
		gm->player->velocity.y = effectiveValue;
		effectiveValue = value;
	} else if (mode.getSelectedValue() == 2) {  // Teleport
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
	} else if (mode.getSelectedValue() == 3) {  // Jetpack
		float calcYaw = (gm->player->yaw + 90) * (PI / 180);
		float calcPitch = (gm->player->pitch) * -(PI / 180);
		vec3_t moveVec;
		moveVec.x = cos(calcYaw) * cos(calcPitch) * speed;
		moveVec.y = sin(calcPitch) * speed;
		moveVec.z = sin(calcYaw) * cos(calcPitch) * speed;

		gm->player->lerpMotion(moveVec);
	} else if (mode.getSelectedValue() == 4) {  // BlockFly
		static auto clickGUI = moduleMgr->getModule<ClickGuiMod>();
		if (!player->onGround) {
			auto box = g_Data.addInfoBox("Flight:", "You must be on the ground");
			box->closeTimer = 10;
			this->setEnabled(false);
		}
		gm->player->velocity.y = effectiveValue;
		static bool restored = false;
		if (clickGUI->isEnabled()) {
			auto box = g_Data.addInfoBox("Flight:", "Disabled to prevent flags/errors");
			box->closeTimer = 10;
			setEnabled(false);
		}
		if (placeCounter == placeDelay) { // default: 8
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
			if (g_Data.getLocalPlayer() == nullptr)
				return;
			if (!g_Data.canUseMoveKeys())
				return;

			auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();
			if ((selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr || !selectedItem->getItem()->isBlock()))
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
	} else if (mode.getSelectedValue() == 5) {  // Jump
		if (!g_Data.canUseMoveKeys()) {
			auto notification = g_Data.addInfoBox("Flight:", "Disabled to prevent errors");
			notification->closeTimer = 20;
			setEnabled(false);
		}
		vec3_t* pos = g_Data.getLocalPlayer()->getPos();
		height = (float)floorf(pos->y);

		tick++;
		if (tick >= 8) {
			if (GameData::isKeyDown(*input->spaceBarKey)) {
				jumpHeight == height;
				jumpHeight += 1.f;
			}
			if (GameData::isKeyDown(*input->sneakKey)) {
				jumpHeight == height;
				jumpHeight -= 1.f;
			}
			tick = 0;
		}
		//if (player->onGround) jumpHeight == height;
		if (player->fallDistance >= 5) {
			clientMessageF("fell");
			jumpHeight == height;
		}
	} else if (mode.getSelectedValue() == 6) {  // Hive
	}
}

void Flight::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.01f;

	if (boost && mode.getSelectedValue() != 1) {
		auto player = g_Data.getLocalPlayer();
		if (player == nullptr) return;

		if (player->onGround && pressed)
			player->jumpFromGround();
	}

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
	} else if (mode.getSelectedValue() == 2) {  // Teleport
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
	} else if (mode.getSelectedValue() == 4) {  // BlockFly
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
	} else if (mode.getSelectedValue() == 5) {  // Jump
		auto player = g_Data.getLocalPlayer();
		vec3_t* pos = g_Data.getLocalPlayer()->getPos();
		height = (float)floorf(pos->y);

		vec2_t movement = {input->forwardMovement, -input->sideMovement};
		bool pressed = movement.magnitude() > 0.f;
		float calcYaw = (player->yaw + 90) * (PI / 180);
		vec3_t moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		if (player->onGround && pressed) player->jumpFromGround();
		movement = {movement.x * c - movement.y * s, movement.x * s + movement.y * c};
		moveVec.x = movement.x * speed;
		moveVec.y = player->velocity.y;
		moveVec.z = movement.y * speed;
		if (pressed)
			player->lerpMotion(moveVec);
		if (height != jumpHeight)
			return;
		if (height == jumpHeight) {
			//player->velocity.x *= 0;
			player->velocity.y *= -0.01 / 1;
			//player->velocity.z *= 0;
			player->jumpFromGround();

			if (g_Data.getLocalPlayer()->velocity.squaredxzlen() > 0.01) {
				float calcYaw2 = (player->yaw - 90) * (PI / 180);
				vec3_t moveVec;
				float c2 = cos(calcYaw2);
				float s2 = sin(calcYaw2);
				C_MovePlayerPacket p = C_MovePlayerPacket(g_Data.getLocalPlayer(), player->getPos()->add(vec3_t(c2 * .1f, 0.f, s2 * .1f)));
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
				C_MovePlayerPacket p2 = C_MovePlayerPacket(g_Data.getLocalPlayer(), player->getPos()->add(vec3_t(player->velocity.x / 1.3f, 0.f, player->velocity.z / 2.3f)));
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p2);
			}
		}
	} else if (mode.getSelectedValue() == 6) {  // Hive
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
		C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (mode.getSelectedValue() == 4) { // BlockFly - Blocks
			if (block != nullptr && block->toLegacy()->material->isSolid) {
				inv->selectedHotbarSlot = i;
				return true;
			}
		} else if (mode.getSelectedValue() == 6) { // Hive - Ender Pearl
			if (throwPearl && (*itemStack->item)->itemId == 422) {
				inv->selectedHotbarSlot = i;
				return true;
			}
		}
	}
	return false;
}

void Flight::onSendPacket(C_Packet* packet) {
	auto player = g_Data.getLocalPlayer();
	if (mode.getSelectedValue() == 6) {  // Hive TNT
		if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*g_Data.getLocalPlayer()->getPos());
			float myPitchq = player->pitch;
			float myYawq = player->yaw;
			float bodyYawq = player->bodyYaw;
			movePacket->pitch = myPitchq;
			movePacket->headYaw = myYawq;
		}
	}
}


void Flight::onDisable() {
	*g_Data.getClientInstance()->minecraft->timer = 20;
	auto player = g_Data.getLocalPlayer();
	auto scaffold = moduleMgr->getModule<Scaffold>();
	auto speed = moduleMgr->getModule<Speed>();
	if (speedWasEnabled == true) {
		speed->setEnabled(true);
		speedWasEnabled = false;
	}
	if (g_Data.getLocalPlayer() == nullptr || !g_Data.canUseMoveKeys())
		return;

	if (mode.getSelectedValue() == 5 || mode.getSelectedValue() == 6 || mode.getSelectedValue() == 1) { // Hive and Airwalk
		player->onGround = false;
	}
	if (mode.getSelectedValue() == 1)
		return;
	g_Data.getLocalPlayer()->velocity = vec3_t(0, 0, 0);
	if (mode.getSelectedValue() == 4 || mode.getSelectedValue() == 6) {  // BlockFly / Hive
		__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
		g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prevSlot;
	}
}