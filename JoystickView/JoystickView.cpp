#include "pch.h"
#include "JoystickView.h"


BAKKESMOD_PLUGIN(JoystickView, "JoystickView", "1.1", PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void JoystickView::onLoad()
{
	_globalCvarManager = cvarManager;

	myImage = std::make_shared<ImageWrapper>(gameWrapper->GetDataFolder() / "JoystickView" / "OctaneHitbox.png", true, true);
	arrowImage = std::make_shared<ImageWrapper>(gameWrapper->GetDataFolder() / "JoystickView" / "red-up-arrow.png", true, true);

	gameWrapper->HookEvent("Function Engine.GameViewportClient.Tick", std::bind(&JoystickView::onTick, this, std::placeholders::_1));
}


void JoystickView::onTick(std::string eventName) //Reminder : This function is called every frame
{
	if (gameWrapper->IsInFreeplay() || gameWrapper->IsInCustomTraining())
	{
		CarWrapper car = gameWrapper->GetLocalCar();
		if (!car) { return; } //needed to avoid game crash if car is null


		if (!car.IsNull()) {
			if (car.HasFlip())
			{
				controllerInput = car.GetInput();
			}
			else
			{
				if (!FreezeArrowWhenCarDontHaveFlip)
				{
					controllerInput = car.GetInput();
				}
			}
		}
		else
		{
			controllerInput.Yaw = 0; //X (-1 to 1)
			controllerInput.Pitch = 0; //Y (-1 to 1)
		}
	}
}

void JoystickView::onUnload()
{
}
