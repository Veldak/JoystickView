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

	ImGui::SliderFloat("Arrow Location X", &arrowLocX, 0.f, 500.f);
	ImGui::SliderFloat("Arrow Location Y", &arrowLocY, 0.f, 500.f);
	ImGui::SliderFloat("Arrow Width", &arrowWidth, 0.f, 500.f);
	ImGui::SliderFloat("Arrow arrowHeight", &arrowHeight, 0.f, 500.f);
}


// Do ImGui rendering here
void JoystickView::Render()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoTitleBar;


	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}




	float scale = (1 == 1 ? 2.0f : 1.0f);
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	/*std::string valueX = "x : " + std::to_string(controllerInput.Steer);
	ImGui::Text(valueX.c_str());
	std::string valueY = "y : " + std::to_string(controllerInput.Pitch);
	ImGui::Text(valueY.c_str());*/


	// Calculate the angle in radians
	float angleRadians = std::atan2(controllerInput.Steer, controllerInput.Pitch);

	// Convert the angle to degrees
	float angleDegrees = angleRadians * (180.0f / M_PI);
	float angle = (((6.28f * angleDegrees) / 360.f) + 3.14f) * -1;

	if (controllerInput.Steer == 0 && controllerInput.Pitch == 0) //makes the arrow pointing up at starting position
		angle = -6.28f;

	/*ImGui::Text("degree : %f", angleDegrees);
	ImGui::Text("angle : %f", angle);

	static float widthTest = 0.f;
	ImGui::SliderFloat("widthTest", &widthTest, 0.f, 400.f);
	static float hightTest = 0.f;
	ImGui::SliderFloat("hightTest", &hightTest, 0.f, 400.f);*/

	ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x + 20, ImGui::GetCursorScreenPos().y + 20));
	ImVec2 p = ImGui::GetCursorScreenPos();

	float leftStickRadius = 32 * scale;
	ImVec2 leftStickCenter = ImVec2(p.x + arrowLocX, p.y + arrowLocY);

	Vector2F rect = myImage->GetSizeF();
	ImGui::Image(myImage->GetImGuiTex(), { rect.X, rect.Y });
	//drawList->AddImage(myImage->GetImGuiTex(), p, { p.x + rect.X, p.y + rect.Y });
	ImageRotated(arrowImage->GetImGuiTex(), leftStickCenter, ImVec2(arrowWidth, arrowHeight), angle);
	//drawList->AddCircle(leftStickCenter, 24 * scale, WHITE, 32, 2 * scale);


	ImVec2 JoystickPos = ImVec2(leftStickCenter.x + (controllerInput.Steer * 8 * scale * 4), leftStickCenter.y + (controllerInput.Pitch * 8 * scale * 4));
	//drawList->AddLine(ImVec2(leftStickCenter), JoystickPos, WHITE, 1.f);


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
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool JoystickView::IsActiveOverlay()
{
	return true;
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
