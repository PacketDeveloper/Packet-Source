#include "Killaura.h"

Killaura::Killaura() : IModule(0, Category::COMBAT, "Automatically attacks entites") {
	registerEnumSetting("Rotations", &mode, 0);
	mode.addEntry("Normal", 0);
	mode.addEntry("Smooth", 1);
	mode.addEntry("Old", 2);
	mode.addEntry("Silent", 3);
	mode.addEntry("None", 4);
	registerBoolSetting("Visualize", &render, render);
	registerBoolSetting("Distance", &distanceCheck, distanceCheck);
	registerBoolSetting("Multi", &multi, multi);
	registerBoolSetting("Mobs", &mobAura, mobAura);
	registerBoolSetting("Click", &click, click);
	registerBoolSetting("Hold", &hold, hold);
	registerFloatSetting("range", &range, range, 3.f, 8.f);
	registerIntSetting("delay", &delay, delay, 0, 10);
#ifdef _DEBUG
	registerBoolSetting("TestMode", &test, test);
#endif
}

Killaura::~Killaura() {
}

const char* Killaura::getRawModuleName() {
	return "Killaura";
}

const char* Killaura::getModuleName() {
	if (multi) {
		name = std::string("Killaura ") + std::string(GRAY) + std::string("Multi");
		return name.c_str();
	} else if (!multi) {
		name = std::string("Killaura ") + std::string(GRAY) + std::string("Switch");
		return name.c_str();
	}
}

static std::vector<C_Entity*> targetList;

void findEntity(C_Entity* currentEntity, bool isRegularEntity) {
	static auto killauraMod = moduleMgr->getModule<Killaura>();

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

	if (killauraMod->mobAura) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
			return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f)  // Don't hit this pesky antibot on 2b2e.org
			return;
		if (currentEntity->getEntityTypeId() == 64)  //item
			return;

	} else {
		if (!Target::isValidTarget(currentEntity))
			return;

		if (currentEntity->getEntityTypeId() == 1677999)  // Villager
			return;

		if (currentEntity->getEntityTypeId() == 51)  // NPC
			return;
	}

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < killauraMod->range) {
		targetList.push_back(currentEntity);
	}
}

struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void Killaura::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		setEnabled(false);
	targetList.clear();
	targethud = 0;
}

void Killaura::onTick(C_GameMode* gm) {
	auto clickGUI = moduleMgr->getModule<ClickGuiMod>();
	auto scaffold = moduleMgr->getModule<Scaffold>();
	auto player = g_Data.getLocalPlayer();
	targetListEmpty = targetList.empty();
	if (clickGUI->isEnabled()) targetListEmpty = true;
	if (g_Data.getLocalPlayer() != nullptr) holdingWeapon = false;

	targetList.clear();

	g_Data.forEachEntity(findEntity);

	if (GameData::canUseMoveKeys()) {
		if (hold && !g_Data.isLeftClickDown())
			return;

		if (scaffold->useRot) {
			Odelay++;
			if (distanceCheck)
				std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
			if (!targetList.empty() && Odelay >= delay) {
				// Attack all entitys in targetList
				if (multi) {
					for (auto& i : targetList) {
						if (!moduleMgr->getModule<NoSwing>()->isEnabled())
							player->swingArm();
						if (!(i->damageTime > 1 && hurttime)) {
							gm->attack(i);
							targethud++;
						} else
							targethud = 0;
					}
				} else {  // Switch
					if (!(targetList[0]->damageTime > 1 && hurttime)) {
						gm->attack(targetList[0]);
						targethud++;
					} else
						targethud = 0;
				}
				Odelay = 0;
			}
			for (auto& i : targetList) {
				C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
				PointingStruct* pointing = g_Data.getClientInstance()->getPointerStruct();
				if (!moduleMgr->getModule<NoSwing>()->isEnabled())
					player->swingArm();
				Odelay++;

				if (click && !targetList.empty()) {
					if (Odelay >= delay) {
						g_Data.leftclickCount++;
						if (pointing->hasEntity())
							gm->attack(pointing->getEntity());
					}
				}
			}
		}
	} else {
		targetListEmpty = true;
	}
	if (g_Data.getLocalPlayer() != nullptr) {
		auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();
		if ((selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr || !selectedItem->getItem()->isWeapon()))
			return;
		holdingWeapon = true;
	}
}

void Killaura::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (GameData::canUseMoveKeys()) {
		if (hold && !g_Data.isLeftClickDown())
			return;
		auto scaffold = moduleMgr->getModule<Scaffold>();
		auto player = g_Data.getLocalPlayer();
		if (scaffold->useRot) {
			for (auto& i : targetList) {
				if (g_Data.getLocalPlayer() == nullptr)
					return;
				if (mode.getSelectedValue() == 0 || mode.getSelectedValue() == 1 && !targetList.empty()) {
					vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());
					auto weewee = g_Data.getLocalPlayer();
					weewee->setRot(angle);
				}
				if (mode.getSelectedValue() == 0 || mode.getSelectedValue() == 1 && !targetList.empty()) {
					vec2_t testRot = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());
					auto rotation = g_Data.getLocalPlayer();
					float prevyaw = rotation->yawUnused1;
					float prevyaw2 = rotation->yaw;
					float prevyaw3 = rotation->yaw2;
					rotation->setRot(testRot);

					// Head
					rotation->yawUnused1 = testRot.y;
					rotation->pitch = testRot.x;
					rotation->yaw2 = testRot.y;
					rotation->yaw = prevyaw2;
					rotation->pitch2 = testRot.x;

					// Body
					if (mode.getSelectedValue() == 0) {
						rotation->bodyYaw = testRot.y;
						rotation->yawUnused2 = prevyaw2;
					}
				}
				if (mode.getSelectedValue() == 2 && !targetList.empty()) {
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
}

void Killaura::onSendPacket(C_Packet* packet) {
	if (GameData::canUseMoveKeys()) {
		auto scaffold = moduleMgr->getModule<Scaffold>();
		if (scaffold->useRot) {
			if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr && mode.getSelectedValue() == 3) {
				if (!targetList.empty()) {
					auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
					vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
					movePacket->pitch = angle.x;
					movePacket->headYaw = angle.y;
					movePacket->yaw = angle.y;
				}
			}
		}
	}
}

float tttt = 0;
void Killaura::onLevelRender() {
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

void Killaura::onDisable() {
	useSprint = true;
	targethud = 0;
	targetList.clear();
}