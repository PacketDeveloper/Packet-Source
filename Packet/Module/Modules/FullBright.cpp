#include "FullBright.h"

FullBright::FullBright() : IModule(0, Category::VISUAL, "Cave vision go brr") {
}

FullBright::~FullBright() {
}

const char* FullBright::getModuleName() {
	return "Fullbright";
}
