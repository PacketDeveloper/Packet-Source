#include "../../DrawUtils.h"
#include "HackerDetector.h"

HackerDetector::HackerDetector() : IModule(0, Category::EXPLOIT, "shit client moment") {
	registerBoolSetting("Speed", &speedCheck, speedCheck);
	registerBoolSetting("Fly", &flyCheck, flyCheck);
	registerIntSetting("Range", &range, range, 30, 255);
}

HackerDetector::~HackerDetector() {
}

const char* HackerDetector::getModuleName() {
	return ("HackerDetector");
}

static std::vector<C_Entity*> hackerList;

void findHackerMan(C_Entity* currentEntity, bool isRegularEntity) {
	static auto hackerDetector = moduleMgr->getModule<HackerDetector>();
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

	if (dist < hackerDetector->range) {
		hackerList.push_back(currentEntity);
	}
}

void HackerDetector::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		setEnabled(false);
	hackerList.clear();
}

void HackerDetector::onTick(C_GameMode* gm) {
	hackerList.clear();

	g_Data.forEachEntity(findHackerMan);
	if (g_Data.canUseMoveKeys()) {
		for (auto& i : hackerList) {
			if (speedCheck) {
				int velX = hackerList[0]->velocity.x;
				int velZ = hackerList[0]->velocity.z;
				if (velX || velZ > 2) {
					speedFlag = true;
				} else {
					speedFlag = false;
				}
				//if (hackerList.empty()) hackerList.clear();
			}
			if (flyCheck) {
				int velocityY = hackerList[0]->velocity.y;
				if (velocityY == 0 && !hackerList[0]->onGround) {
					flyFlag = true;
				} else {
					flyFlag = false;
				}
			}
		}
	}
}

void HackerDetector::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	auto player = g_Data.getLocalPlayer();
	float x = windowSize.x / 114 + 1;
	float y = windowSize.y - 20;
	float y2 = windowSize.y - 40;

	if (g_Data.canUseMoveKeys()) {
		for (auto& i : hackerList) {
			std::string name = hackerList[0]->getNameTag()->getText();
			name = Utils::sanitize(name);
			if (speedFlag) {
				std::string textStr = name + " was detected for Speed";
				DrawUtils::drawText(vec2_t(x, y), &textStr, MC_Color(255, 255, 255), 1, 1, true);
			}
			if (flyFlag) {
				std::string textStr = name + " was detected for Fly";
				DrawUtils::drawText(vec2_t(x, y), &textStr, MC_Color(255, 255, 255), 1, 1, true);
			}
		}
	}
}

void HackerDetector::onDisable() {
}