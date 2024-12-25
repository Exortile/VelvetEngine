#pragma once

#include <gccore.h>

namespace velvet::math::quat {
	[[nodiscard]] guQuaternion MakeRotation(const f32 x, const f32 y, const f32 z);
}
