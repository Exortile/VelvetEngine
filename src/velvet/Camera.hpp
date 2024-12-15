#pragma once

#include <gccore.h>

namespace velvet {

	class Camera {
	public:
		Camera();

		void Update();

		Mtx44 &GetViewMatrix();

	private:
		Mtx44 _view{};

		guVector _pos{0, 0, -2};
		guVector _up{0, 1, 0};
		guVector _front{0, 0, 1};

		f32 _yaw = 90.f; // c stick x
		f32 _pitch = 0.f; // c stick y

		void UpdateViewMatrix();
	};

} // namespace velvet
