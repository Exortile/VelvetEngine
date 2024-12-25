#include "Matrix.hpp"

namespace velvet::math::matrix {
	void RemoveTranslation(Mtx m) {
		guMtxRowCol(m, 0, 3) = 0.f;
		guMtxRowCol(m, 1, 3) = 0.f;
		guMtxRowCol(m, 2, 3) = 0.f;
	}
} // namespace velvet::math::matrix
