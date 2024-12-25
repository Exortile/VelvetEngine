#include "Quaternion.hpp"

namespace velvet::math::quat {
	[[nodiscard]] guQuaternion MakeRotation(const f32 x, const f32 y, const f32 z) {
		constexpr guVector xAxis = {1, 0, 0};
		constexpr guVector yAxis = {0, 1, 0};
		constexpr guVector zAxis = {0, 0, 1};

		guQuaternion result;
		guQuaternion axisQuat;

		guQuatRotAxisRad(&result, &xAxis, DegToRad(x));

		guQuatRotAxisRad(&axisQuat, &yAxis, DegToRad(y));
		guQuatMultiply(&result, &axisQuat, &result);

		guQuatRotAxisRad(&axisQuat, &zAxis, DegToRad(z));
		guQuatMultiply(&result, &axisQuat, &result);

		return result;
	}
} // namespace velvet::math::quat
