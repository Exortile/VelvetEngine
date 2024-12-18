#pragma once

#include <gccore.h>
#include <optional>
#include <string>

namespace velvet::dvd {
	[[nodiscard]] std::optional<dvdfileinfo> LoadFile(const char *path, s32 priority = 2);

	[[nodiscard]] bool LoadFileAsync(const char *path, dvdcbcallback doneCallback, s32 priority = 2);

	[[nodiscard]] std::optional<u32> GetFileLength(const char *path);
} // namespace velvet::dvd
