#include <gccore.h>
#include <gctypes.h>

#include <array>
#include "velvet/Renderer.hpp"
#include "velvet/core/Engine.hpp"
#include "velvet/core/vtx/VtxDescription.hpp"
#include "velvet/dvd/DVD.hpp"
#include "velvet/input/Input.hpp"

[[noreturn]] s32 main() {
	using namespace velvet;

	core::Engine engine{};

	engine.Init();

	std::array desc{
			core::vtx::VtxDescription(GX_VA_POS, GX_INDEX8, GX_POS_XYZ, GX_S16),
//			core::vtx::VtxDescription(GX_VA_CLR0, GX_INDEX8, GX_CLR_RGBA, GX_RGBA8),
			core::vtx::VtxDescription(GX_VA_TEX0, GX_DIRECT, GX_TEX_ST, GX_F32),
	};

	Renderer::SetVtxFormat(desc, GX_VTXFMT0);

	auto chungusBuf = velvet::dvd::LoadFile("chungus.tpl");
	TPLFile chungusTpl;
	TPL_OpenTPLFromMemory(&chungusTpl, chungusBuf, velvet::dvd::GetFileLength("chungus.tpl").value());

	GXTexObj texObj;
	TPL_GetTexture(&chungusTpl, 0, &texObj);
	GX_LoadTexObj(&texObj, GX_TEXMAP0);

	for (;;) {
		velvet::input::UpdateControllers();
		engine.renderer.Update();
		velvet::core::Engine::BeginDraw();

		static f32 rot = 0;

		engine.renderer.DrawCube({0, 0, 0}, {-1, 1, 0}, rot);
		engine.renderer.DrawCube({0, -3, -2}, {-1, 1, 0}, rot);

		rot++;
		if (rot > 360.f)
			rot = 0.f;

		engine.EndDraw();
	}
}
