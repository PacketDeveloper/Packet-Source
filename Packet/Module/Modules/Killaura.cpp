#include "Killaura.h"

	Killaura::Killaura() : IModule(0, Category::COMBAT, "Automatically attacks entites") {
		registerEnumSetting("Rotations", &mode, 0);
		mode.addEntry("Normal", 0);
		mode.addEntry("Chronos", 1);
		mode.addEntry("Old", 2);
		mode.addEntry("Silent", 3);
		registerBoolSetting("ShowTarget", &render, render);
		registerBoolSetting("Rotations", &rotations, rotations);
		registerBoolSetting("MultiAura", &multi, multi);
		registerBoolSetting("Distance", &distanceCheck, distanceCheck);
		registerBoolSetting("MobAura", &mobAura, mobAura);
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

	static std::vector<C_Entity*> targetList;

	void findEntity(C_Entity * currentEntity, bool isRegularEntity) {
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
		targetListEmpty = true;
		targetList.empty();
		if (render) {
			renderStart;
		}
	}

	void Killaura::onTick(C_GameMode * gm) {
		auto clickGUI = moduleMgr->getModule<ClickGuiMod>();
		auto scaffold = moduleMgr->getModule<Scaffold>();
		targetListEmpty = targetList.empty();

		if (clickGUI->isEnabled()) {
			targetListEmpty = true;
		}

		targetList.clear();

		g_Data.forEachEntity(findEntity);

		if (renderStart >= 1)
			renderStart++;
		if (renderStart >= 5)
			renderStart = 5;

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
				//fake auto block using texture pack
				if (test) {
					if (targetList.empty()) {
						targethud = 0;
						useSprint = true;
					} else {
						gm->player->setSprinting(false);
						useSprint = false;
					}
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


						if (rotations && mode.getSelectedValue() == 0 || mode.getSelectedValue() == 1 && !targetList.empty()) {
							vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());
							auto rotation = g_Data.getLocalPlayer();
							rotation->setRot(angle);

							vec2_t testRot = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());
							rotation->setRot(testRot);

							float prevyaw2 = rotation->yaw;

							// Head
							rotation->yawUnused1 = testRot.y;
							rotation->pitch = testRot.x;
							rotation->yaw2 = testRot.y;
							rotation->yaw = prevyaw2;
							rotation->pitch2 = testRot.x;
							rotation->viewAngles;

							// Body
							if (mode.getSelectedValue() == 0) {
								rotation->bodyYaw = testRot.y;
							}
						}
						if (rotations && mode.getSelectedValue() == 2) {
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
	}

	void Killaura::onPreRender(C_MinecraftUIRenderContext * renderCtx) {
		if (GameData::canUseMoveKeys()) {
			if (hold && !g_Data.isLeftClickDown())
				return;
			auto scaffold = moduleMgr->getModule<Scaffold>();
			if (render && scaffold->useRot) {
			}
		}
	}

	void Killaura::onPostRender(C_MinecraftUIRenderContext * renderCtx) {
		if (GameData::canUseMoveKeys()) {
			if (hold && !g_Data.isLeftClickDown())
				return;
			auto scaffold = moduleMgr->getModule<Scaffold>();
			auto player = g_Data.getLocalPlayer();
			if (scaffold->useRot) {
				for (auto& i : targetList) {
					if (g_Data.getLocalPlayer() == nullptr)
						return;
					
				}
			}
		}
	}

	void Killaura::onSendPacket(C_Packet * packet) {
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

	void Killaura::onLevelRender() {
		if (GameData::canUseMoveKeys()) {
			if (hold && !g_Data.isLeftClickDown())
				return;
			float t = 0;
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