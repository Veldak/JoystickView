#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include <math.h>

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

#include "RenderingTools/RenderingTools.h"

#define M_PI       3.14159265358979323846

#define WHITE ImColor(255, 255, 255, 255)
#define GREEN ImColor(0, 255, 0, 255)
#define RED ImColor(255, 0, 0, 255)


class JoystickView: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow, public BakkesMod::Plugin::PluginWindow
{
	ImVec2 windowPos = ImVec2(50.f, 50.f);

	//Boilerplate
	virtual void onLoad();
	virtual void onUnload();

	float arrowLocX = 149.f;
	float arrowLocY = 253.f;
	float arrowWidth = 25.f;
	float arrowHeight = 100.f;

	bool carArrowEnable = true;
	float carArrowLocation[3] = { 81.f, 0.f, 0.f };
	float carArrowColor[4] = { 0.6f, 0.6f, 0.6f, 0.7f };
	float carArrowRotationAnchorPoint = 0.f;
	float carArrowLength = 89.f;
	float carArrowRadius = 6.f;
	int carArrowSegments = 20;
	bool carArrowHideWhenNoInputs = true;
	bool carArrowPerpendicularAxis = true;
	bool carArrowDynamicLength = false;

	bool FreezeArrowWhenCarDontHaveFlip = false;
	ControllerInput controllerInput;

	std::shared_ptr<ImageWrapper> myImage;
	std::shared_ptr<ImageWrapper> arrowImage;
	void ImageRotated(ImTextureID tex_id, ImVec2 center, ImVec2 size, float angle);

	void onTick(std::string eventName);
	Quat RotationQuat(float angleDegrees, Vector axis);
	double CalculateDistanceOfPointFromOrigin(float x, float y);
	void DrawArrow(CanvasWrapper canvas, RT::Frustum& frust, Vector location, Quat rotation, float angleDegrees, float radius, float length, int segments, float roationAnchorPoint);
	void Render(CanvasWrapper canvas);

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

