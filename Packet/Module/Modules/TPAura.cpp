#include "TPAura.h"

TPAura::TPAura() : IModule(0, Category::COMBAT, "fucks players") {
	registerEnumSetting("Rotations", &mode, 0);
	mode.addEntry("Normal", 0);
	mode.addEntry("Smooth", 1);
	mode.addEntry("Old", 2);
	mode.addEntry("Silent", 3);
	mode.addEntry("None", 4);
	registerBoolSetting("Visualize", &visualize, visualize);
	//registerBoolSetting("Multi", &multi, multi);
	//registerIntSetting("TPDelay", &tpDelay, tpDelay, 5, 100);
	registerIntSetting("Delay", &delay, delay, 0, 10);
	registerFloatSetting("Distance", &distance, distance, 0, 10);
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

	if (g_Data.getLocalPlayer() == nullptr || !g_Data.canUseMoveKeys())
		return;

	//Loop through all our players and retrieve their information
	auto player = g_Data.getLocalPlayer();
	targetList.clear();

	g_Data.forEachEntity(findPlayer2);

	float calcYaw = (gm->player->yaw + 90) * (PI / 180);
	float calcPitch = (gm->player->pitch) * -(PI / 180);

	float teleportX = cos(calcYaw) * cos(calcPitch) * distance;
	float teleportZ = sin(calcYaw) * cos(calcPitch) * distance;
	C_MovePlayerPacket teleportPacket;

	Odelay++;
	if (targetList.size() > 0 && Odelay >= delay) {
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		tick++;
		if (tick >= 1 && tick <= 34) {
			auto notification1 = g_Data.addInfoBox("TPAura: Teleported!");
			notification1->closeTimer = 15;
			for (auto& i : targetList) {
				vec3_t pos = *targetList[0]->getPos();
				teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), vec3_t(pos.x - teleportX, pos.y, pos.z - teleportZ));
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
				if (!moduleMgr->getModule<NoSwing>()->isEnabled()) player->swingArm();
				gm->attack(targetList[0]);
			}
		} else if (tick == 35) {
			for (auto& i : targetList) {
				vec3_t pos = *targetList[0]->getPos();
				teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
			}
			tick = 1;
		}
		Odelay = 0;
	}
}

void TPAura::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	for (auto& i : targetList) {
		//std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
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

float ttttt = 0;
void TPAura::onLevelRender() {
	//if (g_Data.getLocalPlayer() == nullptr || !g_Data.canUseMoveKeys())
		//return;
	//DrawUtils::setColor(1, 1, 1, 1);
	//DrawUtils::drawLinestrip3d(position);
	/*auto targetStrafe = moduleMgr->getModule<TargetStrafe>();
	if (targetStrafe->isEnabled())
		return;
	if (!targetList.empty()) {
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		ttttt++;
		DrawUtils::setColor(1, 1, 1, 0.9f);

		vec3_t permutations[56];
		for (int i = 0; i < 56; i++) {
			permutations[i] = {sinf((i * 10.f) / (180 / PI)), 0.f, cosf((i * 10.f) / (180 / PI))};
		}

		const float coolAnim = 0.9f + 0.9f * sin((ttttt / 60) * PI * 1);

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
	}*/
}

void TPAura::onSendPacket(C_Packet* packet) {
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

void TPAura::onDisable() {
	if (g_Data.getLocalPlayer() == nullptr || !g_Data.canUseMoveKeys())
		return;
	C_MovePlayerPacket teleportPacket;
	teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
}