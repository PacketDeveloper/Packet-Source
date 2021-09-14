#include "PlayerList.h"

PlayerList::PlayerList() : IModule(VK_TAB, Category::CONFIG, "PlayerList") {
}

const char* PlayerList::getModuleName() {
	return "PlayerList";
}

PlayerList::~PlayerList() {
}

static std::vector<C_Entity*> playerList;

void findPlayers(C_Entity* currentEntity, bool isRegularEntity) {
	if (currentEntity == nullptr)
		return;

	if (!Target::isValidTarget(currentEntity))
		return;

	if (currentEntity->getEntityTypeId() != 319)
		return;

	playerList.push_back(currentEntity);
}

bool PlayerList::isFlashMode() {
	return true;
}

void PlayerList::onTick(C_GameMode* gm) {
	playerList.clear();
}

void PlayerList::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	g_Data.forEachEntity(findPlayers);
	for (auto& i : playerList) {
		if (g_Data.isInGame() && i->getEntityTypeId() == 319) {
			float yOffset = 0;
			float textPadding = 0.6f * 1;
			float textHeight = 10.0f * 1;
			float xOffset = windowSize.x - 130;
			vec2_t textPos = vec2_t(
				xOffset + textPadding,
				yOffset + textPadding);

			std::string name = i->getNameTag()->getText();
			std::string* realName = &name;
			float titleWidth = DrawUtils::getTextWidth(&name);

			//vec2_t textPos = vec2_t(windowSize.x / 2.f - titleWidth / 2.f, windowSize.y / 50.f);

			DrawUtils::drawText(textPos, &name, MC_Color(255, 255, 255), 1, 1, true);
			yOffset += textHeight + (textPadding * 2);
		}
	}
}