#include "../../DrawUtils.h"
#include "HackerDetector.h"

HackerDetector::HackerDetector() : IModule(0, Category::EXPLOIT, "shit client moment") {
	registerBoolSetting("Speed", &speedCheck, speedCheck);
	registerBoolSetting("Fly", &flyCheck, flyCheck);
	registerIntSetting("Range", &range, range, 50, 255);
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
	time = 1;
}

void HackerDetector::onTick(C_GameMode* gm) {
	//if (hackerList.empty())
		//return;
	hackerList.clear();

	g_Data.forEachEntity(findHackerMan);

	if (g_Data.canUseMoveKeys() && g_Data.isInGame()) {
		for (auto& i : hackerList) {

			// Speed Checks
			if (speedCheck) {

			}

			if (flyCheck) {
				if (time == INFINITY) {
					time = 1;
				} else {
					time++;
				}
				if (hackerList[0]->onGround) {
					time = 1;
				}

				int velocityY = hackerList[0]->velocity.y;

				if (velocityY == 0 && !hackerList[0]->onGround && time >= 400) {
					clientMessageF("time >= 400");
					flag2 = true;
				} else {
					flag2 = false;
				}
			}
		}
	}
}

void HackerDetector::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	//if (hackerList.empty())
		//return;
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	auto player = g_Data.getLocalPlayer();
	float x = windowSize.x / 114 + 1;
	float y = windowSize.y - 20;
	float y2 = windowSize.y - 40;

	if (g_Data.canUseMoveKeys() && g_Data.isInGame()) {
		for (auto& i : hackerList) {
			std::string name = hackerList[0]->getNameTag()->getText();
			name = Utils::sanitize(name);
			if (flag1) {
				std::string textStr = name + " was detected for Speed";
				DrawUtils::drawText(vec2_t(x, y), &textStr, MC_Color(255, 255, 255), 1, 1, true);
			}
			if (flag2) {
				auto box = g_Data.addInfoBox(name + " flagged for: Fly1");
				box->closeTimer = 2;
			}
		}
	}
}

void HackerDetector::onDisable() {

}