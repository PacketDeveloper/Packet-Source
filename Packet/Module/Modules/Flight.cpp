#include "Flight.h"

#include "../../Module/ModuleManager.h"

Flight::Flight() : IModule(0, Category::MOVEMENT, "You're dumb for not knowing lol") {
	registerBoolSetting("Damage", &damage, damage);
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Vanilla", 0);
	mode.addEntry("Boost", 1);
	mode.addEntry("BlockFly", 2);
	mode.addEntry("Teleport", 3);
	mode.addEntry("Jetpack", 4);
	mode.addEntry("AirJump", 5);
	//registerIntSetting("PlaceDelay", &placeDelay, placeDelay, 2, 20);
	registerFloatSetting("Speed", &speed, speed, 0.3f, 4.f);
	registerFloatSetting("value", &glideMod, glideMod, -0.15f, 0.00);
}

Flight::~Flight() {
}

const char* Flight::getModuleName() {
	return ("Flight");
}

const char* Flight::getRawModuleName() {
	return "Flight";
}

void Flight::onEnable() {
	if (damage) {
		auto player = g_Data.getLocalPlayer();
		player->animateHurt();
	}
	if (mode.getSelectedValue() == 2) {  // BlockFly
		placeCounter = 1;
		if (g_Data.getLocalPlayer() == nullptr)
			return;
		prevSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
	}
}

bool Flight::isFlashMode() {
	if (mode.getSelectedValue() == 4) {
		return true;
	} else {
		return false;
	}
}

void Flight::onTick(C_GameMode* gm) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	auto scaffoldMod = moduleMgr->getModule<Scaffold>();
	auto speedMod = moduleMgr->getModule<Speed>();
	auto player = g_Data.getLocalPlayer();
	scaffoldMod->setEnabled(false);
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
	if (mode.getSelectedValue() == 0 && GameData::isKeyDown(*input->sneakKey)) {  // Vanilla
		g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
	}
	if (mode.getSelectedValue() == 1 && GameData::isKeyDown(*input->sneakKey)) {  // Boost
		g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
	}
	if (mode.getSelectedValue() == 2 && GameData::isKeyDown(*input->sneakKey)) {  // BlockFly
		g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
	}
	if (mode.getSelectedValue() == 3 && GameData::isKeyDown(*input->sneakKey)) {  // Teleport
		g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
	}

	if (mode.getSelectedValue() == 1) {  // Boost
		float yaw = player->yaw;
		if (input->forwardKey && input->backKey && input->rightKey && input->leftKey) {
			gm->player->velocity = vec3_t(0, 0, 0);
		}
		gm->player->velocity.y = glideModEffective;
		if (GameData::isKeyDown(*input->spaceBarKey))
			glideModEffective += 0.0f;
		if (GameData::isKeyDown(*input->sneakKey))
			glideModEffective -= 0.0f;
	}
	glideModEffective = glideMod;

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
	}
	if (mode.getSelectedValue() == 4) {  // Jetpack
		float calcYaw = (gm->player->yaw + 90) * (PI / 180);
		float calcPitch = (gm->player->pitch) * -(PI / 180);
		vec3_t moveVec;
		moveVec.x = cos(calcYaw) * cos(calcPitch) * speed;
		moveVec.y = sin(calcPitch) * speed;
		moveVec.z = sin(calcYaw) * cos(calcPitch) * speed;

		gm->player->lerpMotion(moveVec);
	}
	if (mode.getSelectedValue() == 2) {  // BlockFly
		static auto clickGUI = moduleMgr->getModule<ClickGuiMod>();
		gm->player->velocity.y = glideModEffective;
		static bool restored = false;
		if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "geo.hivebedrock.network") == 0) {
			if (!player->onGround) {
				clientMessageF("%sYou must be on the ground", GRAY);
				this->glideMod = -0.00034065544605255127;
				this->setEnabled(false);
			}
		}
		if (clickGUI->isEnabled()) {
			clientMessageF("%sDisabled to prevent flags/errors", GRAY);
			this->setEnabled(false);
		}
		if (placeCounter == placeDelay /*8*/) {
			placeCounter = 1;
		} else {
			placeCounter++;
		}
		if (placeCounter == 3) {
			*g_Data.getClientInstance()->minecraft->timer = 20.f;
		}
		if (selectBlock() == false) {
			if (!restored) {
				restored = true;
				g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prevSlot;
				if (restored == true) {
					this->setEnabled(false);
				}
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
			*g_Data.getClientInstance()->minecraft->timer = 16.f;
			g_Data.getLocalPlayer()->velocity = vec3_t(0, 0, 0);
			C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());  // Rotations (kinda)
			if (speed > 0.05f) {
				p.pitch = (float)(rand() % 1);
				p.yaw = (float)(rand() % -1);
				gm->player->pitch = (float)(rand() % -154);
				gm->player->bodyYaw = (float)(rand() % -1);
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
	}
	if (mode.getSelectedValue() == 5) {  // AirJump
		if (input == nullptr)
			return;

			int hasJumped = 0;
		if (GameData::isKeyDown(*input->spaceBarKey) && hasJumped == 0) {
			gm->player->onGround = true;
			hasJumped = 1;
		} else if (!GameData::isKeyDown(*input->spaceBarKey)) {
			hasJumped = 0;
		}
	}
}

void Flight::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	if (mode.getSelectedValue() == 3) { // Teleport
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
	}
	if (mode.getSelectedValue() == 1) {  // Boost
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
	}
	if (mode.getSelectedValue() == 2) {  // BlockFly
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


void Flight::onDisable() {
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
	if (mode.getSelectedValue() != 5) {
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
}