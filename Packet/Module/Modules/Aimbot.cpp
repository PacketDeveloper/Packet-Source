#include "Aimbot.h"

Aimbot::Aimbot() : IModule(0, Category::COMBAT, "Automatically aims at the nearest entity") {
	this->registerBoolSetting("Vertical", &this->vertical, this->vertical);
	this->registerBoolSetting("Hold", &this->click, this->click);
	this->registerFloatSetting("range", &this->range, this->range, 3.f, 8.f);
	//this->registerBoolSetting("only swords/axes", &this->sword, this->sword);
	this->registerFloatSetting("Speed", &this->speedAm, this->speedAm, 10.f, 90.f);
	//this->registerFloatSetting("vertical speed", &this->verticalspeed, this->verticalspeed, 10.f, 90.f);
	this->registerFloatSetting("Angle", &this->rangeAm, this->rangeAm, 20.f, 180.f);
	//this->registerFloatSetting("vertical range", &this->verticalrange, this->verticalrange, 20.f, 180.f);
	//this->registerBoolSetting("aimlock", &this->lock, this->lock);
}

Aimbot::~Aimbot() {
}

const char* Aimbot::getModuleName() {
	return ("Aimbot");
}

struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void Aimbot::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;
	C_EntityList* entList = g_Data.getEntityList();
	if (entList == nullptr)
		return;
	size_t listSize = entList->getListSize();

	if (listSize > 1000) {
		return;
	}

	vec3_t origin = g_Data.getClientInstance()->levelRenderer->origin;

	//Loop through all our players and retrieve their information
	static std::vector<C_Entity*> targetList;
	targetList.clear();
	for (size_t i = 0; i < listSize; i++) {
		C_Entity* currentEntity = entList->get(i);

		if (!Target::isValidTarget(currentEntity))
			continue;

		float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

		if (dist < range)
			targetList.push_back(currentEntity);
	}

	if (targetList.size() > 0) {
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		vec2_t angle = origin.CalcAngle(*targetList[0]->getPos());
		vec2_t appl = angle.sub(localPlayer->viewAngles).normAngles();
		appl.x = -appl.x;
		if ((appl.x < rangeAm && appl.x > -rangeAm) && (appl.y < rangeAm && appl.y > -rangeAm) && GameData::canUseMoveKeys()) {
			auto selectedItemId = localPlayer->getSelectedItemId();
			if (sword && !(selectedItemId == 268 || selectedItemId == 267 || selectedItemId == 272 || selectedItemId == 276 || selectedItemId == 283 /*swords*/ || selectedItemId == 271 || selectedItemId == 275 || selectedItemId == 279 || selectedItemId == 286 || selectedItemId == 258 /*axes*/))
				return;

			if (click && !g_Data.isLeftClickDown())
				return;

			if (!this->lock) {
				appl.x /= (100.f - speedAm);
				appl.y /= (100.f - speedAm);
				if (appl.x >= 1 || appl.x <= -1) appl.div(abs(appl.x));
				if (appl.y >= 1 || appl.y <= -1) appl.div(abs(appl.y));
			}
			if (!vertical)
				appl.x = 0;
			localPlayer->applyTurnDelta(&appl);
		}
	}
}
