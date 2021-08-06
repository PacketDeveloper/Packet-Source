#include "EditionFaker.h"

EditionFaker::EditionFaker() : IModule(0, Category::MISC, "Fakes your device to android") {
	registerIntSetting("Version", &vers, vers, 0, 11);
}

EditionFaker::~EditionFaker() {
}

int EditionFaker::getFakedEditon() {
	//static const char* gameEditons[] = {"Android"}; //incase u still need this
	// static const char* gameEditons[] = { "Unknown", "Android", "iOS", "macOS", "FireOS", "GearVR", "HoloLens", "Windows 10 (PC)", "Windows", "Dedicated", "Orbis", "NX" };

	return vers;  // ios
}

const char* EditionFaker::getModuleName() {
	return ("EditionFaker");
}
