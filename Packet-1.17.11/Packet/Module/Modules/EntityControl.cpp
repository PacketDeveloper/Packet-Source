#include "../../DrawUtils.h"
#include "EntityControl.h"

EntityControl::EntityControl() : IModule(0, Category::EXPLOIT, "Controls Entities") {
	registerBoolSetting("BoatFly", &control, control);
	registerFloatSetting("Speed", &speed, speed, 1, 2);
	registerFloatSetting("Range", &range, range, 1, 8);
}

EntityControl::~EntityControl() {
}

const char* EntityControl::getModuleName() {
	return ("EntityControl");
}

static std::vector<C_Entity*> targetList;

void findEntityController(C_Entity* currentEntity, bool isRegularEntity) {
	static auto entityControl = moduleMgr->getModule<EntityControl>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity->getEntityTypeId() != 90)
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < entityControl->range) {
		targetList.push_back(currentEntity);
	}
}

void EntityControl::onEnable() {
}

void EntityControl::onTick(C_GameMode* gm) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	targetList.clear();
	g_Data.forEachEntity(findEntityController);

	if (!targetList.empty()) { // scuffed ass boat fly lol
		if (control) {
			targetList[0]->velocity.y = 0;
			if (g_Data.canUseMoveKeys()) {
				if (GameData::isKeyDown(*input->spaceBarKey)) // this doesnt even work bc space = jump out of boat
					targetList[0]->velocity.y += speed;
				if (GameData::isKeyDown(*input->sneakKey)) // same with this
					targetList[0]->velocity.y -= speed;
			}
		}
	}
}

void EntityControl::onMove(C_MoveInputHandler* input) {
}

void EntityControl::onDisable() {
}