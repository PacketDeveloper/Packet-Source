#include "Scaffold.h"

#include "../../../Utils/Logger.h"
#include "../../DrawUtils.h"
#include "../../Module/ModuleManager.h"

Scaffold::Scaffold() : IModule(0, Category::MOVEMENT, "BasicallyBly") {
	registerBoolSetting("Downwards", &staircase, staircase);
	registerBoolSetting("Rotations", &rotations, rotations);
	registerBoolSetting("AirPlace", &airplace, airplace);
	registerBoolSetting("Tower", &towerMode, towerMode);
	//registerBoolSetting("LockY", &lockY, lockY);
	registerBoolSetting("Spoof", &spoof, spoof);
	registerIntSetting("TimerBoost", &timer, timer, 20, 80);
	registerIntSetting("Extend", &expand, expand, 0, 7);
}

Scaffold::~Scaffold() {
}

const char* Scaffold::getRawModuleName() {
	return "Scaffold";
}

const char* Scaffold::getModuleName() {
	if (spoof) {
		name = std::string("Scaffold ") + std::string(GRAY) + std::string("Spoof");
		return name.c_str();
	} else if (!spoof) {
		name = std::string("Scaffold");
		return name.c_str();
	}
}

void Scaffold::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	prevSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
	auto player = g_Data.getLocalPlayer();
	if (lockY) {
		vec3_t blockBelowLock = *player->getPos();
		blockBelowLock.y -= 2.f;

		initialPos = blockBelowLock;
	}
}

void Scaffold::onTick(C_GameMode* gm) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	*g_Data.getClientInstance()->minecraft->timer = static_cast<float>(this->timer);
	auto breakerMod = moduleMgr->getModule<Breaker>();
	auto speedMod = moduleMgr->getModule<Speed>();
	auto scMod = moduleMgr->getModule<Scaffold>();
	if (input == nullptr) return;
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;

	if (switchMode) {
		static bool restored = false;
		if (!selectBlock()) {
			if (!restored) {
				restored = true;
				g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prevSlot;
				if (restored == true)
					setEnabled(false);
			}
			return;
		}
		restored = false;
	}

	isHoldingSpace = (GameData::isKeyDown(*input->spaceBarKey));
	isOnHive = strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "geo.hivebedrock.network") == 0;

	auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();
	if ((selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr || !selectedItem->getItem()->isBlock()))  // Block in hand?
		return;

	// Adjustment by velocity
	float speed = g_Data.getLocalPlayer()->velocity.magnitudexz();
	vec3_t vel = g_Data.getLocalPlayer()->velocity;
	vel = vel.normalize();  // Only use values from 0 - 1
	if (extendMode) {
		if (towerMode && GameData::isKeyDown(*input->spaceBarKey))
			return;
		if (!GameData::isKeyDown(*input->sneakKey)) {
			float cal = (gm->player->yaw + 90) * (PI / 180);

			vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block 1 block below the player
			blockBelow.y -= g_Data.getLocalPlayer()->height;
			blockBelow.x = blockBelow.x += cos(cal) * expand;  // Block 1 ahead the player X
			blockBelow.z = blockBelow.z += sin(cal) * expand;  // Block 1 ahead the player Z
			if (!tryScaffold(blockBelow)) {
				if (speed > 0.05f) {  // Are we actually walking?
					blockBelow.z -= vel.z * 0.4f;
					if (!tryScaffold(blockBelow)) {
						blockBelow.x -= vel.x * 0.4f;

						if (!tryScaffold(blockBelow) && g_Data.getLocalPlayer()->isSprinting()) {
							blockBelow.z += vel.z;
							blockBelow.x += vel.x;
							tryScaffold(blockBelow);
						}
					}
				}
			}
		}
	}

	if (staircase && GameData::isKeyDown(*input->sneakKey)) {
		g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
		vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block 1 block below the player
		blockBelow.y -= g_Data.getLocalPlayer()->height;
		blockBelow.y -= 1.5f;

		vec3_t blockBelowBelow = g_Data.getLocalPlayer()->eyePos0;  // Block 2 blocks below the player
		blockBelowBelow.y -= g_Data.getLocalPlayer()->height;
		blockBelowBelow.y -= 2.0f;

		if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow)) {
			if (speed > 0.05f) {  // Are we actually walking?
				blockBelow.z -= vel.z * 0.4f;
				blockBelowBelow.z -= vel.z * 0.4f;
				if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow)) {
					blockBelow.x -= vel.x * 0.4f;
					blockBelowBelow.x -= vel.x * 0.4f;
					if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow) && g_Data.getLocalPlayer()->isSprinting()) {
						blockBelow.z += vel.z;
						blockBelow.x += vel.x;
						blockBelowBelow.z += vel.z;
						blockBelowBelow.x += vel.x;
						tryScaffold(blockBelow);
						tryScaffold(blockBelowBelow);
					}
				}
			}
		}
	} else {
		vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block below the player
		blockBelow.y -= g_Data.getLocalPlayer()->height;
		blockBelow.y -= 0.5f;

		if (!tryScaffold(blockBelow)) {
			blockBelow.z -= vel.z * 0.4f;
			if (!tryScaffold(blockBelow)) {
				blockBelow.x -= vel.x * 0.4f;
				if (!tryScaffold(blockBelow) && g_Data.getLocalPlayer()->isSprinting()) {
					blockBelow.z += vel.z;
					blockBelow.x += vel.x;
					tryScaffold(blockBelow);
				}
			}
		}
	}
	if (lockY) {
		float cal = (gm->player->yaw + 90) * (PI / 180);
		vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block below the player
		blockBelow.y = initialPos.y;

		//if (useExtend) {
		blockBelow.x = blockBelow.x += cos(cal) * expand;
		blockBelow.z = blockBelow.z += sin(cal) * expand;
		//}

		if (!tryScaffold(blockBelow)) {
			if (speed > 0.05f) {  // Are we actually walking?
				blockBelow.z -= vel.z * 0.4f;
				if (!tryScaffold(blockBelow)) {
					blockBelow.x -= vel.x * 0.4f;
					if (!tryScaffold(blockBelow) && g_Data.getLocalPlayer()->isSprinting()) {
						blockBelow.z += vel.z;
						blockBelow.x += vel.x;
						tryScaffold(blockBelow);
					}
				}
			}
		}
	}
	if (spoof) {
		gm->player->getSupplies()->selectedHotbarSlot = prevSlot;
	}
}

