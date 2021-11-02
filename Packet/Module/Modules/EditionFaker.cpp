#include "EditionFaker.h"

EditionFaker::EditionFaker() : IModule(0, Category::EXPLOIT, "Fakes your device to IOS") {
}

EditionFaker::~EditionFaker() {
}

int EditionFaker::getFakedEditon() {
	static const char* gameEditons[] = {"iOS"};

	return 2;  // Ios
}

const char* EditionFaker::getModuleName() {
	return ("EditionFaker");
}
