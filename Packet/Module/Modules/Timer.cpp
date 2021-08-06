#include "Timer.h"

Timer::Timer() : IModule(0, Category::MISC, "Modifies game speed") {
	registerIntSetting("TPS", &timer, timer, 5, 700);
}

Timer::~Timer() {
}

const char* Timer::getModuleName() {
	return ("Timer");
}

void Timer::onTick(C_GameMode* gm) {
	if (timer) {
		*g_Data.getClientInstance()->minecraft->timer = static_cast<float>(timer);
	}
}

void Timer::onDisable() {
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
}