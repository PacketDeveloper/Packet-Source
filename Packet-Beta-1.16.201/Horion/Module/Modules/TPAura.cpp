#include "TPAura.h"

TPAura::TPAura() : IModule(0, Category::COMBAT, "TPS to the closest entity") {
	this->registerBoolSetting("Push", &this->push, this->push);
	this->registerIntSetting("TP Delay", &this->delay, this->delay, 0, 10);
	this->registerFloatSetting("Range", &this->range, this->range, 1.f, 250.f);
}

const char* TPAura::getModuleName() {
	return ("TPAura");
}

TPAura::~TPAura() {
}

void TPAura::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		this->setEnabled(false);
}

static std::vector<C_Entity*> targetList;

void findEntity1(C_Entity* currentEntity, bool isRegularEntity) {
	static auto tpauraMod = moduleMgr->getModule<TPAura>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (tpauraMod->enabled) {
		if (!Target::isValidTarget(currentEntity))
			return;
	}

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (tpauraMod->enabled) {
		if (dist < tpauraMod->range) {
			targetList.push_back(currentEntity);
		}
	}
}

void findEntityPlayer(C_Entity* currentEntity, bool isRegularEntity) { // Attack
	static auto tpauraMod = moduleMgr->getModule<TPAura>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (tpauraMod->isMobAura) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
			return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f)  // Don't hit this pesky antibot on 2b2e.org
			return;
		if (currentEntity->getEntityTypeId() == 64)  // item
			return;
	} else {
		if (!Target::isValidTarget(currentEntity))
			return;
	}

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < tpauraMod->range) {
		targetList.push_back(currentEntity);
	}
}

struct CompareTargetEnArray { // Attack
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void TPAura::onTick(C_GameMode* gm) {
	//Loop through all our players and retrieve their information
	targetList.clear();

	g_Data.forEachEntity(findEntity1);

	Odelay++;
	if (!targetList.empty() && Odelay >= delay) {
		vec3_t pos = *targetList[0]->getPos();
		if (!push) {
			//if (g_Data.getLocalPlayer()->velocity.squaredxzlen() < 0.01) {
			C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
			//}
			C_LocalPlayer* player = g_Data.getLocalPlayer();
			player->setPos(pos);
		} else {
			float dist2 = gm->player->getPos()->dist(pos);
			g_Data.getLocalPlayer()->lerpTo(pos, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
		}
		Odelay = 0;
	}
	// Attack
	targetList.clear();

	g_Data.forEachEntity(findEntityPlayer);

	Odelay++;
	if (distanceCheck)
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
	if (!targetList.empty() && Odelay >= delay) {
		if (autoweapon) findWeapon();
		if (g_Data.getLocalPlayer()->velocity.squaredxzlen() < 0.01) {
			C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);  // make sure to update rotation if player is standing still
		}

		// Attack all entitys in targetList
		if (isMulti) {
			for (auto& i : targetList) {
				if (!(i->damageTime > 1 && hurttime)) {
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(i);
					targethud++;
				} else
					targethud = 0;
			}
		} else {
			if (!(targetList[0]->damageTime > 1 && hurttime)) {
				g_Data.getLocalPlayer()->swing();
				g_Data.getCGameMode()->attack(targetList[0]);
				targethud++;
			} else
				targethud = 0;
		}
		Odelay = 0;
	}

	for (auto& i : targetList) {
		if (!targetList.empty()) {
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());
			auto weewee2 = g_Data.getLocalPlayer();
			if (weewee2->pitch != 0 && weewee2->yaw != 0) {
				weewee2->yawUnused1 = angle.y;
				weewee2->pitch = angle.x;
				weewee2->bodyYaw = angle.y;
				weewee2->yawUnused2 = angle.y;
				weewee2->yaw2 = angle.y;
				weewee2->pitch2 = angle.x;
				//weewee2->setRot(angle);
			}
		}
	}
}

void TPAura::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr && silent) {
		if (!targetList.empty()) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			movePacket->pitch = angle.x;
			movePacket->headYaw = angle.y;
			movePacket->yaw = angle.y;
		}
	}
}

void TPAura::findWeapon() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage) {
				damage = currentDamage;
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}