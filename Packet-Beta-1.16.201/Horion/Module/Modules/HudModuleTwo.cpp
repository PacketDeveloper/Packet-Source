#include "HudModuleTwo.h"

#include "../../DrawUtils.h"
#include "../../Scripting/ScriptManager.h"

HudModuleTwo::HudModuleTwo() : IModule(0, Category::VISUAL, "Displays ArrayList") {
	registerBoolSetting("Rainbow", &this->rainbow, this->rainbow);
	registerBoolSetting("Rainbow2", &this->r2, this->r2);
	registerBoolSetting("Dynamic", &this->dynamic, this->dynamic);
	registerBoolSetting("Horion", &this->horion, this->horion);
	registerBoolSetting("White", &this->white, this->white);
	
	registerBoolSetting("TextShadow", &this->textShadow, this->textShadow);
	registerBoolSetting("Keystrokes", &this->keystrokes, this->keystrokes);
	registerBoolSetting("Watermark", &this->watermark, this->watermark);
	registerBoolSetting("FPS", &this->fps, this->fps);
	//registerBoolSetting("CPS", &this->cps, this->cps);
	registerBoolSetting("BPS", &this->bps, this->bps);

	registerFloatSetting("FpsPosX", &this->fpspos2, this->fpspos2, 1.f, 450.f);
	registerFloatSetting("FpsPosY", &this->fpspos, this->fpspos, 1.f, 150.f);

	//registerFloatSetting("CpsPosX", &this->cpspos2, this->cpspos2, 1.f, 450.f);
	//registerFloatSetting("CpsPosY", &this->cpspos, this->cpspos, 1.f, 150.f);

		registerFloatSetting("BpsPosX", &this->bpsX, this->bpsX, 1.f, 800.f);
	registerFloatSetting("BpsPosY", &this->bpsY, this->bpsY, 1.f, 800.f);
	//registerBoolSetting("Always show", &this->alwaysShow, this->alwaysShow);
}

HudModuleTwo::~HudModuleTwo() {
}

const char* HudModuleTwo::getModuleName() {
	return ("Hud");
}

void HudModuleTwo::onTick(C_GameMode* gm) {
	if (bps) {
		auto player = g_Data.getLocalPlayer();
		vec3_t vel = gm->player->velocity;
		if (vel.x < 0.f)
			vel.x *= -1.f;
		if (vel.y < 0.f)
			vel.y *= -1.f;
		if (vel.z < 0.f)
			vel.z *= -1.f;

		speed = sqrtf((vel.x * vel.x) + (vel.z * vel.z));
		speed *= 36.6f;

		if (!gm->player->onGround)
			speed /= 1.5f;
	}
}

void HudModuleTwo::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (bps) {
		if (g_Data.getClientInstance()->isInGame()) {
			float yVal = bpsY;
			float xVal = bpsX;

			std::string BpsText = "Speed: " + std::string(std::to_string((int)speed)) + " b/s";

			DrawUtils::drawText(vec2_t(bpsX, bpsY), &BpsText, MC_Color(255, 255, 255), scale);
		}
	}
}

void HudModuleTwo::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	float f = 10.f * this->scale;
	std::string tempStr("Movement");
	float len = DrawUtils::getTextWidth(&tempStr, scale) + 7.f;
	float startY = tabgui ? 6 * f : 0.f;
	if (tabgui && scriptMgr.getNumEnabledScripts() > 0)
		startY += f;
	{  // FPS
		if (!(g_Data.getLocalPlayer() == nullptr || !this->fps)) {
			std::string fpsText = "FPS: " + std::to_string(g_Data.getFPS());
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
			vec2_t textPos = vec2_t(rectPos.x * fpspos2, rectPos.y * fpspos);
			DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), 0.0f);
			DrawUtils::drawText(textPos, &fpsText, MC_Color(255, 255, 255), scale);

			startY *= f;
		}
	}

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

			std::string coordsX = "X: " + std::to_string((int)floorf(pos->x));
			std::string coordsY = "Y: " + std::to_string((int)floorf(pos->y));
			std::string coordsZ = "Z: " + std::to_string((int)floorf(pos->z));
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 35.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 1.f);
			DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), 0.3f);
			DrawUtils::drawText(textPos, &coordsX, MC_Color(255, 255, 255), scale);
			textPos.y += f;
			DrawUtils::drawText(textPos, &coordsY, MC_Color(255, 255, 255), scale);
			textPos.y += f;
			DrawUtils::drawText(textPos, &coordsZ, MC_Color(255, 255, 255), scale);
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
					DrawUtils::drawItem(stack, vec2_t(x, y), opacity, scale, stack->isEnchanted());
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

void HudModuleTwo::onDisable() {
	auto player = g_Data.getLocalPlayer();
}