#include "TPAura.h"

TPAura::TPAura() : IModule(0x0, Category::COMBAT, "TP Around The Closest Enemy.") {
	registerFloatSetting("Range", &range, range, 3, 10);
	registerIntSetting("Distance", &entDistance, entDistance, 1, 4); //DistanceFromEnemy
	registerFloatSetting("Height", &entHeight, entHeight, -3, 4);
	registerBoolSetting("Use LerpTo", &push, push);
	registerFloatSetting("LerpSpeed", &rangeSpeed, rangeSpeed, 1, 100);
	registerIntSetting("Delay", &delay, delay, 0, 40);
}

TPAura::~TPAura() {
}

const char* TPAura::getModuleName() {
	return ("TPAura");
}

static std::vector<C_Entity*> targetListTA;

void TPAura::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr || !g_Data.isInGame())setEnabled(false);
}

void findEntitiesTA(C_Entity* currentEntity, bool isRegularEntitie) {
	static auto tpaura = moduleMgr->getModule<TPAura>();

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (currentEntity == 0)
		return;

	if (currentEntity->timeSinceDeath > 0 || currentEntity->damageTime >= 7)
		return;

	if (FriendList::findPlayer(currentEntity->getNameTag()->getText()))  // Skip Friend
		return;

	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < tpaura->range) {
		targetListTA.push_back(currentEntity);
	}
}

struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void TPAura::onTick(C_GameMode* gm) {
	targetListTA.clear();

	g_Data.forEachEntity(findEntitiesTA);

	std::sort(targetListTA.begin(), targetListTA.end(), CompareTargetEnArray());

	auto player = g_Data.getLocalPlayer();
	float calcYaw = (gm->player->yaw + 90) * (PI / 180);
	float calcPitch = (gm->player->pitch) * -(PI / 180);
	vec3_t enemyPos = *targetListTA[0]->getPos();

	delay0++;
	if (!targetListTA.empty() && delay0 >= delay) {
	if (push) {
		player->velocity = vec3_t(0, 0, 0);
		player->lerpTo(vec3_t(enemyPos.x + (float)(rand() % entDistance), enemyPos.y + entHeight, enemyPos.z - (float)(rand() % entDistance)), vec2_t(1, 1), (int)fmax((int)rangeSpeed * 0.1, 2.f));
	} else {
		player->velocity = vec3_t(0, 0, 0);
		player->setPos(vec3_t(enemyPos.x + (float)(rand() % entDistance), enemyPos.y + entHeight, enemyPos.z + (float)(rand() % entDistance)));
	}
}
}