#include "TargetStrafe.h"

TargetStrafe::TargetStrafe() : IModule(0, Category::MOVEMENT, "Strafe around the target") {
	this->registerBoolSetting("CircleRender", &this->circleRender, this->circleRender);
	this->registerBoolSetting("EdgeCheck", &this->avoidvoid, this->avoidvoid);
	this->registerBoolSetting("OnKey", &this->onKey, this->onKey);
	this->registerFloatSetting("Distance", &this->StrafeDistance, this->StrafeDistance, 1.f, 10.f);
	this->registerFloatSetting("Speed", &this->speedMod, this->speedMod, 0.2f, 5.f);
}

TargetStrafe::~TargetStrafe() {
}

const char* TargetStrafe::getModuleName() {
	return ("TargetStrafe");
}

void TargetStrafe::onMove(C_MoveInputHandler* input) {
}

static std::vector<C_Entity*> targetList69;

void findEntityTS(C_Entity* currentEntity, bool isRegularEntity) {
	static auto targetstrafeMod = moduleMgr->getModule<TargetStrafe>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < targetstrafeMod->range) {
		targetList69.push_back(currentEntity);
	}
}

vec2_t getAngles3(vec3_t PlayerPosition, vec3_t EntityPosition) {
	vec2_t Angles;
	float dX = PlayerPosition.x - EntityPosition.x;
	float dY = PlayerPosition.y - EntityPosition.y;
	float dZ = PlayerPosition.z - EntityPosition.z;
	double distance = sqrt(dX * dX + dY * dY + dZ * dZ);
	Angles.x = (float)(atan2(dY, distance) * 180.0f / PI);
	Angles.y = (float)(atan2(dZ, dX) * 180.0f / PI) + 90.0f;
	return Angles;
};
struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void TargetStrafe::onTick(C_GameMode* gm) {
	auto speed = moduleMgr->getModule<Speed>();
	targetList69.clear();
	g_Data.forEachEntity(findEntityTS);
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	if (input == nullptr) return;
	if (initRender) {
		renderTimer++;
		if (renderTimer >= 200) renderTimer = 200;
	}

	if (collide) {
		std::vector<vec3_ti> sideBlocks;
		sideBlocks.reserve(8);

		float calcYaw = (gm->player->yaw + 90) * (PI / 180);
		vec3_t moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);

		for (int x = -1; x <= 1; x++) {
			for (int z = -1; z <= 1; z++) {
				if (x == 0 && z == 0)
					continue;

				sideBlocks.push_back(vec3_ti(x, 0, z));
			}
		}

		auto pPos = *gm->player->getPos();
		pPos.y = gm->player->aabb.lower.y;
		auto pPosI = vec3_ti(pPos.floor());

		auto isObstructed = [&](int yOff, AABB* obstructingBlock = nullptr, bool ignoreYcoll = false) {
			for (const auto& current : sideBlocks) {
				vec3_ti side = pPosI.add(0, yOff, 0).add(current);
				if (side.y < 0 || side.y >= 256)
					break;
				auto block = gm->player->region->getBlock(side);
				if (block == nullptr || block->blockLegacy == nullptr)
					continue;
				C_BlockLegacy* blockLegacy = block->toLegacy();
				if (blockLegacy == nullptr)
					continue;
				AABB collisionVec;
				if (!blockLegacy->getCollisionShape(&collisionVec, block, gm->player->region, &side, gm->player))
					continue;
				bool intersects = ignoreYcoll ? collisionVec.intersectsXZ(gm->player->aabb.expandedXZ(0.3f)) : collisionVec.intersects(gm->player->aabb.expandedXZ(0.3f));

				if (intersects) {
					if (obstructingBlock != nullptr)
						*obstructingBlock = collisionVec;
					return true;
				}
			}
			return false;
		};

		AABB lowerObsVec, upperObsVec;
		bool upperObstructed = isObstructed(1, &upperObsVec);

		bool lowerObstructed = isObstructed(0, &lowerObsVec);

		if (upperObstructed || lowerObstructed) {
			intersectingTimer++;
		}
		if ((upperObstructed || lowerObstructed)) {
			if (clockwise && intersectingTimer >= 1) {
				//clientMessageF("changed");
				intersectingTimer = 0;
				clockwise = false;
			}
			if (!clockwise && intersectingTimer >= 1) {
				clockwise = true;
				//clientMessageF("changed-2");
				intersectingTimer = 0;
			}
		}
	}

	for (auto& i : targetList69) {
		if (!targetList69.empty()) {
			std::sort(targetList69.begin(), targetList69.end(), CompareTargetEnArray());
			vec2_t angle2 = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList69[0]->getPos());
			vec2_t angle = getAngles3(*gm->player->getPos(), *targetList69[0]->getPos());
			C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
			if (jump && gm->player->onGround) {
				gm->player->jumpFromGround();
			}

			if (testMode) {
				float distance = 99;
				float distanc = 999;
				vec3_t myPos = *gm->player->getPos();
				vec3_t EntPos = *i->getPos();
				if (distanc > distance) {
					distance = i->getPos()->dist(myPos);
					distanc = distance;
				}
				if (GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
					clockwise = false;
				} else if (GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
					clockwise = true;
				}
				C_LocalPlayer* player = g_Data.getLocalPlayer();

				vec2_t CalcRot = getAngles3(*player->getPos(), EntPos);
				if (clockwise) {
					CalcRot.y += 90.0f;
					if (distanc > StrafeDistance) CalcRot.y -= 45.0f;
				} else {
					CalcRot.y -= 90.0f;
					if (distanc > StrafeDistance) CalcRot.y += 45.0f;
				}

				if (avoidvoid) {
					bool onvoid = false;
					if (player->region->getBlock(vec3_t{myPos.x, myPos.y - 3, myPos.z})->toLegacy()->blockId == 0) {
						onvoid = true;
						intersectingTimer2++;
					}
					if (onvoid) {
						if (clockwise && intersectingTimer2 >= 5) {
							clockwise = false;
							intersectingTimer2 = 0;
						}
						if (!clockwise && intersectingTimer2 >= 5) {
							clockwise = true;
							intersectingTimer2 = 0;
						}
					}
				}
				if (g_Data.canUseMoveKeys()) {
					/*if (onKey) {
						if (GameData::isKeyDown(*input->spaceBarKey)) {
							vec2_t CalcAngles = vec2_t((CalcRot.x) * -(PI / 180.f), (CalcRot.y + 90.0f) * (PI / 180.f));
							player->velocity = vec3_t(cos(CalcAngles.y) * cos(CalcAngles.x) * speedMod, player->velocity.y, sin(CalcAngles.y) * cos(CalcAngles.x) * speedMod);
							if (player->onGround) player->jumpFromGround();
						}
					} else {
						//if (speed->isEnabled()) {
							vec2_t CalcAngles = vec2_t((CalcRot.x) * -(PI / 180.f), (CalcRot.y + 90.0f) * (PI / 180.f));
							player->velocity = vec3_t(cos(CalcAngles.y) * cos(CalcAngles.x) * speedMod, player->velocity.y, sin(CalcAngles.y) * cos(CalcAngles.x) * speedMod);
						//}
					}*/
					if (!speedCheck) {
						if (spacekeyMode)
							if (GameData::isKeyDown(*input->spaceBarKey)) {
								vec2_t CalcAngles = vec2_t((CalcRot.x) * -(PI / 180.f), (CalcRot.y + 90.0f) * (PI / 180.f));
								player->velocity = vec3_t(cos(CalcAngles.y) * cos(CalcAngles.x) * speedMod, player->velocity.y, sin(CalcAngles.y) * cos(CalcAngles.x) * speedMod);
							}
					}
					if (!speedCheck) {
						if (!spacekeyMode) {
							vec2_t CalcAngles = vec2_t((CalcRot.x) * -(PI / 180.f), (CalcRot.y + 90.0f) * (PI / 180.f));
							player->velocity = vec3_t(cos(CalcAngles.y) * cos(CalcAngles.x) * speedMod, player->velocity.y, sin(CalcAngles.y) * cos(CalcAngles.x) * speedMod);
						}
					}
					if (spacekeyMode) {
						auto bhopMod = moduleMgr->getModule<Speed>();
						if (speedCheck)
							if (bhopMod->isEnabled()) {
								if (GameData::isKeyDown(*input->spaceBarKey)) {
									vec2_t CalcAngles = vec2_t((CalcRot.x) * -(PI / 180.f), (CalcRot.y + 90.0f) * (PI / 180.f));
									player->velocity = vec3_t(cos(CalcAngles.y) * cos(CalcAngles.x) * speedMod, player->velocity.y, sin(CalcAngles.y) * cos(CalcAngles.x) * speedMod);
								}
							}
					}

					if (!spacekeyMode) {
						auto bhopMod = moduleMgr->getModule<Speed>();
						if (speedCheck)
							if (bhopMod->isEnabled()) {
								vec2_t CalcAngles = vec2_t((CalcRot.x) * -(PI / 180.f), (CalcRot.y + 90.0f) * (PI / 180.f));
								player->velocity = vec3_t(cos(CalcAngles.y) * cos(CalcAngles.x) * speedMod, player->velocity.y, sin(CalcAngles.y) * cos(CalcAngles.x) * speedMod);
							}
					}
				}
			}
		}
	}
}

