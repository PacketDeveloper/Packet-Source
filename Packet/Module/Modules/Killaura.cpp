#include "Killaura.h"

Killaura::Killaura() : IModule(0, Category::COMBAT, "Automatically attacks entites") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Multi", 0);
	mode.addEntry("Switch", 1);
	registerBoolSetting("Rotations", &rot, rot);
	registerBoolSetting("Distance", &distanceCheck, distanceCheck);
	registerBoolSetting("MobAura", &isMobAura, isMobAura);
	registerBoolSetting("Hurttime", &hurttime, hurttime);
	registerBoolSetting("Strafe", &strafe, strafe);
	registerBoolSetting("Silent", &silent, silent);
	registerBoolSetting("Click", &click, click);
	registerFloatSetting("range", &range, range, 3.f, 8.f);
	registerIntSetting("delay", &delay, delay, 0, 10);
}

Killaura::~Killaura() {
}

const char* Killaura::getModuleName() {
	return ("Killaura");
}

static std::vector<C_Entity*> targetList;

void findEntity(C_Entity* currentEntity, bool isRegularEntity) {
	static auto killauraMod = moduleMgr->getModule<Killaura>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (killauraMod->isMobAura) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
			return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f)  // Don't hit this pesky antibot on 2b2e.org
			return;
		if (currentEntity->getEntityTypeId() == 64)  // item
			return;
	} else {
		if (!Target::isValidTarget(currentEntity))
			return;
	}

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < killauraMod->range) {
		targetList.push_back(currentEntity);
	}
}

void Killaura::findWeapon() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage) {
				damage = currentDamage;
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}

struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void Killaura::onTick(C_GameMode* gm) {
	//Loop through all our players and retrieve their information
	targetList.clear();

	g_Data.forEachEntity(findEntity);

	Odelay++;
	if (distanceCheck)
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
	if (!targetList.empty() && Odelay >= delay) {
		if (autoweapon) findWeapon();
		if (g_Data.getLocalPlayer()->velocity.squaredxzlen() < 0.01) {
			C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);  // make sure to update rotation if player is standing still
		}

		// Attack all entitys in targetList
		if (mode.getSelectedValue() == 0) {  // Multi
			for (auto& i : targetList) {
				if (!(i->damageTime > 1 && hurttime)) {
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(i);
					targethud++;
				} else
					targethud = 0;
			}
		} else {  // Switch
			if (!(targetList[0]->damageTime > 1 && hurttime)) {
				g_Data.getLocalPlayer()->swing();
				g_Data.getCGameMode()->attack(targetList[0]);
				targethud++;
			} else
				targethud = 0;
		}
		Odelay = 0;
	}
	if (targetList.empty()) {
		targethud = 0;
	}

	for (auto& i : targetList) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		PointingStruct* pointing = g_Data.getClientInstance()->getPointerStruct();
		Odelay++;

		if (click && !targetList.empty()) {
			if (Odelay >= delay) {
				g_Data.leftclickCount++;
				if (pointing->hasEntity())
					gm->attack(pointing->getEntity());
			}
		}
		if (strafe && !targetList.empty()) {
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());
			auto weewee = g_Data.getLocalPlayer();
			weewee->setRot(angle);
		}

	}
}

#include <Windows.h>
void Killaura::onEnable() {
	targethud = 0;
	if (g_Data.getLocalPlayer() == nullptr)
		this->setEnabled(false);
	//Minecraft.Windows.exe + 1D4C043;
	//Minecraft.Windows.exe + BFADDA;

	if (offset == nullptr) {
		offset = (int*)(FindSignature("8B 86 ?? ?? ?? ?? 89 86 ?? ?? ?? ?? 8B 86 ?? ?? ?? ?? 89 86 ?? ?? ?? ?? 48 8B 4C 24") + 2);
		if ((uintptr_t)offset == 2)
			__debugbreak();
		VirtualProtect((void*)offset, sizeof(int), PAGE_EXECUTE_READWRITE, (PDWORD)&offsetProt);
	}
	if (offset2 == nullptr) {
		offset2 = (int*)(FindSignature("41 FF D0 F3 0F 10 8B ?? ?? ?? ?? F3 0F 10 83") + 7);
		if ((uintptr_t)offset2 == 7)
			__debugbreak();
		VirtualProtect((void*)offset2, sizeof(int), PAGE_EXECUTE_READWRITE, (PDWORD)&offsetProt2);
	}

	*offset += 8;
	*offset2 += 8;
}

