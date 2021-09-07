#include "ChestStealer.h"

#include "../ModuleManager.h"

ChestStealer::ChestStealer() : IModule(0, Category::PLAYER, "Automatically takes all items out of a chest") {
	registerIntSetting("Close Delay", &setDelay, setDelay, -20, 20);
	registerBoolSetting("enhanced", &enhanced, enhanced);
	registerBoolSetting("AutoDisable", &autoDisable, autoDisable);
}

ChestStealer::~ChestStealer() {
}

const char* ChestStealer::getModuleName() {
	return ("ChestStealer");
}

void ChestStealer::onTick(C_GameMode* gm) {

}

void ChestStealer::chestScreenController_tick(C_ChestScreenController* c) {
	if (c != nullptr && !g_Data.getLocalPlayer()->canOpenContainerScreen()) {
		std::vector<int> items = {};
		auto invcleanerMod = moduleMgr->getModule<InvManager>();
		for (int i = 0; i < 54; i++) {
			C_ItemStack* stack = c->_getItemStack(TextHolder("container_items"), i);
			if (stack != nullptr && stack->item != NULL)
				if (!enhanced || invcleanerMod->stackIsUseful(stack))
					items.push_back(i);
		}
		if (!items.empty()) {
			for (int i : items) {
				c->handleAutoPlace(0x7FFFFFFF, "container_items", i);
			}
		} else {
			delay++;
			if (delay > setDelay) {
				c->tryExit();
				delay = 0;
			}
		}
	}
}