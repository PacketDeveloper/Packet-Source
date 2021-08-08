#include "TPAura.h"

TPAura::TPAura() : IModule(0x0, Category::COMBAT, "TP Into The Closest Entity") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Multi", 0);
	mode.addEntry("Switch", 1);
	registerBoolSetting("Silent", &silent, silent);
	registerBoolSetting("Push", &push, push);
	registerIntSetting("TP Delay", &delay, delay, 0, 10);
	registerFloatSetting("Range", &range, range, 5, 250);
}

TPAura::~TPAura() {
}

const char* TPAura::getModuleName() {
	return ("TPAura");
}

static std::vector<C_Entity*> targetList0;
static std::vector<C_Entity*> targetList;

void TPAura::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		setEnabled(false);
	if (mode.getSelectedValue() == 1) {  // Switch
		cCounter = 1;
	}
}

void findEntity1(C_Entity* currentEntity, bool isRegularEntity) {
	static auto tpaura = moduleMgr->getModule<TPAura>();

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

	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < tpaura->range) {
		targetList.push_back(currentEntity);
	}
}

void findEntities(C_Entity* currentEntity, bool isRegularEntitie) {
	static auto tpaura = moduleMgr->getModule<TPAura>();

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (currentEntity == 0)
		return;

	if (currentEntity->timeSinceDeath > 0 || currentEntity->damageTime >= 7)
		return;

	if (FriendList::findPlayer(currentEntity->getNameTag()->getText()))  // Skip Friend
		return;

	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < tpaura->range) {
		targetList0.push_back(currentEntity);
	}
}

void TPAura::onTick(C_GameMode* gm) {
	//Loop through all our players and retrieve their information
	targetList0.clear();
	targetList.clear();

	g_Data.forEachEntity(findEntities);
	g_Data.forEachEntity(findEntity1);

	float calcYaw = (gm->player->yaw + 90) * (PI / 180);
	float calcPitch = (gm->player->pitch) * -(PI / 180);
	float teleportX = cos(calcYaw) * cos(calcPitch) * 3.5f;
	float teleportZ = sin(calcYaw) * cos(calcPitch) * 3.5f;
	//vec3_t pos = *targetList0[0]->getPos();
	C_MovePlayerPacket teleportPacket;

	Odelay++;
	if (!targetList.empty() && Odelay >= delay) {
		vec3_t pos = *targetList[0]->getPos();
		if (!silent) {
			if (!push) {
				C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
				C_LocalPlayer* player = g_Data.getLocalPlayer();
				player->setPos(pos);
			} else {
				float dist2 = gm->player->getPos()->dist(pos);
				g_Data.getLocalPlayer()->lerpTo(pos, vec2_t(1, 1), (int)fmax((int)dist2 * 0.1, 1));
			}
		}
		// Attack all entitys in targetList
		if (mode.getSelectedValue() == 0) {  // Multi
			if (silent) {
				if (targetList0.size() > 0 && Odelay >= delay) {
					if (!moduleMgr->getModule<NoSwing>()->isEnabled())
						g_Data.getLocalPlayer()->swingArm();
					for (int i = 0; i < targetList0.size(); i++) {
						vec3_t pos = *targetList0[i]->getPos();
						teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), vec3_t(pos.x - teleportX, pos.y, pos.z - teleportZ));
						g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
						g_Data.getCGameMode()->attack(targetList0[i]);
						teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
						g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
					}
				}
			} else {
				for (auto& i : targetList) {
					if (!(i->damageTime > 1 && hurttime)) {
						g_Data.getLocalPlayer()->swing();
						g_Data.getCGameMode()->attack(i);
						targethud++;
					} else
						targethud = 0;
				}
			}
		} else {  // Switch
			if (silent) {
				if (targetList0.size() > 0 && Odelay >= delay) {
					if (!moduleMgr->getModule<NoSwing>()->isEnabled())
						g_Data.getLocalPlayer()->swingArm();
					teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), vec3_t(pos.x - teleportX, pos.y, pos.z - teleportZ));
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
					g_Data.getCGameMode()->attack(targetList0[0]);
					teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
				}
			} else {
				if (!(targetList[0]->damageTime > 1 && hurttime)) {
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(targetList[0]);
					targethud++;
				} else
					targethud = 0;
			}
		}
		Odelay = 0;
	}
}

