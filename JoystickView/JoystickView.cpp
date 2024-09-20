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

	gameWrapper->RegisterDrawable([this](CanvasWrapper canvas) {
		Render(canvas);
		});
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
			controllerInput.Steer = 0; //X (-1 to 1)
			controllerInput.Pitch = 0; //Y (-1 to 1)
		}
	}
}

Quat JoystickView::RotationQuat(float angleDegrees, Vector axis)
{
	float halfAngleRadians = angleDegrees * M_PI / 180.0f / 2.0f;
	float sinHalfAngle = sin(halfAngleRadians);

	return Quat(
		cos(halfAngleRadians), // w
		axis.X * sinHalfAngle, // x
		axis.Y * sinHalfAngle, // y
		axis.Z * sinHalfAngle  // z
	);
}

double JoystickView::CalculateDistanceOfPointFromOrigin(float x, float y)
{
	return std::sqrt(x * x + y * y);
}

void JoystickView::DrawArrow(CanvasWrapper canvas, RT::Frustum& frust, Vector location, Quat rotation, float angleDegrees, float radius, float height, int segments, float roationAnchorPoint)
{
	// Apply the 90 degree rotation to the cylinder quaternion because the initial rotation is not aligned with the car
	Quat fixedCylinderRotation = rotation * RotationQuat(90, { 0, 1, 0 });
	Quat cylinderRotated = fixedCylinderRotation * RotationQuat(angleDegrees * -1.f, { 1, 0, 0 });

	Quat coneRotated = rotation * RotationQuat(angleDegrees, { 0, 0, 1 });
	Vector coneLocation = location + RotateVectorWithQuat({ height * float(1.f - roationAnchorPoint), 0, 0}, coneRotated);

	RT::Cylinder(location, cylinderRotated, radius * 0.5f, height).Draw(canvas, frust, segments, roationAnchorPoint);
	RT::Cone(coneLocation, Vector(coneLocation - location), radius, height * 0.33f, 0.f, segments).Draw(canvas);
}

void JoystickView::Render(CanvasWrapper canvas)
{
	if (!carArrowEnable) return;
	if (!gameWrapper->IsInFreeplay() && !gameWrapper->IsInCustomTraining()) return;

	CarWrapper localCar = gameWrapper->GetLocalCar();
	if (!localCar) return;

	CameraWrapper camera = gameWrapper->GetCamera();
	if (camera.IsNull()) return;

	RT::Frustum frust{ canvas, camera };

	// Dodge angle snapping
	float inputSteer = (std::abs(controllerInput.Steer) < 0.1 * std::abs(controllerInput.Pitch)) ? 0 : controllerInput.Steer;
	//doing * -1 so the arrow is in the right direction because the positive part of the input is when the joystick is put backward
	float inputPitch = (std::abs(controllerInput.Pitch) < 0.1 * std::abs(controllerInput.Steer)) ? 0 : controllerInput.Pitch * -1;

	// Calculate the angle in radians
	float angleRadians = std::atan2(inputSteer, inputPitch);
	// Convert the angle to degrees
	float angleDegrees = angleRadians * (180.0f / M_PI);

	//makes the arrow pointing up at starting position if not touching the joystick
	if (inputSteer == 0 && inputPitch == 0)
		angleDegrees = 0.f;

	Vector arrowLocation = localCar.GetLocation() + RotateVectorWithQuat({ carArrowLocation[0], carArrowLocation[1], carArrowLocation[2] }, RotatorToQuat(localCar.GetRotation()));

	float arrowLength = carArrowLength;
	if (carArrowDynamicLength)
	{
		arrowLength = carArrowLength * CalculateDistanceOfPointFromOrigin(inputSteer, inputPitch);
	}

	DrawArrow(canvas, frust, arrowLocation, RotatorToQuat(localCar.GetRotation()), angleDegrees, carArrowRadius, arrowLength, carArrowSegments, carArrowRotationAnchorPoint);
}

void JoystickView::onUnload()
{
}
