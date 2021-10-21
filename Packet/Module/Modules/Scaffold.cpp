#include "Scaffold.h"

#include "../../../Utils/Logger.h"
#include "../../DrawUtils.h"
#include "../../Module/ModuleManager.h"

Scaffold::Scaffold() : IModule(0, Category::MOVEMENT, "BasicallyBly") {
	registerBoolSetting("Downwards", &staircase, staircase);
	registerBoolSetting("Rotations", &rotations, rotations);
	registerBoolSetting("NoSprint", &noSprint, noSprint);
	registerBoolSetting("AirPlace", &airplace, airplace);
	registerBoolSetting("Tower", &tower, tower);
	registerBoolSetting("LockY", &lockY, lockY);
	registerBoolSetting("Spoof", &spoof, spoof);
	registerIntSetting("TimerBoost", &timer, timer, 20, 80);
	registerIntSetting("Extend", &expand, expand, 0, 7);
}

Scaffold::~Scaffold() {
	if (isOnHive)
		getPlayerAuthInputPacketHolder()->reserve(500);
}

const char* Scaffold::getRawModuleName() {
	return "Scaffold";
}

const char* Scaffold::getModuleName() {
	//if (isOnHive) {
	//name = std::string("Scaffold ") + std::string(GRAY) + std::string("Hive");
	//return name.c_str();
	//} else {
	return "Scaffold";
	//}
}

void Scaffold::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	prevSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
	auto player = g_Data.getLocalPlayer();
}

void Scaffold::onTick(C_GameMode* gm) {
	isOnHive = strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "geo.hivebedrock.network") == 0;
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

	isHoldingSpace = GameData::isKeyDown(*input->spaceBarKey);
	isCrouching = GameData::isKeyDown(*input->sneakKey);

	auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();
	if ((selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr || !selectedItem->getItem()->isBlock()))  // Block in hand?
		return;

	if (noSprint) {
		auto sprint = moduleMgr->getModule<Sprint>();
		sprint->useSprint = false;
		gm->player->setSprinting(false);
	}

	// Adjustment by velocity
	float speed = g_Data.getLocalPlayer()->velocity.magnitudexz();
	vec3_t vel = g_Data.getLocalPlayer()->velocity;
	//vel = vel.normalize();  // Only use values from 0 - 1

	if (extendOut) {
		if (tower && GameData::isKeyDown(*input->spaceBarKey))
			return;
		if (!GameData::isKeyDown(*input->sneakKey)) {
			float cal = (gm->player->yaw + 90) * (PI / 180);

			vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block 1 block below the player
			blockBelow.y -= g_Data.getLocalPlayer()->height;
			blockBelow.x = blockBelow.x += cos(cal) * expand;  // Block 1 ahead the player X
			blockBelow.z = blockBelow.z += sin(cal) * expand;  // Block 1 ahead the player Z
			if (!tryScaffold(blockBelow)) {
				if (speed > 0.05f) {  // Are we actually walking?
					blockBelow.z -= vel.z;
					if (!tryScaffold(blockBelow)) {
						blockBelow.x -= vel.x;

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
		blockBelow.y -= 2.f;

		vec3_t blockBelowBelow = g_Data.getLocalPlayer()->eyePos0;  // Block 2 blocks below the player
		blockBelowBelow.y -= g_Data.getLocalPlayer()->height;
		blockBelowBelow.y -= 3.f;

		if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow)) {
			if (speed > 0.05f) {  // Are we actually walking?
				blockBelow.z -= vel.z;
				blockBelowBelow.z -= vel.z;
				if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow)) {
					blockBelow.x -= vel.x;
					blockBelowBelow.x -= vel.x;
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
			if (isOnHive) {
			}
			//}
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

	if (spoof)
		gm->player->getSupplies()->selectedHotbarSlot = prevSlot;
	if (isOnHive) {
		auto sprint = moduleMgr->getModule<Sprint>();
		sprint->useSprint = false;
		gm->player->setSprinting(true);
	}
	if (rotations) {
		auto player = g_Data.getLocalPlayer();
		vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;
		blockBelow.y -= g_Data.getLocalPlayer()->height;
		blockBelow.y -= 0.5f;
		vec3_t eyePos = player->eyePos0;
		eyePos.y = eyePos.y - 1.5;
		blockBelow2.push_back(eyePos);
		if (speed >= 0.05) {
			if (isHoldingSpace)
				return;
			vec2_t angleBelow = g_Data.getLocalPlayer()->getPos()->CalcAngle(blockBelow);
			auto rotation = g_Data.getLocalPlayer();
			float prevyaw = rotation->yawUnused1;
			float prevyaw2 = rotation->yaw;
			float prevyaw3 = rotation->yaw2;
			rotation->setRot(angleBelow);
			rotation->yawUnused1 = angleBelow.y;
			rotation->pitch = angleBelow.x;
			rotation->yaw2 = angleBelow.y;
			rotation->yaw = prevyaw2;
			rotation->pitch2 = angleBelow.x;
			useRot = false;
		} else {
			useRot = true;
		}
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

	auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();
	if ((selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr || !selectedItem->getItem()->isBlock()))  // Block in hand?
		return;

	if (rotations && g_Data.canUseMoveKeys() && g_Data.getLocalPlayer() != nullptr && g_Data.isInGame()) {
		if (speed > 0.05f) {
			player->bodyYaw = player->yaw - 200;
			left = input->left;
			right = input->right;
		} else {
			left = false;
			right = false;
		}
	}

	if (isOnHive) {
		if (tower && isHoldingSpace) {
			auto clickGUI = moduleMgr->getModule<ClickGuiMod>();
			if (foundCandidate2 && !clickGUI->isEnabled()) {
				*g_Data.getClientInstance()->minecraft->timer = 20;
				vec2_t movement = {input->forwardMovement, -input->sideMovement};
				bool pressed = movement.magnitude() > 0.f;
				float calcYaw = (player->yaw + 90) * (PI / 180);
				vec3_t moveVec;
				float c = cos(calcYaw);
				float s = sin(calcYaw);

				movement = {movement.x * c - movement.y * s, movement.x * s + movement.y * c};
				player->jumpFromGround();
				moveVec.y = player->velocity.y;
				player->velocity.x *= 0;
				player->velocity.z *= 0;
			}
		}
	}
}

bool Scaffold::tryScaffold(vec3_t blockBelow) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	static std::vector<vec3_ti*> checklist;
	blockBelow = blockBelow.floor();
	int i = 0;

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blockBelow));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blockBelow);
		if (lockY) {
			if (!GameData::isKeyDown(*input->spaceBarKey)) {
				if (g_Data.getLocalPlayer()->onGround)
					yLock = blok.y;
				else if (yLock > -1)
					blok.y = yLock;
			}
		}

		bool foundBlock = false;
		if (airplace) {
			g_Data.getCGameMode()->buildBlock(&blok, i);
			return true;
		} else {
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
				if (!g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy->material->isReplaceable) {
					foundBlock = true;
					blok = calc;
					break;
				}
				i++;
			}
			if (foundBlock) {
				g_Data.getCGameMode()->buildBlock(&blok, i);
				return true;
			}
		}
	}
	return false;
}

