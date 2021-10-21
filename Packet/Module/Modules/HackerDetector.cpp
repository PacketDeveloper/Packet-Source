#include "../../DrawUtils.h"
#include "HackerDetector.h"

HackerDetector::HackerDetector() : IModule(0, Category::EXPLOIT, "shit client moment") {
	registerEnumSetting("Messages", &mode, 0);
	mode.addEntry("Client", 0);
	mode.addEntry("Server", 1);
	registerBoolSetting("Speed", &speedCheck, speedCheck);
	registerBoolSetting("Fly", &flyCheck, flyCheck);
	registerIntSetting("Sensitivity", &sensitivity, sensitivity, 1, 10);
}

HackerDetector::~HackerDetector() {
}

const char* HackerDetector::getModuleName() {
	return ("HackerDetector");
}

static std::vector<C_Entity*> playerList;

void findHackerMan(C_Entity* currentEntity, bool isRegularEntity) {
	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (currentEntity->getEntityTypeId() != 319)  // Players
		return;

	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < 255) {
		playerList.push_back(currentEntity);
	}
}

void HackerDetector::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr) setEnabled(false);
	time = 0;
}

void HackerDetector::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();

	playerList.clear();
	g_Data.forEachEntity(findHackerMan);

	C_TextPacket textPacket;
	for (auto& i : playerList) {
		name = playerList[0]->getNameTag()->getText();
		name = Utils::sanitize(name);
	}

	if (g_Data.isInGame()) {
		for (auto& i : playerList) {
			vec3_t vel = playerList[0]->velocity;
			if (speedCheck) {
				if (vel.x < 0.f)
					vel.x *= -1.f;
				if (vel.y < 0.f)
					vel.y *= -1.f;
				if (vel.z < 0.f)
					vel.z *= -1.f;

				speed = sqrtf((vel.x * vel.x) + (vel.z * vel.z));
				speed *= 36.6f;

				if (!playerList[0]->onGround)
					speed /= 1.5f;

				if (playerList[0]->onGround) {
					if (speed >= 40 - sensitivity) failedSpeedA = true;
				} else {  // off ground
					if (speed >= 43 - sensitivity) failedSpeedB = true;
				}
				if (failedSpeedA || failedSpeedB) failedSpeed = true;
			}
			if (flyCheck) {
				
				/*if (playerList[0]->onGround) time++;
				else if (time >= 30) {
					if (vel.y == 0) clientMessageF("haxor flyng!");  //failedFlyA = true;
				}*/
				if (!playerList[0]->onGround && playerList[0]->fallDistance == 0) {
					clientMessageF("haxor flyng!");
				}
				if (failedFlyA || failedFlyB) failedFly = true;
			}
		}

		Odelay++;
		if (Odelay > 1 * 5) {
			std::string speedStr = name + " failed Speed-" + std::string(failedSpeedA ? ("A") : ("B")) + " (x" + std::string(std::to_string((int)speed)) + ")";
			std::string flyStr = name + " failed Fly-" + std::string(failedFlyA ? ("A") : ("B"));
			// send messsage
			if (speed <= 39 - sensitivity)
				return;
			if (mode.getSelectedValue() == 0) {  // client
				if (speedCheck && failedSpeed) clientMessageF("%sPacket: %d %s", GRAY, WHITE, speedStr);
				if (flyCheck && failedFly) clientMessageF("%sPacket: %d %s", GRAY, WHITE, flyStr);
			}
			if (mode.getSelectedValue() == 1) {  // server
				if (speedCheck && failedSpeed) textPacket.message.setText(speedStr);
				if (flyCheck && failedFly) textPacket.message.setText(flyStr);

				if (failedSpeed || failedFly) {
					textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
					textPacket.messageType = 69;
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
				}
			}
			Odelay = 0;
		}
	}
}

void HackerDetector::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	auto player = g_Data.getLocalPlayer();
	float x = windowSize.x / 114 + 1;
	float y = windowSize.y - 20;
	float y2 = windowSize.y - 40;

	if (g_Data.isInGame()) {
		for (auto& i : playerList) {

		}
	}
}

void HackerDetector::onDisable() {

}