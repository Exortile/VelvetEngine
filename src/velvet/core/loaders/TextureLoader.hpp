#pragma once

#include <gccore.h>

namespace velvet::core::loaders {
	TPLFile LoadTPL(const char *path);
	void LoadTexFromTPL(TPLFile &file, const s32 id, const u8 texmap);

	void LoadCubemapTexture(const char *path);
	void LoadCubemapTexture(TPLFile &file);
} // namespace velvet::core::loaders