void TargetStrafe::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		this->setEnabled(false);
	initRender = true;
}

void TargetStrafe::onDisable() {
	initRender = false;
	renderTimer = 0;
}

void TargetStrafe::onSendPacket(C_Packet* packet) {
}

float tt = 0;
void TargetStrafe::onLevelRender() {
	if (renderTimer >= 4 && circleRender && !targetList69.empty()) {
		std::sort(targetList69.begin(), targetList69.end(), CompareTargetEnArray());
		tt++;
		DrawUtils::setColor(1, 1, 1, 0.9f);

		vec3_t permutations[56];
		for (int i = 0; i < 56; i++) {
			permutations[i] = {sinf((i * 10.f) / (180 / PI)), 0.f, cosf((i * 10.f) / (180 / PI))};
		}

		const float coolAnim = 0.9f + 0.9f * sin((tt / 60) * PI * 2);

		vec3_t* start = targetList69[0]->getPosOld();
		vec3_t* end = targetList69[0]->getPos();

		auto te = DrawUtils::getLerpTime();
		vec3_t pos = start->lerp(end, te);

		auto yPos = pos.y;
		yPos -= 1.6f;
		//yPos += coolAnim;

		std::vector<vec3_t> posList;
		posList.reserve(56);
		for (auto& perm : permutations) {
			vec3_t curPos(pos.x, yPos, pos.z);
			posList.push_back(curPos.add(perm));
		}

		DrawUtils::drawLinestrip3d(posList);
	}
}

void TargetStrafe::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
}

void TargetStrafe::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
}