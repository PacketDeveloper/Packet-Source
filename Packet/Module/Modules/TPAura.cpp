#include "TPAura.h"

TPAura::TPAura() : IModule(0, Category::COMBAT, "TP Into The Closest Entity") {
	registerEnumSetting("Rotations", &mode, 0);
	mode.addEntry("Normal", 0);
	mode.addEntry("Old", 1);
	mode.addEntry("None", 2);
	registerBoolSetting("MultiAura", &multi, multi);
	registerBoolSetting("Attack", &attack, attack);
	registerBoolSetting("Silent", &silent, silent);
	//registerBoolSetting("Hive", &hive, hive);r
	registerIntSetting("Delay", &delay, delay, 0, 50);
	registerFloatSetting("Range", &range, range, 5, 250);
}

TPAura::~TPAura() {
}

const char* TPAura::getModuleName() {
	return ("TPAura");
}

static std::vector<C_Entity*> targetList;

void findPlayer2(C_Entity* currentEntity, bool isRegularEntity) {
	static auto tpaura = moduleMgr->getModule<TPAura>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;

	if (currentEntity->getEntityTypeId() == 80)  // Arrows
		return;

	if (currentEntity->getEntityTypeId() == 51)  // NPC
		return;

	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < tpaura->range) {
		targetList.push_back(currentEntity);
	}
}

void TPAura::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		setEnabled(false);
}

struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void TPAura::onTick(C_GameMode* gm) {
	auto clickGUI = moduleMgr->getModule<ClickGuiMod>();
	targetListEmpty = targetList.empty();

	if (clickGUI->isEnabled()) targetListEmpty = true;

	targetList.clear();

	g_Data.forEachEntity(findPlayer2);

	float calcYaw = (gm->player->yaw + 90) * (PI / 180);
	float calcPitch = (gm->player->pitch) * -(PI / 180);

	float teleportX2 = cos(calcYaw) * cos(calcPitch) * 3.5f;
	float teleportZ2 = sin(calcYaw) * cos(calcPitch) * 3.5f;
	C_MovePlayerPacket teleportPacket2;
	vec3_t pos = *targetList[0]->getPos();
	float teleportX = 0;
	float teleportZ = 0;
	Odelay++;
	if (GameData::canUseMoveKeys()) {
		if (!targetList.empty() && Odelay >= delay) {
			// Attack all entitys in targetList
			if (multi) {
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttime) && attack) {
						if (attack) {
							g_Data.getLocalPlayer()->swing();
							gm->attack(i);
						}
						if (silent) {
							for (int i = 0; i < targetList.size(); i++) {
								vec3_t pos = *targetList[i]->getPos();
								teleportPacket2 = C_MovePlayerPacket(g_Data.getLocalPlayer(), vec3_t(pos.x - teleportX2, pos.y, pos.z - teleportZ2));
								g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket2);
								//gm->attack(targetList[i]);
								teleportPacket2 = C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
								g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket2);
							}
						} else {
							C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
							g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
							C_LocalPlayer* player = g_Data.getLocalPlayer();
							player->setPos(pos);
							player->setPos(vec3_t(pos.x + teleportX, pos.y + teleportY, pos.z + teleportZ));
							//gm->player->velocity.y = -0.1;
						}
						targethud++;
					} else
						targethud = 0;
				}
			} else {
				if (!(targetList[0]->damageTime > 1 && hurttime)) {
					if (attack) {
						g_Data.getLocalPlayer()->swing();
						gm->attack(targetList[0]);
					}
					if (silent) {
						vec3_t pos = *targetList[0]->getPos();
						teleportPacket2 = C_MovePlayerPacket(g_Data.getLocalPlayer(), vec3_t(pos.x - teleportX2, pos.y, pos.z - teleportZ2));
						g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket2);
						gm->attack(targetList[0]);
						teleportPacket2 = C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
						g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket2);
					} else if (!hive) {
						C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
						g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
						C_LocalPlayer* player = g_Data.getLocalPlayer();
						player->setPos(pos);
						player->setPos(vec3_t(pos.x + teleportX, pos.y + teleportY, pos.z + teleportZ));
					}
					targethud++;
				} else
					targethud = 0;
			}
			Odelay = 0;
		}
		for (auto& i : targetList) {
			C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
			PointingStruct* pointing = g_Data.getClientInstance()->getPointerStruct();
			Odelay++;

			if (hive && !targetList.empty()) {
				if (Odelay >= delay) {
					g_Data.leftclickCount++;
					if (pointing->hasEntity())
						gm->attack(pointing->getEntity());
				}
			}
		}
	} else {
		targetListEmpty = true;
	}
}

