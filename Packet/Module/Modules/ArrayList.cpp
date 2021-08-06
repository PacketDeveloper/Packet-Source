#include "ArrayList.h"

#include "../../DrawUtils.h"
#include "../../Scripting/ScriptManager.h"

ArrayList::ArrayList() : IModule(0, Category::VISUAL, "Displays ArrayList") {
	registerEnumSetting("Mode", &this->mode, 0);
	mode.addEntry("Outline", 0);
	mode.addEntry("Split", 1);
	mode.addEntry("Bar", 2);
	mode.addEntry("None", 3);
	registerBoolSetting("Keybinds", &this->keybinds, this->keybinds);
	this->registerFloatSetting("Opacity", &this->bgOp, this->bgOp, 0.f, 1.f);
}

ArrayList::~ArrayList() {
}

const char* ArrayList::getModuleName() {
	return ("ArrayList");
}