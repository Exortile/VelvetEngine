#include "TextureLoader.hpp"
#include "velvet/dvd/DVD.hpp"

namespace {
	void LoadAllFromCubemapTPL(TPLFile &file) {
		GXTexObj texObj;

		TPL_GetTexture(&file, 0, &texObj);
		GX_InitTexObjWrapMode(&texObj, GX_MIRROR, GX_MIRROR);
		GX_LoadTexObj(&texObj, GX_TEXMAP0);

		TPL_GetTexture(&file, 1, &texObj);
		GX_InitTexObjWrapMode(&texObj, GX_MIRROR, GX_MIRROR);
		GX_LoadTexObj(&texObj, GX_TEXMAP1);

		TPL_GetTexture(&file, 2, &texObj);
		GX_InitTexObjWrapMode(&texObj, GX_MIRROR, GX_MIRROR);
		GX_LoadTexObj(&texObj, GX_TEXMAP2);

		TPL_GetTexture(&file, 3, &texObj);
		GX_InitTexObjWrapMode(&texObj, GX_MIRROR, GX_MIRROR);
		GX_LoadTexObj(&texObj, GX_TEXMAP3);

		TPL_GetTexture(&file, 4, &texObj);
		GX_InitTexObjWrapMode(&texObj, GX_MIRROR, GX_MIRROR);
		GX_LoadTexObj(&texObj, GX_TEXMAP4);

		TPL_GetTexture(&file, 5, &texObj);
		GX_InitTexObjWrapMode(&texObj, GX_MIRROR, GX_MIRROR);
		GX_LoadTexObj(&texObj, GX_TEXMAP5);
	}
} // namespace

namespace velvet::core::loaders {
	TPLFile LoadTPL(const char *path) {
		auto file = dvd::LoadFile(path).value();

		TPLFile tpl;
		TPL_OpenTPLFromMemory(&tpl, dvd::GetFileBuffer(file), dvd::GetFileLength(file));

		return tpl;
	}

	void LoadTexFromTPL(TPLFile &file, const s32 id, const u8 texmap) {
		GXTexObj texObj;

		TPL_GetTexture(&file, id, &texObj);
		GX_LoadTexObj(&texObj, texmap);
	}

	void LoadCubemapTexture(const char *path) {
		auto file = dvd::LoadFile(path).value();

		TPLFile tpl;
		TPL_OpenTPLFromMemory(&tpl, dvd::GetFileBuffer(file), dvd::GetFileLength(file));

		LoadAllFromCubemapTPL(tpl);
	}

	void LoadCubemapTexture(TPLFile &file) {
		LoadAllFromCubemapTPL(file);
	}
} // namespace velvet::core::loaders
