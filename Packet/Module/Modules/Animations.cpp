#include <array>
#include <deque>
#include <glm/ext.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/matrix_transform.hpp>  // perspective, translate, rotate
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>  //radians

#include "../../../SDK/MatrixStack.h"
#include "../../../Utils/Logger.h"
#include "../../DrawUtils.h"
#include "Animations.h"
static std::vector<C_Entity> targList;
Animations::Animations() : IModule(0, Category::VISUAL, "Custom item view model") {
	//registerFloatSetting("float1", &float1, 0, -10, 10);
	//registerBoolSetting("Translate", &doTranslate, doTranslate);
	//registerBoolSetting("Scale", &doScale, doScale);
	registerBoolSetting("Spin", &aroundWorld, aroundWorld);
	registerFloatSetting("X", &xTrans, 0.f, -2.f, 2.f);
	registerFloatSetting("Y", &yTrans, 0.f, -2.f, 2.f);
	registerFloatSetting("Z", &zTrans, 0.f, -2.f, 2.f);
	registerFloatSetting("ScaleX", &xMod, 1.f, 0.f, 2.f);
	registerFloatSetting("ScaleY", &yMod, 1.f, 0.f, 2.f);
	registerFloatSetting("ScaleZ", &zMod, 1.f, 0.f, 2.f);
	registerBoolSetting("Reset", &redo, redo);
	//registerFloatSetting("RotateX", &xRotate, 1.f, 0.f, 2.f);
	//registerFloatSetting("RotateY", &yRotate, 1.f, 0.f, 2.f);
	//registerFloatSetting("RotateZ", &zRotate, 1.f, 0.f, 2.f);
}

Animations::~Animations() {
}

void Animations::onTick(C_GameMode* gm) {
	if (redo) {
		auto aniMod = moduleMgr->getModule<Animations>();
		aniMod->redo = false;
			float1 = 0;
			xMod = 1.f;
			yMod = 1.f;
			zMod = 1.f;

			xTrans = 0.f;
			yTrans = 0.f;
			zTrans = 0.f;

			xRotate = 0.f;
			yRotate = 0.f;
			zRotate = 0.f;
		}
}

const char* Animations::getModuleName() {
	return ("Animations");
}