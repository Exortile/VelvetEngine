#include "Renderer.hpp"
#include "velvet/core/Engine.hpp"
#include "velvet/core/vtx/VtxFormat.hpp"

#include <array>
#include <gctypes.h>
#include <stdexcept>

namespace {
	using namespace velvet;

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

	const core::vtx::VtxFormat VtxFormatTex(
			{
					core::vtx::VtxDescription(GX_VA_POS, GX_INDEX8, GX_POS_XYZ, GX_S16),
					//			core::vtx::VtxDescription(GX_VA_CLR0, GX_INDEX8, GX_CLR_RGBA, GX_RGBA8),
					core::vtx::VtxDescription(GX_VA_TEX0, GX_DIRECT, GX_TEX_ST, GX_F32),
			},
			GX_VTXFMT0);

	const core::vtx::VtxFormat VtxFormatColor(
			{
					core::vtx::VtxDescription(GX_VA_POS, GX_INDEX8, GX_POS_XYZ, GX_S16),
					core::vtx::VtxDescription(GX_VA_CLR0, GX_INDEX8, GX_CLR_RGBA, GX_RGBA8),
					//					core::vtx::VtxDescription(GX_VA_TEX0, GX_DIRECT, GX_TEX_ST, GX_F32),
			},
			GX_VTXFMT1);

	const core::vtx::VtxFormat VtxFormatVobj(
			{
					core::vtx::VtxDescription(GX_VA_POS, GX_INDEX8, GX_POS_XYZ, GX_F32),
					core::vtx::VtxDescription(GX_VA_NRM, GX_INDEX8, GX_NRM_XYZ, GX_F32),
					core::vtx::VtxDescription(GX_VA_TEX0, GX_INDEX8, GX_TEX_ST, GX_F32),
			},
			GX_VTXFMT0);

	void SetVtxFormatAndDesc(const core::vtx::VtxFormat &vtxFormat) {
		vtxFormat.Apply();
	}

	void DrawTexturedQuadIndexed(u8 p0, u8 p1, u8 p2, u8 p3) {
		GX_Position1x8(p0);
		GX_TexCoord2f32(0, 0);

		GX_Position1x8(p1);
		GX_TexCoord2f32(1, 0);

		GX_Position1x8(p2);
		GX_TexCoord2f32(1, 1);

		GX_Position1x8(p3);
		GX_TexCoord2f32(0, 1);
	}

	void DrawColoredQuadIndexed(u8 p0, u8 p1, u8 p2, u8 p3, u8 c0) {
		GX_Position1x8(p0);
		GX_Color1x8(c0);

		GX_Position1x8(p1);
		GX_Color1x8(c0);

		GX_Position1x8(p2);
		GX_Color1x8(c0);

		GX_Position1x8(p3);
		GX_Color1x8(c0);
	}
} // namespace

namespace velvet::renderer {
	XfbData gXfbData{};

	void BeginDraw() {
		GX_InvVtxCache();
		GX_InvalidateTexAll();
	}

	void EndDraw() {
		auto &[currFb, xfb] = gXfbData;

		GX_DrawDone();
		GX_CopyDisp(xfb[currFb], GX_TRUE);

		VIDEO_SetNextFramebuffer(xfb[currFb]);
		VIDEO_Flush();
		VIDEO_WaitVSync();

		currFb ^= 1;
	}