void Scaffold::onMove(C_MoveInputHandler* input) {
	C_GameSettingsInput* inpute = g_Data.getClientInstance()->getGameSettingsInput();
	if (inpute == nullptr) return;
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	float speed = g_Data.getLocalPlayer()->velocity.magnitudexz();
	auto player = g_Data.getLocalPlayer();
	if (rotations && g_Data.canUseMoveKeys() && g_Data.getLocalPlayer() != nullptr && g_Data.isInGame()) {
		if (speed > 0.05f) {
			player->bodyYaw = player->yaw - 200;
			forward = input->forward;
			left = input->left;
			right = input->right;
			backwards = input->backward;
		} else {
			forward = false;
			left = false;
			right = false;
			backwards = false;
		}
	}
	if (towerMode && isOnHive && isHoldingSpace) {
		auto clickGUI = moduleMgr->getModule<ClickGuiMod>();
		if (foundCandidate2 && !clickGUI->isEnabled()) {
			vec2_t movement = {input->forwardMovement, -input->sideMovement};
			bool pressed = movement.magnitude() > 0.f;
			float calcYaw = (player->yaw + 90) * (PI / 180);
			vec3_t moveVec;
			float c = cos(calcYaw);
			float s = sin(calcYaw);

			movement = {movement.x * c - movement.y * s, movement.x * s + movement.y * c};
			player->jumpFromGround();
			moveVec.y = player->velocity.y;
			player->velocity.x = 0;
			player->velocity.z = 0;
		}
	}
	if (isOnHive) {
		if (isHoldingSpace) {
			useRot = false;
		} else {
			useRot = true;
		}
	} else {
		useRot = true;
	}
}

bool Scaffold::tryScaffold(vec3_t blockBelow) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	blockBelow = blockBelow.floor();
	int i = 0;

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blockBelow));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blockBelow);
		/*if (lockY) {
			if (!GameData::isKeyDown(*input->spaceBarKey)) {
				if (g_Data.getLocalPlayer()->onGround)
					yLock = blok.y;
				else if (yLock > -1)
					blok.y = yLock;
			}
		}*/

		if (airplace) {
			g_Data.getCGameMode()->buildBlock(&blok, i);
			return true;
		} else {
			static std::vector<vec3_ti*> checklist;
			bool foundCandidate = false;
			if (checklist.empty()) {
				checklist.push_back(new vec3_ti(0, -1, 0));
				checklist.push_back(new vec3_ti(0, 1, 0));

				checklist.push_back(new vec3_ti(0, 0, -1));
				checklist.push_back(new vec3_ti(0, 0, 1));

				checklist.push_back(new vec3_ti(-1, 0, 0));
				checklist.push_back(new vec3_ti(1, 0, 0));
			}

			for (auto current : checklist) {
				vec3_ti calc = blok.sub(*current);
				bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
				if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
					// Found a solid block to click
					foundCandidate = true;
					blok = calc;
					break;
				}
				i++;
			}
			if (foundCandidate) {
				g_Data.getCGameMode()->buildBlock(&blok, i);
				return true;
			}
		}
	}
	return false;
}

