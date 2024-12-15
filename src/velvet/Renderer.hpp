#pragma once

#include <gccore.h>
#include <span>
#include "velvet/Camera.hpp"
#include "velvet/core/vtx/VtxDescription.hpp"

namespace velvet {

	class Renderer {
	public:
		static void SetVtxFormat(const std::span<velvet::core::vtx::VtxDescription> &format, GXVtxFmt vtxfmt);

		void Update();

		void DrawCube(const guVector &translation, const guVector &rotAxis, f32 rotation);

	private:
		inline static void DrawQuadIndexed(u8 p0, u8 p1, u8 p2, u8 p3, u8 c0);
		velvet::Camera _camera;
	};

} // namespace velvet