void TPAura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (GameData::canUseMoveKeys()) {
		auto scaffold = moduleMgr->getModule<Scaffold>();
		if (g_Data.isInGame() && targetList.size() == 1) {
			if (targethud > 1) {
				for (auto& i : targetList) {
					if ((i->getEntityTypeId() == 319)) {
						vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
						static auto hudMod = moduleMgr->getModule<HudModule>();
						vec3_t* pos = targetList[0]->getPos();
						std::string namestr = "Name: ";
						std::string name = namestr + targetList[0]->getNameTag()->getText();
						std::string position = "Position: " + std::to_string((int)floorf(pos->x)) + " " + std::to_string((int)floorf(pos->y)) + " " + std::to_string((int)floorf(pos->z));

						name = Utils::sanitize(name);

						float margin = windowSize.x / 5;
						constexpr float borderPadding = 1;
						constexpr float unused = 5;
						constexpr float idek = 5;

						float nameLength = DrawUtils::getTextWidth(&name) + 20;

						static const float rectHeight = (idek, unused) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();

						vec4_t rectPos = vec4_t(
							windowSize.x - margin - nameLength - 15 - borderPadding * 2,
							windowSize.y - margin - rectHeight + 10,
							windowSize.x - margin + borderPadding - 2,
							windowSize.y - margin + 11);

						vec4_t LinePos = vec4_t(
							windowSize.x - margin - nameLength - 6.5 - borderPadding * 2,
							windowSize.y - margin - rectHeight + 49,
							windowSize.x - margin + borderPadding - 10.5,
							windowSize.y - margin + 7);

						vec2_t TextPos = vec2_t(rectPos.x + 8, rectPos.y + 5);
						vec2_t ArmorPos = vec2_t(rectPos.x + 5.5, rectPos.y + 24);
						vec2_t TextPos2 = vec2_t(rectPos.x + 8, rectPos.y + 15);

						if (targetList[0]->damageTime >= 1) {
							vec2_t TextPosIdk = vec2_t(LinePos.x + 55, LinePos.y + 2);
							std::string Health = " ";
							DrawUtils::drawText(TextPosIdk, &Health, MC_Color(255, 255, 255), 0.67, 1, true);
						}

						if (targetList[0]->damageTime >= 1) {
							DrawUtils::fillRectangle(LinePos, MC_Color(255, 0, 0), 0.5);
							DrawUtils::drawRectangle(LinePos, MC_Color(255, 0, 0), 1);
						} else {
							DrawUtils::fillRectangle(LinePos, MC_Color(0, 255, 0), 0.5);
							DrawUtils::drawRectangle(LinePos, MC_Color(0, 255, 0), 1);
						}

						DrawUtils::flush();

						if ((i->getEntityTypeId() == 319)) {
							static float constexpr opacity = 1;
							float scale = 3 * 0.26f;
							float spacing = scale + 15.f + 2;

							auto* player = reinterpret_cast<C_Player*>(targetList[0]);

							for (int t = 0; t < 4; t++) {
								C_ItemStack* stack = player->getArmor(t);
								if (stack->isValid()) {
									DrawUtils::drawItem(stack, vec2_t(ArmorPos), 1, scale, false);
									ArmorPos.x += scale * spacing;
								}
							}
							C_PlayerInventoryProxy* supplies = player->getSupplies();
							C_ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
							if (item->isValid())
								DrawUtils::drawItem(item, vec2_t(ArmorPos), opacity, scale, item->isEnchanted());
						}

						DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), 0.3);
						//DrawUtils::drawRectangle(rectPos, MC_Color(0, 0, 0), 0.35);
						DrawUtils::drawText(TextPos2, &position, MC_Color(255, 255, 255), 1, 1, true);
						DrawUtils::drawText(TextPos, &name, MC_Color(255, 255, 255), 1, 1, true);
					}
				}
			}
		}
	}
}

void TPAura::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (GameData::canUseMoveKeys()) {
		for (auto& i : targetList) {
			if (g_Data.getLocalPlayer() == nullptr)
				return;
			if (rotations && mode.getSelectedValue() == 0 && !targetList.empty()) {
				vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());
				auto weewee = g_Data.getLocalPlayer();
				weewee->setRot(angle);
			}
			if (rotations && mode.getSelectedValue() == 0 || mode.getSelectedValue() == 1 && !targetList.empty()) {
				vec2_t tpAuraRot = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());
				auto rotation = g_Data.getLocalPlayer();
				float prevyaw = rotation->yawUnused1;
				float prevyaw2 = rotation->yaw;
				float prevyaw3 = rotation->yaw2;
				rotation->setRot(tpAuraRot);

				// Head
				rotation->yawUnused1 = tpAuraRot.y;
				rotation->pitch = tpAuraRot.x;
				rotation->yaw2 = tpAuraRot.y;
				rotation->yaw = prevyaw2;
				rotation->pitch2 = tpAuraRot.x;

				// Body
				if (mode.getSelectedValue() == 0) {
					rotation->bodyYaw = tpAuraRot.y;
					rotation->yawUnused2 = prevyaw2;
				}
			}
			if (rotations && mode.getSelectedValue() == 1 && !targetList.empty()) {
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

void TPAura::onDisable() {
	auto player = g_Data.getLocalPlayer();
}