bool Scaffold::tryTower(vec3_t blockBelow) {  // Tower
	auto player = g_Data.getLocalPlayer();
	if (towerMode) {
		C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

		if (input == nullptr)
			return false;

		blockBelow = blockBelow.floor();

		DrawUtils::drawBox(blockBelow, vec3_t(blockBelow).add(0), 0.f);

		C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blockBelow));
		C_BlockLegacy* blockLegacy = (block->blockLegacy);
		if (blockLegacy->material->isReplaceable) {
			vec3_ti blok(blockBelow);

			// Find neighbour
			static std::vector<vec3_ti*> checklist;
			if (checklist.empty()) {
				checklist.push_back(new vec3_ti(0, -1, 0));
				checklist.push_back(new vec3_ti(0, 1, 0));

				checklist.push_back(new vec3_ti(0, 0, -1));
				checklist.push_back(new vec3_ti(0, 0, 1));

				checklist.push_back(new vec3_ti(-1, 0, 0));
				checklist.push_back(new vec3_ti(1, 0, 0));
			}
			bool foundCandidate = false;
			int i = 0;
			for (auto current : checklist) {
				vec3_ti calc = blok.sub(*current);
				if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
					// Found a solid block to click
					foundCandidate = true;
					blok = calc;
					break;
				}
				i++;
			}
			foundCandidate2 = foundCandidate;
			if (foundCandidate && GameData::isKeyDown(*input->spaceBarKey)) {
				vec3_t moveVec2;
				moveVec2.x = g_Data.getLocalPlayer()->velocity.x;
				if (!isOnHive) {
					moveVec2.y = 0.4;
				}
				moveVec2.z = g_Data.getLocalPlayer()->velocity.z;
				g_Data.getLocalPlayer()->lerpMotion(moveVec2);
				g_Data.getCGameMode()->buildBlock(&blok, i);

				return true;
			}
			if (airplace && !isOnHive && isHoldingSpace) {
				vec3_t moveVec2;
				moveVec2.x = g_Data.getLocalPlayer()->velocity.x;
				moveVec2.y = 0.4;
				moveVec2.z = g_Data.getLocalPlayer()->velocity.z;
				g_Data.getLocalPlayer()->lerpMotion(moveVec2);
				g_Data.getCGameMode()->buildBlock(&blok, i);

				return true;
			}
		}
		return false;
	}
	return false;
}

void Scaffold::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	float speedY = g_Data.getLocalPlayer()->velocity.magnitudexy();
	float speed = g_Data.getLocalPlayer()->velocity.magnitudexz();
	auto player = g_Data.getLocalPlayer();
	if (towerMode)
		if (g_Data.getLocalPlayer() == nullptr)
			return;
	if (!g_Data.canUseMoveKeys())
		return;
	auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();
	if (!selectedItem->isValid() || !(*selectedItem->item)->isBlock())  // Block in hand?
		return;

	vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;
	blockBelow.y -= g_Data.getLocalPlayer()->height;
	blockBelow.y -= 0.5f;

	// Adjustment by velocity
	vec3_t vel = g_Data.getLocalPlayer()->velocity;
	vel.normalize();  // Only use values from 0 - 1

	if (!tryTower(blockBelow)) {
		if (speedY > 0.05f) {
			blockBelow.z -= vel.z * 0.4f;
			if (!tryTower(blockBelow)) {
				blockBelow.x -= vel.x * 0.4f;
				if (!tryTower(blockBelow) && g_Data.getLocalPlayer()->isSprinting()) {
					blockBelow.z += vel.z;
					blockBelow.x += vel.x;
					tryTower(blockBelow);
				}
			}
		}
	}

	if (isOnHive) {
		if (towerMode && isHoldingSpace) {
			vec3_t eyePos = player->eyePos0;
			eyePos.y = eyePos.y - 1.5;
			blockBeloww.push_back(eyePos);
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(blockBelow);
			auto weewee = g_Data.getLocalPlayer();
			weewee->setRot(angle);
			vec2_t scaffoldRot = g_Data.getLocalPlayer()->getPos()->CalcAngle(blockBelow);
			auto rotation = g_Data.getLocalPlayer();
			float prevyaw = rotation->yawUnused1;
			float prevyaw2 = rotation->yaw;
			float prevyaw3 = rotation->yaw2;
			rotation->setRot(scaffoldRot);
			rotation->yawUnused1 = scaffoldRot.y;
			rotation->pitch = scaffoldRot.x;
			rotation->yaw2 = scaffoldRot.y;
			rotation->yaw = prevyaw2;
			rotation->pitch2 = scaffoldRot.x;
		}
	}
	if (isOnHive) {
		useRot = (forward || backwards || left || right || (GameData::isKeyDown(*input->spaceBarKey)));
	} else {
		useRot = true;
	}
	if (backwards) {
		player->bodyYaw = -360;
		player->viewAngles;
	}
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
}

bool Scaffold::findBlock() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->isBlock() && (*stack->item)->itemId != 0 && (*stack->item)->itemId != 46) {
				C_MobEquipmentPacket a(id, *stack, n, n);
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
				return true;
			}
		}
	}
	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return false;
}

bool Scaffold::selectBlock() {
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

void Scaffold::onDisable() {
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
	useRot = true;
	auto player = g_Data.getLocalPlayer();
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	//g_Data.getClientInstance()->loopbackPacketSender->sendToServer((C_Packet*)((uintptr_t)&a + 16));
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prevSlot;
	if (!player->onGround && towerMode) {
	}
	//g_Data.getLocalPlayer()->velocity = vec3_t(0, 0, 0);
}