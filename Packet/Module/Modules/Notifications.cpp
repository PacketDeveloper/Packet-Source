#include "Notifications.h"

Notifications::Notifications() : IModule(0, Category::VISUAL, "Notifications") {
	//registerBoolSetting("Disable", &disable, &disable);
	//registerBoolSetting("Enable", &enable, &enable);
	registerFloatSetting("Opacity", &opacity, opacity, 0.f, 1.f);
}

Notifications::~Notifications() {
}

void Notifications::onEnable() {
	auto box = g_Data.addInfoBox("Module:", "Example Notification");
	box->closeTimer = 13; // 13
	setEnabled(false);
}

const char* Notifications::getModuleName() {
	return ("Notifications");
}