bool Scaffold::tryTower(vec3_t blockBelow) {  // Tower
	auto player = g_Data.getLocalPlayer();
	if (tower) {
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
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();
	if (!selectedItem->isValid() || selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr || !selectedItem->getItem()->isBlock())
		return;
	vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;
	blockBelow.y -= g_Data.getLocalPlayer()->height;
	blockBelow.y -= 0.5f;
	vec3_t eyePos = player->eyePos0;
	eyePos.y = eyePos.y - 1.5;
	blockBelow2.push_back(eyePos);
	if (tower) {
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
		if (isHoldingSpace && isOnHive) {  // look down
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(blockBelow);
			auto weewee = g_Data.getLocalPlayer();
			weewee->setRot(angle);
			vec2_t angleBelow = g_Data.getLocalPlayer()->getPos()->CalcAngle(blockBelow);
			auto rotation = g_Data.getLocalPlayer();
			float prevyaw = rotation->yawUnused1;
			float prevyaw2 = rotation->yaw;
			float prevyaw3 = rotation->yaw2;
			rotation->setRot(angleBelow);
			rotation->yawUnused1 = angleBelow.y;
			rotation->pitch = angleBelow.x;
			rotation->yaw2 = angleBelow.y;
			rotation->yaw = prevyaw2;
			rotation->pitch2 = angleBelow.x;
		}
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;
	}
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

void Scaffold::onSendPacket(C_Packet* packet) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	auto player = g_Data.getLocalPlayer();
	vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;
	blockBelow.y -= g_Data.getLocalPlayer()->height;
	blockBelow.y -= 0.5f;
	vec3_t eyePos = player->eyePos0;
	eyePos.y = eyePos.y - 1.5;
	blockBelow2.push_back(eyePos);

	float speed = g_Data.getLocalPlayer()->velocity.magnitudexz();
	if (packet->isInstanceOf<C_MovePlayerPacket>() || packet->isInstanceOf<PlayerAuthInputPacket>()) {
		if (g_Data.getLocalPlayer() != nullptr && g_Data.canUseMoveKeys()) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			if (rotations && speed >= 0.05 || GameData::isKeyDown(*input->spaceBarKey)) {
				vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(blockBelow);
				movePacket->pitch = angle.x;
				movePacket->headYaw = angle.y;
				movePacket->yaw = angle.y;
			}
		}
	}
}

void Scaffold::onDisable() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	*g_Data.getClientInstance()->minecraft->timer = 20;
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	auto player = g_Data.getLocalPlayer();
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prevSlot;
	auto sprint = moduleMgr->getModule<Sprint>();
	sprint->useSprint = true;
	useRot = true;
}