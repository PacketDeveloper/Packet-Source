#include "ChestStealer.h"
#include "../ModuleManager.h"

ChestStealer::ChestStealer() : IModule(0, Category::MISC, "Automatically takes all items out of a chest") {
	registerIntSetting("Close Delay", &setDelay, setDelay, 0, 20);
	registerBoolSetting("enhanced", &enhanced, enhanced);
}

ChestStealer::~ChestStealer() {
}

const char* ChestStealer::getModuleName() {
	return ("ChestStealer");
}

class ContainerScreenController {
public:
	virtual void destructor();
	virtual void games1();
	virtual void games2();
	virtual void games3();
	virtual void games4();
	virtual void games5();
	virtual void games6();
	virtual void games7();
	virtual void games8();
	virtual void games9();
	virtual void games10();
	virtual void games11();
	virtual void games12();
	virtual void games13();
	virtual void games14();
	virtual void games15();
	virtual void games16();
	virtual void games17();
	virtual void games18();
	virtual void games19();
	virtual void games20();
	virtual void games21();
	virtual void games22();
	virtual void games23();
	virtual void games24();
	virtual void games25();
	virtual void games26();
	virtual void games27();
	virtual void games28();
	virtual void games29();
	virtual void games30();
	virtual void games31();
	virtual void games32();
	virtual void games33();
	virtual void games34();
	virtual void games35();
	virtual void games36();
	virtual void games37();
	virtual void games38();
	virtual void games39();
	virtual void games40();
	virtual void games41();
	virtual void games42();
	virtual void games43();
	virtual void games44();
	virtual void games45();
	virtual void games46();
	virtual void games47();
	virtual void games48();
	virtual void games49();
	virtual void games50();
	virtual void games51();
	virtual void games52();
	virtual C_ItemStack* _getItemStack(const TextHolder*, int);
};

void ChestStealer::chestScreenController_tick(C_ChestScreenController* c) {
	ContainerScreenController* ctrler = (ContainerScreenController*)c;
	if (ctrler != nullptr && !g_Data.getLocalPlayer()->canOpenContainerScreen()) {
		std::vector<int> items = {};
		auto invcleanerMod = moduleMgr->getModule<InventoryCleaner>();
		for (int i = 0; i < 54; i++) {
			static TextHolder container_items("container_items");
			C_ItemStack* stack = ctrler->_getItemStack(&container_items, i);
			int igames = 69;
			//if (stack != nullptr && stack->item != NULL)
			//	if (!this->enhanced || invcleanerMod->stackIsUseful(stack))
			//		items.push_back(i);
		}
		return;
		if (!items.empty()) {
			for (int i : items) {
				c->handleAutoPlace(0x7FFFFFFF, "container_items", i);
			}
		} else  {
			delay++;
			if (delay > setDelay) {
				uintptr_t ss = *(uintptr_t*)((uintptr_t)g_Data.getClientInstance() + (((0x116 * 2) + 18) - (6*3)) * 2);
				if (!ss) return;
				uintptr_t thing = *(uintptr_t*)(ss + 8);
				uintptr_t gameing = *(uintptr_t*)(ss + 16);
				if (thing == 0 || gameing == 0 || !((gameing - thing) > 0)) return;

				uintptr_t* us = *(uintptr_t**)(thing + ((((gameing - thing) / 8) - 2) * sizeof(uintptr_t*)));
				uintptr_t** videogames = (uintptr_t**)*us;
				((void (*)(uintptr_t*)) videogames[13])(us);


				//ctrler->leaveScreen(); doesn't leave anymore
				delay = 0;
			}
		}
	}
}
