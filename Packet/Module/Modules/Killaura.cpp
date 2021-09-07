#include "Killaura.h"

Killaura::Killaura() : IModule(0, Category::COMBAT, "Automatically attacks entites") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Multi", 0);
	mode.addEntry("Switch", 1);  //bolz
	//registerBoolSetting("ShowTarget", &render, render);
	registerBoolSetting("Rotations", &rot, rot);
	registerBoolSetting("Distance", &distanceCheck, distanceCheck);
	registerBoolSetting("MobAura", &isMobAura, isMobAura);
	//registerBoolSetting("Hurttime", &hurttime, hurttime);
	registerBoolSetting("Strafe", &strafe, strafe);
	registerBoolSetting("Silent", &silent, silent);
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

const char* Killaura::getModuleName() {
	return ("Killaura");
}

void Killaura::onEnable() {
	targethud = 0;
	if (g_Data.getLocalPlayer() == nullptr)
		setEnabled(false);
	if (render)
		renderStart++;
}

static std::vector<C_Entity*> targetList;

void findEntity(C_Entity* currentEntity, bool isRegularEntity) {
	if ((GameData::canUseMoveKeys())) {
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

		if (killauraMod->isMobAura) {
			if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
				return;
			if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f)  // Don't hit this pesky antibot on 2b2e.org
				return;
			if (currentEntity->getEntityTypeId() == 64)  //item
				return;

		} else {
			if (!Target::isValidTarget(currentEntity))
				return;
		}

		float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

		if (dist < killauraMod->range) {
			targetList.push_back(currentEntity);
		}
	}
}

struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void Killaura::onTick(C_GameMode* gm) {
	if (silent && rot) {
		auto KAbox = g_Data.addInfoBox("Killaura: Disabled to prevent crash");
		KAbox->closeTimer = 10;
		silent = false;
	}
	if (targetList.empty()) {
		targetListA = true;
	} else {
		targetListA = false;
	}
	if (GameData::canUseMoveKeys()) {
		if (hold && !g_Data.isLeftClickDown())
			return;
		auto scaffold = moduleMgr->getModule<Scaffold>();

		if (renderStart >= 1)
			renderStart++;
		if (renderStart >= 5)
			renderStart = 5;

		if (scaffold->useRot) {
			//Loop through all our players and retrieve their information
			targetList.clear();

			g_Data.forEachEntity(findEntity);

			Odelay++;
			if (distanceCheck)
				std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
			if (!targetList.empty() && Odelay >= delay) {
				// Attack all entitys in targetList
				if (mode.getSelectedValue() == 0) {  // Multi
					for (auto& i : targetList) {
						if (!(i->damageTime > 1 && hurttime)) {
							g_Data.getLocalPlayer()->swing();
							gm->attack(i);
							targethud++;
						} else
							targethud = 0;
					}
				} else {  // Switch
					if (!(targetList[0]->damageTime > 1 && hurttime)) {
						g_Data.getLocalPlayer()->swing();
						gm->attack(targetList[0]);
						targethud++;
					} else
						targethud = 0;
				}
				Odelay = 0;
			}
#ifdef _DEBUG
			//fake auto block
			if (targetList.empty()) {
				targethud = 0;
				useSprint = true;
			} else {
				gm->player->setSprinting(false);
				useSprint = false;
			}
#endif

			for (auto& i : targetList) {
				C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
				PointingStruct* pointing = g_Data.getClientInstance()->getPointerStruct();
				Odelay++;

				if (click && !targetList.empty()) {
					if (Odelay >= delay) {
						g_Data.leftclickCount++;
						if (pointing->hasEntity())
							gm->attack(pointing->getEntity());
					}
				}
				if (strafe && targetListA) {
					vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());
					auto weewee = g_Data.getLocalPlayer();
					weewee->setRot(angle);
				}
				if (test && !targetList.empty()) {
					vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());
					auto weewee = g_Data.getLocalPlayer();
					weewee->setRot(angle);
				}
				if (test && !targetList.empty()) {
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
					rotation->bodyYaw = testRot.y;
					rotation->yawUnused2 = prevyaw2;
				}
				if (rot && !targetList.empty()) {
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

void Killaura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (GameData::canUseMoveKeys()) {
		if (hold && !g_Data.isLeftClickDown())
			return;
		auto scaffold = moduleMgr->getModule<Scaffold>();
		if (render && scaffold->useRot) {
			// will recode this shit later
			if (targetList.size() == 1) {
				if (targethud > 1 && g_Data.canUseMoveKeys()) {
					for (auto& i : targetList) {
						C_GuiData* dat = g_Data.getClientInstance()->getGuiData();
						vec2_t windowSize = dat->windowSize;
						std::string text = targetList[0]->getNameTag()->getText();
						text = Utils::sanitize(text);
						std::string realname = "Name: " + text;
						int dist2 = (int)(*targetList[0]->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
						auto dist = std::to_string(dist2);
						auto distancestring = std::string("Distance: " + dist);
						vec4_t duotagteam = (vec4_t(windowSize.x / 1.5f - (windowSize.x / 7),
													windowSize.y / 1.61f - (windowSize.y / 13),
													windowSize.x / 1.7f + (windowSize.x / 9 + targetList[0]->getNameTag()->textLength),
													windowSize.y / 2 - windowSize.y / 8 + windowSize.y / 4));
						DrawUtils::fillRectangle(vec4_t(duotagteam),
												 MC_Color(0.05f, 0.05f, 0.05f), 0.35f);
						DrawUtils::drawRectangle(vec4_t(duotagteam),
												 MC_Color(1.f, 1.f, 1.f), 1.f);

						DrawUtils::drawText(vec2_t(windowSize.x / 1.5f - windowSize.x / 7.25f,
												   windowSize.y / 2 - windowSize.y / 5.f + windowSize.y / 4),
											&realname,
											MC_Color(1.f, 1.f, 1.f), 1.f);

						DrawUtils::drawText(vec2_t(windowSize.x / 1.5f - windowSize.x / 7.25f,
												   windowSize.y / 2 - windowSize.y / 5.8f + windowSize.y / 4),
											&distancestring,
											MC_Color(1.f, 1.f, 1.f), 1.f);
						DrawUtils::flush();
						vec2_t textPos;
						vec4_t rectPos;
						auto* player = reinterpret_cast<C_Player*>(targetList[0]);
						float x = windowSize.x / 1.5f - windowSize.x / 7.1f;
						float y = windowSize.y / 2 - windowSize.y / 6.4f + windowSize.y / 4;
						float scale = 3 * 0.26f;
						float spacing = scale + 15.f + 2;
					}
				}
			}
		}
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
				vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());
				if (rot && !targetList.empty()) {
					auto rotation2 = g_Data.getLocalPlayer();
					rotation2->yawUnused1 = angle.y;
					rotation2->pitch = angle.x;
				}
				if (rot && !targetList.empty()) {
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
				if (test && !targetList.empty()) {
					vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());
					auto weewee = g_Data.getLocalPlayer();
					weewee->setRot(angle);
				}
				if (test && !targetList.empty()) {
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
					rotation->bodyYaw = testRot.y;
					rotation->yawUnused2 = prevyaw2;
				}
			}
		}
	}
}

void Killaura::onSendPacket(C_Packet* packet) {
	auto scaffold = moduleMgr->getModule<Scaffold>();
	if (scaffold->useRot) {
		if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr && !rot && silent) {
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

void Killaura::onLevelRender() {
	float t = 0;
	if (GameData::canUseMoveKeys()) {
		if (hold && !g_Data.isLeftClickDown())
			return;
		DrawUtils::setColor(1, 1, 1, 1);
		if (distanceCheck)
			std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());

		if (render && !targetList.empty()) {
			t++;

			vec3_t permutations[56];
			for (int i = 0; i < 56; i++)
				permutations[i] = {sinf((i * 10.f) / (180 / PI)), 0.f, cosf((i * 10.f) / (180 / PI))};

			const float coolAnim = 0.9f + 0.9f * sin((t / 60) * PI * 2);

			if (targetList[0]->damageTime >= 1) {
				vec3_t* start = targetList[0]->getPosOld();
				vec3_t* end = targetList[0]->getPos();

				auto te = DrawUtils::getLerpTime();
				vec3_t pos = start->lerp(end, te);

				auto yPos = pos.y;
				yPos -= 1.62f;
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
	}
}

void Killaura::onDisable() {
	useSprint = true;
	targethud = 0;
	renderStart = 0;
}