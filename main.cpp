#include <gccore.h>
#include <gctypes.h>

#include "velvet/Renderer.hpp"
#include "velvet/core/Core.hpp"
#include "velvet/core/Engine.hpp"
#include "velvet/core/loaders/ModelLoader.hpp"
#include "velvet/core/loaders/TextureLoader.hpp"
#include "velvet/dvd/DVD.hpp"
#include "velvet/input/Input.hpp"
#include "velvet/math/Quaternion.hpp"

[[noreturn]] s32 main() {
	using namespace velvet;

	core::Init();

	auto texturesTpl = core::loaders::LoadTPL("textures.tpl");
	auto skyboxTpl = core::loaders::LoadTPL("skybox.tpl");

	auto monkeyFile = dvd::LoadFile("models/aiai.vobj").value();
	const auto monkeyVobj = core::loaders::InitVOBJ(dvd::GetFileBuffer(monkeyFile));

	for (;;) {
		input::UpdateControllers();
		core::gMainCamera.Update();

		renderer::BeginDraw();

		// Draw everything

		core::loaders::LoadTexFromTPL(texturesTpl, 0, GX_TEXMAP0);
		core::loaders::LoadTexFromTPL(texturesTpl, 1, GX_TEXMAP1);
		core::loaders::LoadTexFromTPL(texturesTpl, 2, GX_TEXMAP2);

		renderer::SetLight({0.f, 0.5f, 1.f}, 100.f, {255, 255, 255, 255});
		renderer::SetMaterial({255, 255, 255, 255}, {16, 16, 16, 255});
		renderer::SetTexture(GX_TEXMAP2, true);

		//		static f32 rot = 0;

		renderer::DrawTexturedVObj(
				*monkeyVobj.value(),
				{.translation = {0, 0, 0}, .scale = {2, 2, 2}});

		// Draw skybox

		core::loaders::LoadCubemapTexture(skyboxTpl);
		renderer::DrawSkybox();

		/*rot++;
		if (rot > 360.f)
			rot = 0.f;*/

		renderer::EndDraw();
	}
}
