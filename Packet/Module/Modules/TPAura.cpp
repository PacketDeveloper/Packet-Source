#include "TPAura.h"

TPAura::TPAura() : IModule(0, Category::COMBAT, "fucks players") {
	registerEnumSetting("Rotations", &mode, 0);
	mode.addEntry("Normal", 0);
	mode.addEntry("Old", 1);
	mode.addEntry("Silent", 2);
	mode.addEntry("None", 3);
	registerBoolSetting("Visualize", &render, render);
	registerBoolSetting("Multi", &multi, multi);
	registerIntSetting("Delay", &delay, delay, 0, 10);
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

	if (!g_Data.canUseMoveKeys()) targetListEmpty = true;
	if (clickGUI->isEnabled()) targetListEmpty = true;

	//Loop through all our players and retrieve their information
	targetList.clear();

	g_Data.forEachEntity(findPlayer2);
	Odelay++;

	if (targetList.size() > 0 && Odelay >= delay) {
		g_Data.getLocalPlayer()->swingArm();

		float calcYaw = (gm->player->yaw + 90) * (PI / 180);
		float calcPitch = (gm->player->pitch) * -(PI / 180);

		float teleportX = cos(calcYaw) * cos(calcPitch) * 3.5f;
		float teleportZ = sin(calcYaw) * cos(calcPitch) * 3.5f;
		C_MovePlayerPacket teleportPacket;

		/*if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "mco.cubecraft.net") == 0) {
			vec3_t pos = *g_Data.getLocalPlayer()->getPos();

			C_MovePlayerPacket movePlayerPacket(g_Data.getLocalPlayer(), pos);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePlayerPacket);

			pos.y += 0.35f;

			movePlayerPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), pos);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePlayerPacket);
		}*/

		// Attack all entitys in targetList
		if (multi) {
			for (int i = 0; i < targetList.size(); i++) {
				vec3_t pos = *targetList[i]->getPos();
				teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), vec3_t(pos.x - teleportX, pos.y, pos.z - teleportZ));
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
				g_Data.getCGameMode()->attack(targetList[i]);

				teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
			}
		} else {
			tick++;
			vec3_t pos = *targetList[0]->getPos();
			teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), vec3_t(pos.x - teleportX, pos.y, pos.z - teleportZ));
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
			g_Data.getCGameMode()->attack(targetList[0]);

			teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
		}
		Odelay = 0;
	}
}

void TPAura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	auto scaffold = moduleMgr->getModule<Scaffold>();
	if (g_Data.getLocalPlayer() == nullptr || !g_Data.canUseMoveKeys())
		return;
	if (targetList.size() == 1 && targethud > 1) {
		for (auto& i : targetList) {
			if (visualize && scaffold->useRot && (i->getEntityTypeId() == 319)) {
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
				vec2_t armorPos = vec2_t(rectPos.x + 5.5, rectPos.y + 24);
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

				if (render && (i->getEntityTypeId() == 319)) {
					static float constexpr opacity = 10;
					float scale = 3 * 0.26f;
					float spacing = scale + 15.f + 2;

					auto* player = reinterpret_cast<C_Player*>(targetList[0]);

					for (int t = 0; t < 4; t++) {
						C_ItemStack* stack = player->getArmor(t);
						if (stack->isValid()) {
							DrawUtils::drawItem(stack, vec2_t(armorPos), 1, scale, false);
							armorPos.x += scale * spacing;
						}
					}
					C_PlayerInventoryProxy* supplies = player->getSupplies();
					C_ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
					if (item->isValid()) DrawUtils::drawItem(item, vec2_t(armorPos), opacity, scale, item->isEnchanted());
				}

				DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), 0.3);
				//DrawUtils::drawRectangle(rectPos, MC_Color(0, 0, 0), 0.35);
				DrawUtils::drawText(TextPos2, &position, MC_Color(255, 255, 255), 1, 1, true);
				DrawUtils::drawText(TextPos, &name, MC_Color(255, 255, 255), 1, 1, true);
			}
		}
	}
}

void TPAura::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr || !g_Data.canUseMoveKeys())
		return;

	for (auto& i : targetList) {
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

float tttt = 0;
void TPAura::onLevelRender() {
	auto targetStrafe = moduleMgr->getModule<TargetStrafe>();
	if (targetStrafe->isEnabled())
		return;
	if (!targetList.empty()) {
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		tttt++;
		DrawUtils::setColor(1, 1, 1, 0.9f);

		vec3_t permutations[56];
		for (int i = 0; i < 56; i++) {
			permutations[i] = {sinf((i * 10.f) / (180 / PI)), 0.f, cosf((i * 10.f) / (180 / PI))};
		}

		const float coolAnim = 0.9f + 0.9f * sin((tttt / 60) * PI * 1);

		vec3_t* start = targetList[0]->getPosOld();
		vec3_t* end = targetList[0]->getPos();

		auto te = DrawUtils::getLerpTime();
		vec3_t pos = start->lerp(end, te);

		auto yPos = pos.y;
		yPos -= 1.6f;
		yPos += coolAnim;

		std::vector<vec3_t> posList;
		posList.reserve(56);
		for (auto& perm : permutations) {
			vec3_t curPos(pos.x, yPos, pos.z);
			posList.push_back(curPos.add(perm));
		}

		DrawUtils::drawLinestrip3d(posList);
	}
}

void TPAura::onDisable() {
}