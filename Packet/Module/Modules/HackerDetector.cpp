#include "../../DrawUtils.h"
#include "HackerDetector.h"

HackerDetector::HackerDetector() : IModule(0, Category::EXPLOIT, "shit client moment") {

}

HackerDetector::~HackerDetector() {
}

const char* HackerDetector::getModuleName() {
	return ("HackerDetector");
}

static std::vector<C_Entity*> hackerList;

void findHackerMan(C_Entity* currentEntity, bool isRegularEntity) {
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

	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < 255) {
		hackerList.push_back(currentEntity);
	}
}

void HackerDetector::onEnable() {

}

void HackerDetector::onTick(C_GameMode* gm) {
	if (speedCheck) {

	}
	if (flyCheck) {

	}
}

//void HackerDetector::onMove(C_MoveInputHandler* input) {
//}

void HackerDetector::onPostRender(C_MinecraftUIRenderContext* renderCtx) {

}

void HackerDetector::onDisable() {
}