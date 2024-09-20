#include "pch.h"
#include "JoystickView.h"

#define M_PI           3.14159265358979323846

std::string JoystickView::GetPluginName() {
	return "JoystickView";
}


// Render the plugin settings here
// This will show up in bakkesmod when the plugin is loaded at
//  f2 -> plugins -> JoystickView
void JoystickView::RenderSettings() 
{
	if (ImGui::Button("Open Plugin Window"))
	{
		gameWrapper->Execute([&](GameWrapper* gw) {
			cvarManager->executeCommand("togglemenu " + GetMenuName());
			});
	}

	ImGui::Checkbox("Freeze Arrow When Car Don't Have Flip", &FreezeArrowWhenCarDontHaveFlip);
	ImGui::NewLine();
	ImGui::SliderFloat("Overlay Location X", &windowPos.x, 0.f, 3840);
	ImGui::SliderFloat("Overlay Location Y", &windowPos.y, 0.f, 2160.f);
	ImGui::NewLine();
	ImGui::SliderFloat("Arrow Location X", &arrowLocX, 0.f, 500.f);
	ImGui::SliderFloat("Arrow Location Y", &arrowLocY, 0.f, 500.f);
	ImGui::NewLine();
	ImGui::SliderFloat("Arrow Width", &arrowWidth, 0.f, 500.f);
	ImGui::SliderFloat("Arrow Height", &arrowHeight, 0.f, 500.f);

	ImGui::Separator();
	ImGui::NewLine();

	ImGui::Text("3D Car Arrow :");

	ImGui::NewLine();

	ImGui::Checkbox("Enable", &carArrowEnable);
	ImGui::ColorEdit4("Color", carArrowColor);
	ImGui::SliderFloat3("Location (X, Y, Z)", carArrowLocation, -150.f, 400.f);
	ImGui::SliderFloat("Rotation Anchor Point", &carArrowRotationAnchorPoint, 0.f, 1.f);
	ImGui::SliderFloat("Length", &carArrowLength, 0.f, 400.f);
	ImGui::SliderFloat("Radius", &carArrowRadius, 0.f, 100.f);
	ImGui::SliderInt("Segments", &carArrowSegments, 0, 50.f);
	ImGui::Checkbox("Hide When No Inputs", &carArrowHideWhenNoInputs);
	ImGui::Checkbox("Perpendicular Axis", &carArrowPerpendicularAxis);
	ImGui::Checkbox("Dynamic Length", &carArrowDynamicLength);
}


// Do ImGui rendering here
void JoystickView::Render()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoTitleBar;

	ImGui::SetNextWindowPos(windowPos);
	ImGui::SetNextWindowSize(ImVec2(345.f, 520));
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	// Dodge angle snapping
	float inputSteer = (std::abs(controllerInput.Steer) < 0.1 * std::abs(controllerInput.Pitch)) ? 0 : controllerInput.Steer;
	float inputPitch = (std::abs(controllerInput.Pitch) < 0.1 * std::abs(controllerInput.Steer)) ? 0 : controllerInput.Pitch;

	// Calculate the angle in radians
	float angleRadians = std::atan2(inputSteer, inputPitch);

	// Convert the angle to degrees
	float angleDegrees = angleRadians * (180.0f / M_PI);
	float angle = (((6.28f * angleDegrees) / 360.f) + 3.14f) * -1;

	if (inputSteer == 0 && inputPitch == 0) //makes the arrow pointing up at starting position
		angle = -6.28f;

	/*ImGui::Text("degree : %f", angleDegrees);
	ImGui::Text("angle : %f", angle);*/

	ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x + 20, ImGui::GetCursorScreenPos().y + 20));

	ImVec2 p = ImGui::GetCursorScreenPos();
	ImVec2 leftStickCenter = ImVec2(p.x + arrowLocX, p.y + arrowLocY);
	Vector2F rect = myImage->GetSizeF();

	ImGui::Image(myImage->GetImGuiTex(), { rect.X, rect.Y });
	ImageRotated(arrowImage->GetImGuiTex(), leftStickCenter, ImVec2(arrowWidth, arrowHeight), angle);

	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
{
	return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}
static inline ImVec2 ImRotate(const ImVec2& v, float cos_a, float sin_a)
{
	return ImVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a);
}
void JoystickView::ImageRotated(ImTextureID tex_id, ImVec2 center, ImVec2 size, float angle)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float cos_a = cosf(angle);
	float sin_a = sinf(angle);
	ImVec2 pos[4] =
	{
		center + ImRotate(ImVec2(-size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
		center + ImRotate(ImVec2(+size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
		center + ImRotate(ImVec2(+size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a),
		center + ImRotate(ImVec2(-size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a)
	};
	ImVec2 uvs[4] =
	{
		ImVec2(0.0f, 0.0f),
		ImVec2(1.0f, 0.0f),
		ImVec2(1.0f, 1.0f),
		ImVec2(0.0f, 1.0f)
	};

	draw_list->AddImageQuad(tex_id, pos[0], pos[1], pos[2], pos[3], uvs[0], uvs[1], uvs[2], uvs[3], IM_COL32_WHITE);
}

// Name of the menu that is used to toggle the window.
std::string JoystickView::GetMenuName()
{
	return "JoystickView";
}

// Title to give the menu
std::string JoystickView::GetMenuTitle()
{
	return menuTitle_;
}

// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void JoystickView::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool JoystickView::ShouldBlockInput()
{
	return false;
}

// Return true if window should be interactive
bool JoystickView::IsActiveOverlay()
{
	return false;
}

// Called when window is opened
void JoystickView::OnOpen()
{
	isWindowOpen_ = true;
}

// Called when window is closed
void JoystickView::OnClose()
{
	isWindowOpen_ = false;
}
