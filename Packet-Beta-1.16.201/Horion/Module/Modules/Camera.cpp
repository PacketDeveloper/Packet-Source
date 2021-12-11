#include "Camera.h"

Camera::Camera() : IModule(0, Category::VISUAL, "DeiDeeDeiDerrtDord") {
	this->registerBoolSetting("NoHurtCam", &this->nohurtMode, this->nohurtMode);
	this->registerBoolSetting("Fullbright", &this->fullbrightMode, this->fullbrightMode);
}

const char* Camera::getModuleName() {
	return "Camera";
}