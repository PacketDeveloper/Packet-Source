#include "HudModule.h"

#include "../../DrawUtils.h"
#include "../../Scripting/ScriptManager.h"

HudModule::HudModule() : IModule(0, Category::VISUAL, "Displays Hud") {
	registerEnumSetting("Color", &color, 0);  // get the value using color.getSelectedValue()
	color.addEntry("Rainbow", 0);
	color.addEntry("Dynamic", 1);
	color.addEntry("Horion", 2);
	color.addEntry("Fadeaway", 3);
	color.addEntry("White", 4);
	color.addEntry("Red", 5);
	color.addEntry("Orange", 6);
	color.addEntry("Yellow", 7);
	color.addEntry("Green", 8);
	color.addEntry("Blue", 9);
	//color.addEntry("Cyan", 10);
	color.addEntry("Purple", 11);
	color.addEntry("Pink", 12);
	registerBoolSetting("TextShadow", &this->textShadow, this->textShadow);
	registerBoolSetting("Keystrokes", &this->keystrokes, this->keystrokes);
	registerBoolSetting("Watermark", &this->watermark, this->watermark);
	registerBoolSetting("ArmorHUD", &this->displayArmor, this->displayArmor);
	registerBoolSetting("FPS", &this->fps, this->fps);
	//registerBoolSetting("CPS", &this->cps, this->cps);
	//registerBoolSetting("coords", &this->coordinates, this->coordinates);
	//registerBoolSetting("BPS", &this->bps, this->bps);
	//registerBoolSetting("Always show", &this->alwaysShow, this->alwaysShow);
}

HudModule::~HudModule() {
}

const char* HudModule::getModuleName() {
	return ("Hud");
}

void HudModule::onTick(C_GameMode* gm) {
}

void HudModule::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
}

void HudModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	float f = 10.f * this->scale;
	std::string tempStr("Movement");
	float len = DrawUtils::getTextWidth(&tempStr, scale) + 7.f;
	float startY = tabgui ? 6 * f : 0.f;
	if (tabgui && scriptMgr.getNumEnabledScripts() > 0)
		startY += f;

/*{  // FPS
		if (g_Data.canUseMoveKeys()) {
			if (!(g_Data.getLocalPlayer() == nullptr || !this->fps)) {
				std::string fpsText = "FPS: " + std::to_string(g_Data.getFPS());
				float x = windowSize.x / 30.f + -16.f;  // dont question this
				float y = windowSize.y - 12.1f;         // 12.1 so it is exactly 16 pixels away from the edge lol
				DrawUtils::drawText(vec2_t(x, y), &fpsText, MC_Color(255, 255, 255), scale);

				startY *= f;
			}
		}
	}*/

	{  // CPS
		if (!(g_Data.getLocalPlayer() == nullptr || !this->cps)) {
			std::string cpsText = "CPS: " + std::to_string(g_Data.getLeftCPS());
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
			vec2_t textPos = vec2_t(rectPos.x * cpspos2, rectPos.y * cpspos);  //CPS Location
			DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), 0.f);
			DrawUtils::drawText(textPos, &cpsText, MC_Color(255, 255, 255), scale);
		}
	}

	{  // Coordinates
		if (!(g_Data.getLocalPlayer() == nullptr || !this->coordinates)) {
			vec3_t* pos = g_Data.getLocalPlayer()->getPos();

			std::string position = "Position: " + std::to_string((int)floorf(pos->x)) + " " + std::to_string((int)floorf(pos->y)) + " " + std::to_string((int)floorf(pos->z)); // amazing code
			float x = windowSize.x / 30.f + -16.f;
			float y = windowSize.y - 12.1f;
			DrawUtils::drawText(vec2_t(x, y), &position, MC_Color(255, 255, 255), scale);
		}
	}
	{  // ArmorHUD
		if (!(g_Data.getLocalPlayer() == nullptr || !this->displayArmor || !GameData::canUseMoveKeys())) {
			static float constexpr scale = 1.f;
			static float constexpr opacity = 0.25f;
			static float constexpr spacing = scale + 15.f;
			C_LocalPlayer* player = g_Data.getLocalPlayer();
			float x = windowSize.x / 2.f + 5.f;
			float y = windowSize.y - 57.5f;
			for (int i = 0; i < 4; i++) {
				C_ItemStack* stack = player->getArmor(i);
				if (stack->isValid()) {
					DrawUtils::drawItem(stack, vec2_t(x, y), opacity, scale, false); //* stack->isEnchanted() is run by the thing already, this bool is if it forces it to be enchanted or no
					x += scale * spacing;
				}
			}
			C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
			C_ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
			//x += scale * spacing;
			if (item->isValid())
				DrawUtils::drawItem(item, vec2_t(x, y), opacity, scale, item->isEnchanted());
		}
	}
	{  // Keystrokes
		if (!(g_Data.getLocalPlayer() == nullptr || !this->keystrokes || !GameData::canUseMoveKeys())) {
			C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
			DrawUtils::drawKeystroke(*input->forwardKey, vec2_t(32.f, windowSize.y - 74));
			DrawUtils::drawKeystroke(*input->leftKey, vec2_t(10.f, windowSize.y - 52));
			DrawUtils::drawKeystroke(*input->backKey, vec2_t(32.f, windowSize.y - 52));
			DrawUtils::drawKeystroke(*input->rightKey, vec2_t(54.f, windowSize.y - 52));
			DrawUtils::drawKeystroke(*input->spaceBarKey, vec2_t(10.f, windowSize.y - 30));
		}
	}
}

void HudModule::onDisable() {
}