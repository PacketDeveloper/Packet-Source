#include "Jetpack.h"

Jetpack::Jetpack() : IModule(0, Category::MOVEMENT, "Duck") {
	registerFloatSetting("speed", &speedMod, 1, 0.2f, 5.f);
	//registerBoolSetting("Bypass", &isBypass, isBypass);
}

Jetpack::~Jetpack() {
}

bool Jetpack::isFlashMode() {
	return true;
}

const char* Jetpack::getModuleName() {
	return ("Jetpack");
}

void Jetpack::onTick(C_GameMode* gm) {
	float calcYaw = (gm->player->yaw + 90) * (PI / 180);
	float calcPitch = (gm->player->pitch) * -(PI / 180);
	float calcYawInvert = (gm->player->yaw - 90) * (PI / 180);

	if (!isBypass) {
		vec3_t moveVec;
		moveVec.x = cos(calcYaw) * cos(calcPitch) * speedMod;
		moveVec.y = sin(calcPitch) * speedMod;
		moveVec.z = sin(calcYaw) * cos(calcPitch) * speedMod;

		gm->player->lerpMotion(moveVec);
	} else {
		delay++;
		//funni
		if (delay >= 5) {
			vec3_t moveVec;
			moveVec.x = cos(calcYawInvert) * cos(calcPitch) * speedMod;
			moveVec.y = sin(calcPitch) * speedMod;
			moveVec.z = sin(calcYawInvert) * cos(calcPitch) * speedMod;
		}
	}
}
