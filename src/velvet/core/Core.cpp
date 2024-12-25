#include "Core.hpp"
#include "defines.hpp"

#include <cstdlib>
#include <cstring>
#include <gccore.h>
#include <malloc.h>

CDefined u64 gettime();

namespace {
	[[maybe_unused]] void SetupVtxFormats() {
		GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
		GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
		GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
		GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	}
} // namespace

namespace velvet::core {
	void Init() {
		VIDEO_Init();
		PAD_Init();
		DVD_Init();
		DVD_Mount();

		auto rmode = VIDEO_GetPreferredMode(nullptr);
		renderer::gGXRMode = rmode;
		VIDEO_Configure(rmode);

		auto gpFifo = memalign(32, GX_FIFO_MINSIZE);
		std::memset(gpFifo, 0, GX_FIFO_MINSIZE);

		auto &[currFb, xfb] = velvet::renderer::gXfbData;

		xfb[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
		xfb[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

		VIDEO_SetNextFramebuffer(xfb[currFb]);
		VIDEO_SetBlack(false);
		VIDEO_Flush();
		VIDEO_WaitVSync();
		if (rmode->viTVMode & VI_NON_INTERLACE)
			VIDEO_WaitVSync();

		// Initialize the console
		CON_Init(
				xfb[currFb],
				20,
				20,
				rmode->fbWidth,
				rmode->xfbHeight,
				rmode->fbWidth * VI_DISPLAY_PIX_SZ);

		GX_Init(gpFifo, GX_FIFO_MINSIZE);

		currFb ^= 1;

		GX_SetCopyClear({0, 0, 0, 255}, GX_MAX_Z24);

		auto yScale = GX_GetYScaleFactor(rmode->efbHeight, rmode->xfbHeight);
		auto xfbHeight = static_cast<u16>(GX_SetDispCopyYScale(yScale));
		GX_SetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
		GX_SetDispCopyDst(rmode->fbWidth, xfbHeight);
		GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
		GX_SetFieldMode(
				rmode->field_rendering,
				((rmode->viHeight == 2 * rmode->xfbHeight) ? GX_ENABLE : GX_DISABLE));

		GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
		GX_SetAlphaUpdate(GX_TRUE);
		GX_SetColorUpdate(GX_TRUE);

		if (rmode->aa)
			GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
		else
			GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);

		GX_SetCullMode(GX_CULL_BACK);
		GX_CopyDisp(xfb[currFb], GX_TRUE);

		Mtx44 projection;
		guPerspective(projection, 60, 4.f / 3.f, 0.1f, 300.f);
		GX_LoadProjectionMtx(projection, GX_PERSPECTIVE);

		srand(static_cast<u32>(gettime()));

		// Create a fully white texture (max Z texture) for skybox
		auto texBufSize = GX_GetTexBufferSize(1, 1, GX_TF_Z24X8, GX_FALSE, 0);
		renderer::gWhiteTexData = malloc(texBufSize);
		std::memset(renderer::gWhiteTexData, 0xFF, texBufSize);

		GX_InitTexObj(
				&renderer::gWhiteZTexObj,
				renderer::gWhiteTexData,
				1,
				1,
				GX_TF_Z24X8,
				GX_CLAMP,
				GX_CLAMP,
				GX_FALSE);

		GX_InitTexObjLOD(
				&renderer::gWhiteZTexObj,
				GX_NEAR,
				GX_NEAR,
				0.f,
				0.f,
				0.f,
				GX_FALSE,
				GX_FALSE,
				GX_ANISO_1);
	}
} // namespace velvet::core
