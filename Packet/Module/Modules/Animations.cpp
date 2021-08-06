#include "Animations.h"
Animations::Animations() : IModule(0, Category::VISUAL, "Custom item view model") {
	//registerFloatSetting("float1", &this->float1, 0, -10, 10);
	//registerBoolSetting("Translate", &this->doTranslate, this->doTranslate);
	//registerBoolSetting("Scale", &this->doScale, this->doScale);
	registerBoolSetting("Spin", &aroundWorld, aroundWorld);
	registerFloatSetting("X", &xTrans, 0.f, -2.f, 2.f);
	registerFloatSetting("Y", &yTrans, 0.f, -2.f, 2.f);
	registerFloatSetting("Z", &zTrans, 0.f, -2.f, 2.f);
	registerFloatSetting("ScaleX", &xMod, 1.f, 0.f, 2.f);
	registerFloatSetting("ScaleY", &yMod, 1.f, 0.f, 2.f);
	registerFloatSetting("ScaleZ", &zMod, 1.f, 0.f, 2.f);
	registerBoolSetting("Reset", &redo, redo);
	//registerFloatSetting("RotateX", &this->xRotate, 1.f, 0.f, 2.f);
	//registerFloatSetting("RotateY", &this->yRotate, 1.f, 0.f, 2.f);
	//registerFloatSetting("RotateZ", &this->zRotate, 1.f, 0.f, 2.f);
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