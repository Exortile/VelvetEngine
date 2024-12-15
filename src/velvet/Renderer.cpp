#include "Renderer.hpp"

#include <array>
#include <gctypes.h>

namespace {
	constinit std::array<s16, 24> CubePositions alignas(32) = {
			// x y z
			-1, 1,	-1, // 0
			1,	1,	-1, // 1
			1,	1,	1,	// 2
			-1, 1,	1,	// 3
			1,	-1, -1, // 4
			1,	-1, 1,	// 5
			-1, -1, 1,	// 6
			-1, -1, -1, // 7
	};

	constinit std::array<u8, 24> CubeColors alignas(32) = {
			// r, g, b, a
			100, 10,  100, 255, // 0 purple
			240, 0,	  0,   255, // 1 red
			255, 180, 0,   255, // 2 orange
			255, 255, 0,   255, // 3 yellow
			10,	 120, 40,  255, // 4 green
			0,	 20,  100, 255	// 5 blue
	};
} // namespace

namespace velvet {
	void Renderer::SetVtxFormat(const std::span<velvet::core::vtx::VtxDescription> &format, GXVtxFmt vtxfmt) {
		GX_ClearVtxDesc();

		for (const auto &attributes: format) {
			GX_SetVtxDesc(attributes.desc.attr, attributes.desc.type);
			GX_SetVtxAttrFmt(vtxfmt, attributes.desc.attr, attributes.fmt.type, attributes.fmt.size, 0);
		}
	}

	inline void Renderer::DrawQuadIndexed(u8 p0, u8 p1, u8 p2, u8 p3, u8 c0) {
		GX_Position1x8(p0);
		GX_TexCoord2f32(0, 0);
//		GX_Color1x8(c0);

		GX_Position1x8(p1);
		GX_TexCoord2f32(1, 0);
//		GX_Color1x8(c0);

		GX_Position1x8(p2);
		GX_TexCoord2f32(1, 1);
//		GX_Color1x8(c0);

		GX_Position1x8(p3);
		GX_TexCoord2f32(0, 1);
//		GX_Color1x8(c0);
	}

	void Renderer::DrawCube(const guVector &translation, const guVector &rotAxis, const f32 rotation) {
		GX_SetNumChans(1);

		GX_SetNumTexGens(1);
		GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);
		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
		GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

		/*GX_SetNumTexGens(0);
		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
		GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);*/

		GX_SetArray(GX_VA_POS, CubePositions.data(), 3 * sizeof(s16));
//		GX_SetArray(GX_VA_CLR0, CubeColors.data(), 4 * sizeof(u8));

		Mtx model;
		guMtxIdentity(model);
		guMtxRotAxisDeg(model, &rotAxis, rotation);
		guMtxTransApply(model, model, translation.x, translation.y, translation.z);

		Mtx modelView;
		guMtxConcat(_camera.GetViewMatrix(), model, modelView);

		GX_LoadPosMtxImm(modelView, GX_PNMTX0);
		GX_SetCurrentMtx(GX_PNMTX0);

		GX_Begin(GX_QUADS, GX_VTXFMT0, 24);

		DrawQuadIndexed(0, 3, 2, 1, 0);
		DrawQuadIndexed(0, 7, 6, 3, 1);
		DrawQuadIndexed(0, 1, 4, 7, 2);
		DrawQuadIndexed(1, 2, 5, 4, 3);
		DrawQuadIndexed(2, 3, 6, 5, 4);
		DrawQuadIndexed(4, 7, 6, 5, 5);

		GX_End();
	}

	void Renderer::Update() {
		_camera.Update();
	}
} // namespace velvet
