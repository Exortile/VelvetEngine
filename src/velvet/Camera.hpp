#pragma once

#include <gccore.h>

namespace velvet {

	class Camera {
	public:
		Camera();

		void Update();

		Mtx &GetViewMatrix();

	private:
		Mtx _view{};

		guVector _pos{0, 0, 2};
		guVector _up{0, 1, 0};
		guVector _front{0, 0, 1};

		f32 _yaw = 270.f; // c stick x
		f32 _pitch = 0.f; // c stick y

		void UpdateViewMatrix();
	};

} // namespace velvet
