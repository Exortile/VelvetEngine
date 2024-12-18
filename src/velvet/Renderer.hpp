#pragma once

#include <gccore.h>
#include <span>
#include "velvet/Camera.hpp"
#include "velvet/core/vtx/VtxFormat.hpp"
#include "velvet/formats/VObject.hpp"

namespace velvet::renderer {

	struct XfbData {
		u32 currFb;
		std::array<void *, 2> xfb;
	};

	extern XfbData gXfbData;

	void BeginDraw();
	void EndDraw();

	void DrawColoredCube(const guVector &translation, const guVector &rotAxis, const f32 rotation);
	void DrawTexturedCube(const u8 texmap, const guVector &translation, const guVector &rotAxis, const f32 rotation);

	void DrawTexturedVObj(const formats::VObject &vobj, const u8 texmap, const guVector &translation, const guVector &rotAxis, const f32 rotation);

} // namespace velvet::renderer