void Killaura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (targetList.size() == 1) {
		if (targethud > 1 && g_Data.canUseMoveKeys()) {
			for (auto& i : targetList) {
				C_GuiData* dat = g_Data.getClientInstance()->getGuiData();
				vec2_t windowSize = dat->windowSize;
				std::string text = targetList[0]->getNameTag()->getText();
				text = Utils::sanitize(text);
				std::string realname = "Name: " + text;
				int dist2 = (int)(*targetList[0]->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
				auto dist = std::to_string(dist2);
				auto distancestring = std::string("Distance: " + dist);
				vec4_t duotagteam = (vec4_t(windowSize.x / 1.5f - (windowSize.x / 7),
											windowSize.y / 1.61f - (windowSize.y / 13),
											windowSize.x / 1.7f + (windowSize.x / 9 + targetList[0]->getNameTag()->textLength),
											windowSize.y / 2 - windowSize.y / 8 + windowSize.y / 4));
				DrawUtils::fillRectangle(vec4_t(duotagteam),
										 MC_Color(0.05f, 0.05f, 0.05f), 0.35f);
				DrawUtils::drawRectangle(vec4_t(duotagteam),
										 MC_Color(1.f, 1.f, 1.f), 1.f);

				DrawUtils::drawText(vec2_t(windowSize.x / 1.5f - windowSize.x / 7.25f,
										   windowSize.y / 2 - windowSize.y / 5.f + windowSize.y / 4),
									&realname,
									MC_Color(1.f, 1.f, 1.f), 1.f);

				DrawUtils::drawText(vec2_t(windowSize.x / 1.5f - windowSize.x / 7.25f,
										   windowSize.y / 2 - windowSize.y / 5.8f + windowSize.y / 4),
									&distancestring,
									MC_Color(1.f, 1.f, 1.f), 1.f);
				DrawUtils::flush();
				vec2_t textPos;
				vec4_t rectPos;
				auto* player = reinterpret_cast<C_Player*>(targetList[0]);
				float x = windowSize.x / 1.5f - windowSize.x / 7.1f;
				float y = windowSize.y / 2 - windowSize.y / 6.4f + windowSize.y / 4;
				float scale = 3 * 0.26f;
				float spacing = scale + 15.f + 2;
				if (i->getEntityTypeId() == 63) {
					// armor
					for (int i = 0; i < 4; i++) {
						C_ItemStack* stack = player->getArmor(i);
						if (stack->item != nullptr) {
							DrawUtils::drawItem(stack, vec2_t(x, y), 1.f, scale, stack->isEnchanted());
							x += scale * spacing;
						}
					}
					// item
					{
						C_ItemStack* stack = player->getSelectedItem();
						if (stack->item != nullptr) {
							DrawUtils::drawItem(stack, vec2_t(x, y), 1.f, scale, stack->isEnchanted());
						}
					}
				}
			}
		}
	}
}

void Killaura::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (targethud > 1) {
		for (auto& i : targetList) {
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());
			if (rot && !targetList.empty()) {
				auto rotation2 = g_Data.getLocalPlayer();
				rotation2->yawUnused1 = angle.y;
				rotation2->pitch = angle.x;
			}
			if (rot && !targetList.empty()) {
				vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());
				auto rotation = g_Data.getLocalPlayer();
				float prevyaw = rotation->yawUnused1;
				float prevyaw2 = rotation->yaw;
				float prevyaw3 = rotation->yaw2;
				rotation->setRot(angle);

				// Head
				rotation->yawUnused1 = angle.y;
				rotation->pitch = angle.x;
				rotation->yaw2 = angle.y;
				rotation->yaw = prevyaw2;
				rotation->pitch2 = angle.x;

				// Body
				rotation->bodyYaw = angle.y;
				rotation->yawUnused2 = prevyaw2;
			}
		}
	}
}


void Killaura::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr && silent) {
		if (rot && !targetList.empty()) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			movePacket->pitch = angle.x;
			movePacket->headYaw = angle.y;
			movePacket->yaw = angle.y;
		}
	}
}

void Killaura::onDisable() {
	targethud = 0;
	if (offset && offset2) {
		*offset -= 8;
		*offset2 -= 8;
		VirtualProtect((void*)offset, sizeof(int), offsetProt, 0);
		VirtualProtect((void*)offset2, sizeof(int), offsetProt2, 0);
	}
}