#include "Renderer.hpp"
#include "velvet/core/Engine.hpp"
#include "velvet/core/Transform.hpp"
#include "velvet/core/vtx/VtxFormat.hpp"
#include "velvet/math/Matrix.hpp"

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

	constinit std::array<s8, 24> SkyboxCubePositions alignas(32) = {
			// x y z

			1,	1,	-1, // 0
			1,	1,	1,	// 1
			1,	-1, 1,	// 2
			1,	-1, -1, // 3

			-1, 1,	1,	// 4
			-1, 1,	-1, // 5
			-1, -1, -1, // 6
			-1, -1, 1,	// 7
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
					core::vtx::VtxDescription(GX_VA_TEX0, GX_DIRECT, GX_TEX_ST, GX_F32),
			},
			GX_VTXFMT0);

	const core::vtx::VtxFormat VtxFormatSkybox(
			{
					core::vtx::VtxDescription(GX_VA_POS, GX_INDEX8, GX_POS_XYZ, GX_S8),
					core::vtx::VtxDescription(GX_VA_TEX0, GX_DIRECT, GX_TEX_ST, GX_S8),
			},
			GX_VTXFMT0);

	const core::vtx::VtxFormat VtxFormatColor(
			{
					core::vtx::VtxDescription(GX_VA_POS, GX_INDEX8, GX_POS_XYZ, GX_S16),
					core::vtx::VtxDescription(GX_VA_CLR0, GX_INDEX8, GX_CLR_RGBA, GX_RGBA8),
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

	void SetVObjVtxFormat(const formats::VObject &vobj) {
		std::vector<core::vtx::VtxDescription> vtxFormat;

		vtxFormat.emplace_back(GX_VA_POS, GX_INDEX16, GX_POS_XYZ, GX_F32);
		GX_SetArray(GX_VA_POS, vobj.vertexData->positions, 3 * sizeof(f32));

		if (vobj.hasNormals) {
			vtxFormat.emplace_back(GX_VA_NRM, GX_INDEX16, GX_NRM_XYZ, GX_F32);
			GX_SetArray(GX_VA_NRM, vobj.vertexData->normals, 3 * sizeof(f32));
		}

		if (vobj.hasColors) {
			vtxFormat.emplace_back(GX_VA_CLR0, GX_INDEX16, GX_CLR_RGB, GX_RGB8);
			GX_SetArray(GX_VA_CLR0, vobj.vertexData->colors, 3 * sizeof(u8));
		}

		if (vobj.hasUVs) {
			vtxFormat.emplace_back(GX_VA_TEX0, GX_INDEX16, GX_TEX_ST, GX_F32);
			GX_SetArray(GX_VA_TEX0, vobj.vertexData->uvs, 2 * sizeof(f32));
		}

		core::vtx::VtxFormat(vtxFormat, GX_VTXFMT0).Apply();
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

	void DrawSingleTexturedQuadIndexed(
			const u8 vtxfmt,
			const u8 p0,
			const u8 p1,
			const u8 p2,
			const u8 p3) {
		GX_Begin(GX_QUADS, vtxfmt, 4);

		GX_Position1x8(p0);
		GX_TexCoord2s8(0, 0);

		GX_Position1x8(p1);
		GX_TexCoord2s8(1, 0);

		GX_Position1x8(p2);
		GX_TexCoord2s8(1, 1);

		GX_Position1x8(p3);
		GX_TexCoord2s8(0, 1);

		GX_End();
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

	void DrawColoredCube(const core::Transform &transform) {
		SetVtxFormatAndDesc(VtxFormatColor);

		GX_SetNumChans(1);

		GX_SetNumTexGens(0);
		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
		GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

		GX_SetArray(GX_VA_POS, CubePositions.data(), 3 * sizeof(s16));
		GX_SetArray(GX_VA_CLR0, CubeColors.data(), 4 * sizeof(u8));

		Mtx modelView;
		transform.toModelMatrix(modelView);
		guMtxConcat(core::gMainCamera.GetViewMatrix(), modelView, modelView);

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

	void DrawTexturedCube(const u8 texmap, const core::Transform &transform) {
		SetVtxFormatAndDesc(VtxFormatTex);

		GX_SetNumChans(1);

		GX_SetNumTexGens(1);
		GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);
		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, texmap, GX_COLOR0A0);
		GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

		GX_SetArray(GX_VA_POS, CubePositions.data(), 3 * sizeof(s16));

		Mtx modelView;
		transform.toModelMatrix(modelView);
		guMtxConcat(core::gMainCamera.GetViewMatrix(), modelView, modelView);

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

	void DrawSkybox() {
		SetVtxFormatAndDesc(VtxFormatSkybox);

		GX_LoadTexObj(&renderer::gWhiteZTexObj, GX_TEXMAP6);

		// Setup a couple of settings for Z textures to work
		GX_SetZCompLoc(GX_FALSE);
		GX_SetZTexture(GX_ZT_REPLACE, GX_TF_Z24X8, 0);

		GX_SetNumChans(0);
		GX_SetNumTexGens(1);
		GX_SetNumTevStages(2); // 2nd stage for Z texture

		// Setup TEV for cubemap and last TEV for Z texture
		GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);

		GX_SetTevOp(GX_TEVSTAGE1, GX_PASSCLR);
		GX_SetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP6, GX_COLORNULL);

		GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

		// Since the Z texture is a different size (fullscreen), we need another texcoord generator
		GX_SetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

		GX_SetArray(GX_VA_POS, SkyboxCubePositions.data(), 3 * sizeof(s8));

		Mtx viewNoTrans;
		std::memcpy(viewNoTrans, core::gMainCamera.GetViewMatrix(), sizeof(Mtx));
		math::matrix::RemoveTranslation(viewNoTrans);

		GX_LoadPosMtxImm(viewNoTrans, GX_PNMTX0);
		GX_SetCurrentMtx(GX_PNMTX0);

		// Draw each cube face with the corresponding texture

		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLORNULL);
		DrawSingleTexturedQuadIndexed(VtxFormatSkybox.vtxfmt, 0, 1, 2, 3);

		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLORNULL);
		DrawSingleTexturedQuadIndexed(VtxFormatSkybox.vtxfmt, 4, 5, 6, 7);

		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP2, GX_COLORNULL);
		DrawSingleTexturedQuadIndexed(VtxFormatSkybox.vtxfmt, 4, 1, 0, 5);

		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP3, GX_COLORNULL);
		DrawSingleTexturedQuadIndexed(VtxFormatSkybox.vtxfmt, 6, 3, 2, 7);

		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP4, GX_COLORNULL);
		DrawSingleTexturedQuadIndexed(VtxFormatSkybox.vtxfmt, 5, 0, 3, 6);

		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP5, GX_COLORNULL);
		DrawSingleTexturedQuadIndexed(VtxFormatSkybox.vtxfmt, 1, 4, 7, 2);

		// Reset settings
		GX_SetZTexture(GX_ZT_DISABLE, GX_TF_Z24X8, 0);
		GX_SetZCompLoc(GX_TRUE);
	}

	void DrawTexturedVObj(const formats::VObject &vobj, const core::Transform &transform) {
		SetVObjVtxFormat(vobj);

		Mtx modelView;
		transform.toModelMatrix(modelView);
		guMtxConcat(core::gMainCamera.GetViewMatrix(), modelView, modelView);

		GX_LoadPosMtxImm(modelView, GX_PNMTX0);

		Mtx normalMatrix;
		guMtxInvXpose(modelView, normalMatrix);
		GX_LoadNrmMtxImm(normalMatrix, GX_PNMTX0);

		GX_CallDispList(vobj.displayList, vobj.displayListSize);
	}

	void SetLight(const guVector &position, const f32 brightness, const GXColor &color) {
		guVector lightPos;
		guMtxMultVec(core::gMainCamera.GetViewMatrix(), &position, &lightPos);

		GXLightObj lightObj;
		GX_InitLightPos(&lightObj, lightPos.x, lightPos.y, lightPos.z);
		GX_InitLightDistAttn(&lightObj, 0.5f, brightness, GX_DA_GENTLE);
		GX_InitLightColor(&lightObj, color);
		GX_LoadLightObj(&lightObj, GX_LIGHT0);
	}

	void SetNoLight() {
		GX_SetNumChans(0);
		GX_SetChanCtrl(
				GX_COLOR0A0,
				GX_DISABLE,
				GX_SRC_REG,
				GX_SRC_REG,
				GX_LIGHTNULL,
				GX_DF_NONE,
				GX_AF_NONE);
	}

	void SetMaterial(const GXColor &diffuseColor, const GXColor &ambientColor) {
		GX_SetNumChans(1);
		GX_SetChanCtrl(
				GX_COLOR0A0,
				GX_ENABLE,
				GX_SRC_REG,
				GX_SRC_REG,
				GX_LIGHT0,
				GX_DF_CLAMP,
				GX_AF_NONE);
		GX_SetChanMatColor(GX_COLOR0A0, diffuseColor);
		GX_SetChanAmbColor(GX_COLOR0A0, ambientColor);
	}

	void SetTexture(const u8 texmap, const bool useLighting) {
		GX_SetNumTexGens(1);
		GX_SetNumTevStages(1);
		GX_SetTevOp(GX_TEVSTAGE0, useLighting ? GX_MODULATE : GX_REPLACE);
		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, texmap, GX_COLOR0A0);
		GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
	}

	[[maybe_unused]] u16 GetWidth() {
		return gGXRMode->fbWidth;
	}

	[[maybe_unused]] u16 GetHeight() {
		return gGXRMode->efbHeight;
	}

} // namespace velvet::renderer
