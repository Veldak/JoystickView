#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include <math.h>

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

#define WHITE ImColor(255, 255, 255, 255)
#define GREEN ImColor(0, 255, 0, 255)
#define RED ImColor(255, 0, 0, 255)

class JoystickView: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow, public BakkesMod::Plugin::PluginWindow
{

	//std::shared_ptr<bool> enabled;

	ImVec2 windowPos = ImVec2(50.f, 50.f);

	//Boilerplate
	virtual void onLoad();
	virtual void onUnload();

	float arrowLocX = 149.f;
	float arrowLocY = 253.f;
	float arrowWidth = 25.f;
	float arrowHeight = 100.f;


	bool FreezeArrowWhenCarDontHaveFlip = false;
	ControllerInput controllerInput;

	std::shared_ptr<ImageWrapper> myImage;
	std::shared_ptr<ImageWrapper> arrowImage;
	void ImageRotated(ImTextureID tex_id, ImVec2 center, ImVec2 size, float angle);

	void onTick(std::string eventName);

	// Inherited via PluginSettingsWindow
	void RenderSettings() override;
	std::string GetPluginName() override;
	

	// Inherited via PluginWindow
	bool isWindowOpen_ = false;
	bool isMinimized_ = false;
	std::string menuTitle_ = "JoystickView";

	virtual void Render() override;
	virtual std::string GetMenuName() override;
	virtual std::string GetMenuTitle() override;
	virtual void SetImGuiContext(uintptr_t ctx) override;
	virtual bool ShouldBlockInput() override;
	virtual bool IsActiveOverlay() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;
	
};

