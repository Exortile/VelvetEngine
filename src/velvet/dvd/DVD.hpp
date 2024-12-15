#pragma once

#include <gccore.h>
#include <string>
#include <optional>

namespace velvet::dvd {
	[[nodiscard]] void *LoadFile(const std::string &path, s32 priority = 2);

	[[nodiscard]] bool LoadFileAsync(const std::string &path, dvdcbcallback doneCallback, s32 priority = 2);

	[[nodiscard]] std::optional<u32> GetFileLength(const std::string &path);
} // namespace velvet::dvd
