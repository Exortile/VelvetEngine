#pragma once

#include <gccore.h>

namespace velvet::core {
	struct Transform {
		guVector translation = {0, 0, 0};
		guQuaternion rotation = {0, 0, 0, 1};
		guVector scale = {1, 1, 1};

		constexpr void toModelMatrix(Mtx out) const {
			guMtxIdentity(out);
			guMtxQuat(out, &rotation);
			guMtxTransApply(out, out, translation.x, translation.y, translation.z);
			guMtxScaleApply(out, out, scale.x, scale.y, scale.z);
		}
	};
} // namespace velvet::core
