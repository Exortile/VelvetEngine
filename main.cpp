#include <gccore.h>
#include <gctypes.h>

#include "velvet/Renderer.hpp"
#include "velvet/core/Core.hpp"
#include "velvet/core/Engine.hpp"
#include "velvet/core/loaders/ModelLoader.hpp"
#include "velvet/dvd/DVD.hpp"
#include "velvet/input/Input.hpp"

[[noreturn]] s32 main() {
	using namespace velvet;

	core::Init();

	auto textureFile = velvet::dvd::LoadFile("textures.tpl").value();
	TPLFile tplFile;
	TPL_OpenTPLFromMemory(&tplFile, textureFile.block.buf, textureFile.len);

	GXTexObj texObj;

	TPL_GetTexture(&tplFile, 0, &texObj);
	GX_LoadTexObj(&texObj, GX_TEXMAP0);

	TPL_GetTexture(&tplFile, 1, &texObj);
	GX_LoadTexObj(&texObj, GX_TEXMAP1);

	auto cubeVobj = velvet::dvd::LoadFile("cube.vobj").value();
	const auto vobj = core::loaders::InitVOBJ(cubeVobj.block.buf);

	for (;;) {
		input::UpdateControllers();
		core::gMainCamera.Update();

		renderer::BeginDraw();

		static f32 rot = 0;

		renderer::DrawTexturedVObj(*vobj.value(), GX_TEXMAP0, {0, 0, 0}, {-1, 1, 0}, rot);
		renderer::DrawTexturedVObj(*vobj.value(), GX_TEXMAP1, {0, -3, -2}, {-1, 1, 0}, rot);
		renderer::DrawTexturedVObj(*vobj.value(), GX_TEXMAP0, {0, -3, -5}, {-1, 1, 0}, rot);

		rot++;
		if (rot > 360.f)
			rot = 0.f;

		renderer::EndDraw();
	}
}
