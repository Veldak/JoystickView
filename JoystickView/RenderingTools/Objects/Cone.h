#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"

class CanvasWrapper;

namespace RT
{
	class Cone
	{
	public:
		Vector location;
		Vector direction;
		float radius;
		float height;
		float rollAmount;
		float thickness;
		int32_t segments;

		// CONSTRUCTORS
		explicit Cone();
		//explicit Cone(Vector loc, Vector dir);
		explicit Cone(Vector loc, Vector dir, float _radius = 5.0f, float _height = 20.0f, float _rollAmount = 0.0f, int32_t _segments = 8, float _thickness = 1.0f);

		// FUNCTIONS
		void Draw(CanvasWrapper canvas) const;
	};
}
