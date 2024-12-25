#pragma once

#include <gccore.h>
#include <span>
#include "velvet/Camera.hpp"
#include "velvet/core/Transform.hpp"
#include "velvet/core/vtx/VtxFormat.hpp"
#include "velvet/formats/VObject.hpp"

namespace velvet::renderer {

	struct XfbData {
		u32 currFb;
		std::array<void *, 2> xfb;
	};

	extern XfbData gXfbData;
	inline GXRModeObj *gGXRMode = nullptr;

	inline void *gWhiteTexData = nullptr;
	inline GXTexObj gWhiteZTexObj;

	[[maybe_unused]] [[nodiscard]] u16 GetWidth();
	[[maybe_unused]] [[nodiscard]] u16 GetHeight();

	void BeginDraw();
	void EndDraw();

	void DrawSkybox();

	void DrawColoredCube(const core::Transform &transform);
	void DrawTexturedCube(const u8 texmap, const core::Transform &transform);

	void DrawTexturedVObj(const formats::VObject &vobj, const core::Transform &transform);

	void SetLight(const guVector &position, f32 brightness, const GXColor &color);
	void SetNoLight();

	void SetMaterial(const GXColor &diffuseColor, const GXColor &ambientColor);

	void SetTexture(const u8 texmap, const bool useLighting);

} // namespace velvet::renderer
