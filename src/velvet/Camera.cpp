#include "Camera.hpp"
#include <algorithm>
#include <cmath>
#include "velvet/input/Input.hpp"

namespace velvet {
	Camera::Camera() {
		UpdateViewMatrix();
	}

	Mtx &Camera::GetViewMatrix() {
		return _view;
	}

	void Camera::Update() {
		constexpr auto moveSpeed = 0.1f / 100.f;
		constexpr auto lookSpeed = 2.0f / 100.f;

		const auto &controller = velvet::input::GetController(PAD_CHAN0);

		const auto [lStick, cStick] = controller.GetSticks().value();

		guVector scaledFront;
		guVecScale(&_front, &scaledFront, static_cast<f32>(lStick.y) * moveSpeed);

		guVector right;
		guVecCross(&_front, &_up, &right);
		guVecNormalize(&right);
		guVecScale(&right, &right, static_cast<f32>(lStick.x) * moveSpeed);

		guVecAdd(&_pos, &scaledFront, &_pos);
		guVecAdd(&_pos, &right, &_pos);

		_yaw += static_cast<f32>(cStick.x) * lookSpeed;
		_pitch += static_cast<f32>(cStick.y) * lookSpeed;

		_pitch = std::clamp(_pitch, -89.f, 89.f);
		if (_yaw > 360.f)
			_yaw = 0.f;
		else if (_yaw < 0.f)
			_yaw = 360.f;

		UpdateViewMatrix();
	}

	void Camera::UpdateViewMatrix() {
		guVector target;
		_front = {
				.x = std::cos(DegToRad(_yaw)) * std::cos(DegToRad(_pitch)),
				.y = std::sin(DegToRad(_pitch)),
				.z = std::sin(DegToRad(_yaw)) * std::cos(DegToRad(_pitch))};
		guVecNormalize(&_front);
		guVecAdd(&_pos, &_front, &target);
		guLookAt(_view, &_pos, &_up, &target);
	}
} // namespace velvet