void TPAura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (targetList.size() == 1 && mode.getSelectedValue() != 1) {
		if (targethud > 1 && g_Data.canUseMoveKeys()) {
			for (auto& i : targetList) {
				C_GuiData* dat = g_Data.getClientInstance()->getGuiData();
				vec2_t windowSize = dat->windowSize;
				std::string text = targetList[0]->getNameTag()->getText();
				text = Utils::sanitize(text);
				std::string realname = "Name: " + text;
				int dist2 = (int)(*targetList[0]->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
				auto dist = std::to_string(dist2);
				auto distancestring = std::string("Distance: " + dist);
				vec4_t duotagteam = (vec4_t(windowSize.x / 1.5f - (windowSize.x / 7),
											windowSize.y / 1.61f - (windowSize.y / 13),
											windowSize.x / 1.7f + (windowSize.x / 9 + targetList[0]->getNameTag()->textLength),
											windowSize.y / 2 - windowSize.y / 8 + windowSize.y / 4));
				DrawUtils::fillRectangle(vec4_t(duotagteam),
										 MC_Color(0.05f, 0.05f, 0.05f), 0.35f);
				DrawUtils::drawRectangle(vec4_t(duotagteam),
										 MC_Color(1.f, 1.f, 1.f), 1.f);

				DrawUtils::drawText(vec2_t(windowSize.x / 1.5f - windowSize.x / 7.25f,
										   windowSize.y / 2 - windowSize.y / 5.f + windowSize.y / 4),
									&realname,
									MC_Color(1.f, 1.f, 1.f), 1.f);

				DrawUtils::drawText(vec2_t(windowSize.x / 1.5f - windowSize.x / 7.25f,
										   windowSize.y / 2 - windowSize.y / 5.8f + windowSize.y / 4),
									&distancestring,
									MC_Color(1.f, 1.f, 1.f), 1.f);
				DrawUtils::flush();
				vec2_t textPos;
				vec4_t rectPos;
				auto* player = reinterpret_cast<C_Player*>(targetList[0]);
				float x = windowSize.x / 1.5f - windowSize.x / 7.1f;
				float y = windowSize.y / 2 - windowSize.y / 6.4f + windowSize.y / 4;
				float scale = 3 * 0.26f;
				float spacing = scale + 15.f + 2;
				if (i->getEntityTypeId() == 63) {
					// armor
					for (int i = 0; i < 4; i++) {
						C_ItemStack* stack = player->getArmor(i);
						if (stack->item != nullptr) {
							DrawUtils::drawItem(stack, vec2_t(x, y), 1.f, scale, stack->isEnchanted());
							x += scale * spacing;
						}
					}
					// item
					{
						C_ItemStack* stack = player->getSelectedItem();
						if (stack->item != nullptr) {
							DrawUtils::drawItem(stack, vec2_t(x, y), 1.f, scale, stack->isEnchanted());
						}
					}
				}
			}
		}
	}
}

void TPAura::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	for (auto& i : targetList) {
		if (!targetList.empty()) {
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());
			auto rotation = g_Data.getLocalPlayer();
			float prevyaw = rotation->yawUnused1;
			float prevyaw2 = rotation->yaw;
			float prevyaw3 = rotation->yaw2;
			rotation->setRot(angle);

			// Head
			rotation->yawUnused1 = angle.y;
			rotation->pitch = angle.x;
			rotation->yaw2 = angle.y;
			rotation->yaw = prevyaw2;
			rotation->pitch2 = angle.x;

			// Body
			rotation->bodyYaw = angle.y;
			rotation->yawUnused2 = prevyaw2;
		}
	}
}

void TPAura::onSendPacket(C_Packet* packet) {
	for (auto& i : targetList) {
		if (!targetList.empty()) {
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());
			auto rotation = g_Data.getLocalPlayer();
			float prevyaw = rotation->yawUnused1;
			float prevyaw2 = rotation->yaw;
			float prevyaw3 = rotation->yaw2;
			rotation->setRot(angle);

			// Head
			rotation->yawUnused1 = angle.y;
			rotation->pitch = angle.x;
			rotation->yaw2 = angle.y;
			rotation->yaw = prevyaw2;
			rotation->pitch2 = angle.x;

			// Body
			rotation->bodyYaw = angle.y;
			rotation->yawUnused2 = prevyaw2;
		}
	}
}