	void DrawColoredCube(const guVector &translation, const guVector &rotAxis, const f32 rotation) {
		SetVtxFormatAndDesc(VtxFormatColor);

		GX_SetNumChans(1);

		GX_SetNumTexGens(0);
		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
		GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

		GX_SetArray(GX_VA_POS, CubePositions.data(), 3 * sizeof(s16));
		GX_SetArray(GX_VA_CLR0, CubeColors.data(), 4 * sizeof(u8));

		Mtx model;
		guMtxIdentity(model);
		guMtxRotAxisDeg(model, &rotAxis, rotation);
		guMtxTransApply(model, model, translation.x, translation.y, translation.z);

		Mtx modelView;
		guMtxConcat(core::gMainCamera.GetViewMatrix(), model, modelView);

		GX_LoadPosMtxImm(modelView, GX_PNMTX0);
		GX_SetCurrentMtx(GX_PNMTX0);

		GX_Begin(GX_QUADS, VtxFormatColor.vtxfmt, 24);

		DrawColoredQuadIndexed(0, 3, 2, 1, 0);
		DrawColoredQuadIndexed(0, 7, 6, 3, 1);
		DrawColoredQuadIndexed(0, 1, 4, 7, 2);
		DrawColoredQuadIndexed(1, 2, 5, 4, 3);
		DrawColoredQuadIndexed(2, 3, 6, 5, 4);
		DrawColoredQuadIndexed(4, 7, 6, 5, 5);

		GX_End();
	}

	void DrawTexturedCube(const u8 texmap, const guVector &translation, const guVector &rotAxis, const f32 rotation) {
		SetVtxFormatAndDesc(VtxFormatTex);

		GX_SetNumChans(1);

		GX_SetNumTexGens(1);
		GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);
		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, texmap, GX_COLOR0A0);
		GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

		GX_SetArray(GX_VA_POS, CubePositions.data(), 3 * sizeof(s16));

		Mtx model;
		guMtxIdentity(model);
		guMtxRotAxisDeg(model, &rotAxis, rotation);
		guMtxTransApply(model, model, translation.x, translation.y, translation.z);

		Mtx modelView;
		guMtxConcat(core::gMainCamera.GetViewMatrix(), model, modelView);

		GX_LoadPosMtxImm(modelView, GX_PNMTX0);
		GX_SetCurrentMtx(GX_PNMTX0);

		GX_Begin(GX_QUADS, VtxFormatTex.vtxfmt, 24);

		DrawTexturedQuadIndexed(0, 3, 2, 1);
		DrawTexturedQuadIndexed(0, 7, 6, 3);
		DrawTexturedQuadIndexed(0, 1, 4, 7);
		DrawTexturedQuadIndexed(1, 2, 5, 4);
		DrawTexturedQuadIndexed(2, 3, 6, 5);
		DrawTexturedQuadIndexed(4, 7, 6, 5);

		GX_End();
	}

	void DrawTexturedVObj(
			[[maybe_unused]] const formats::VObject &vobj,
			[[maybe_unused]] const u8 texmap,
			[[maybe_unused]] const guVector &translation,
			[[maybe_unused]] const guVector &rotAxis,
			[[maybe_unused]] const f32 rotation) {
		SetVtxFormatAndDesc(VtxFormatVobj);

		GX_SetNumChans(1);

		GX_SetNumTexGens(1);
		GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);
		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, texmap, GX_COLOR0A0);
		GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

		GX_SetArray(GX_VA_POS, vobj.vertexData->positions, 3 * sizeof(f32));
		GX_SetArray(GX_VA_NRM, vobj.vertexData->normals, 3 * sizeof(f32));
		GX_SetArray(GX_VA_TEX0, vobj.vertexData->uvs, 2 * sizeof(f32));

		Mtx model;
		guMtxIdentity(model);
		guMtxRotAxisDeg(model, &rotAxis, rotation);
		guMtxTransApply(model, model, translation.x, translation.y, translation.z);

		Mtx modelView;
		guMtxConcat(core::gMainCamera.GetViewMatrix(), model, modelView);
		GX_LoadPosMtxImm(modelView, GX_PNMTX0);

		Mtx normalMatrix;
		guMtxInvXpose(modelView, normalMatrix);
		GX_LoadNrmMtxImm(normalMatrix, GX_PNMTX0);

		GX_CallDispList(vobj.displayList, vobj.displayListSize);
	}
} // namespace velvet::renderer
