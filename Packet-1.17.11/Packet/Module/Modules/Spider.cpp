#include "Spider.h"

Spider::Spider() : IModule(0, Category::MOVEMENT, "Climb walls") {
	registerFloatSetting("Speed", &speed, 0.6f, 0.1f, 1.f);
}

Spider::~Spider() {
}

const char* Spider::getModuleName() {
	return "Spider";
}

void Spider::onMove(C_MoveInputHandler* input) {
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;

	if (player->isInLava() || player->isInWater())
		return;

	if (player->isSneaking())
		return;

	std::vector<vec3_ti> sideBlocks;
	sideBlocks.reserve(8);

	vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.01f;
	if (!pressed)
		return;
	moveVec2d = moveVec2d.normalized();

	float calcYaw = (player->yaw + 90) * (PI / 180);
	vec3_t moveVec;
	float c = cos(calcYaw);
	float s = sin(calcYaw);
	moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};

	for (int x = -1; x <= 1; x++) {
		for (int z = -1; z <= 1; z++) {
			if (x == 0 && z == 0)
				continue;

			if (moveVec2d.dot(vec2_t(x, z).normalized()) < 0.6f)
				continue;
			sideBlocks.push_back(vec3_ti(x, 0, z));
		}
	}

	auto pPos = *player->getPos();
	pPos.y = player->aabb.lower.y;
	auto pPosI = vec3_ti(pPos.floor());

	auto isObstructed = [&](int yOff, AABB* obstructingBlock = nullptr, bool ignoreYcoll = false) {
		for (const auto& current : sideBlocks) {
			vec3_ti side = pPosI.add(0, yOff, 0).add(current);
			if (side.y < 0 || side.y >= 256)
				break;
			auto block = player->region->getBlock(side);
			if (block == nullptr || block->blockLegacy == nullptr)
				continue;
			C_BlockLegacy* blockLegacy = block->toLegacy();
			if (blockLegacy == nullptr)
				continue;
			AABB collisionVec;
			if (!blockLegacy->getCollisionShape(&collisionVec, block, player->region, &side, player))
				continue;
			bool intersects = ignoreYcoll ? collisionVec.intersectsXZ(player->aabb.expandedXZ(0.1f)) : collisionVec.intersects(player->aabb.expandedXZ(0.1f));

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
	float targetSpeed = speed;
	if (stop && (lowerObstructed || upperObstructed)) {
		const auto distanceError = [](float yVel, float distance) {
			int t = 0;
			constexpr int numIter = 60;
			for (; t < numIter; t++) {
				distance -= yVel;
				yVel -= 0.08f;  // gravity
				yVel *= 0.98f;  // drag

				if (yVel <= 0)
					break;
			}
			return std::tuple(distance, yVel, t);
		};

		float getOver = lowerObstructed ? lowerObsVec.upper.y : upperObsVec.upper.y;
		{
			if (upperObstructed)
				getOver = std::max(getOver, upperObsVec.upper.y);

			const int numIterations = (int)ceil(std::max(5.f, -std::get<0>(distanceError(speed, 0)) + 2));

			bool lastObstructed = upperObstructed;
			for (int iter = 2; iter < numIterations; iter++) {
				if (isObstructed(iter, &upperObsVec, true)) {
					getOver = std::max(getOver, upperObsVec.upper.y);
					lastObstructed = true;
				} else {
					if (lastObstructed)
						lastObstructed = false;
					else
						break;
				}
			}
		}

		float targetDist = getOver - pPos.y + 0.02f;
		if (targetDist <= 0)
			return;

		auto [curDist, curYVel, curT] = distanceError(player->velocity.y, targetDist);

		if (curDist <= 0.01f)
			return;

		if (player->velocity.y < speed) {
			auto secondTrajectory = distanceError(speed, targetDist);
			if (std::get<0>(secondTrajectory) <= 0) {

				float error = curDist;
				float startSpeed = player->velocity.y;

				float error2 = std::get<0>(secondTrajectory);
				float startSpeed2 = speed;
				int i = 0;
				for (; i < 16; i++) {
					if (error > -0.05f && error <= 0.001f)
						break;

					float newSpeed = (startSpeed2 * error - startSpeed * error2) / (error - error2);
					startSpeed2 = startSpeed;
					startSpeed = newSpeed;

					error2 = error;
					error = std::get<0>(distanceError(newSpeed, targetDist));
				}
				targetSpeed = startSpeed;
			}
		}
	}
	if (upperObstructed || lowerObstructed) {
		if (player->velocity.y < targetSpeed)
			player->velocity.y = targetSpeed;
	}
}