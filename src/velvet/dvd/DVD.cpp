#include "DVD.hpp"
#include <malloc.h>

// TODO: Implement searching files through directories in libogc, right now only files that are in the root directory are found.

namespace velvet::dvd {
	[[nodiscard]] void *LoadFile(const std::string &path, const s32 priority) {
		dvdfileinfo fileInfo;
		if (!DVD_Open(path.c_str(), &fileInfo))
			return nullptr;

		auto buf = memalign(32, fileInfo.len);
		DVD_ReadPrio(&fileInfo, buf, fileInfo.len, 0, priority);

		DVD_Close(&fileInfo);
		return buf;
	}

	[[nodiscard]] bool LoadFileAsync(const std::string &path, dvdcbcallback doneCallback, const s32 priority) {
		dvdfileinfo fileInfo;
		if (!DVD_Open(path.c_str(), &fileInfo))
			return false;

		auto buf = memalign(32, fileInfo.len);
		DVD_ReadAsyncPrio(&fileInfo, buf, fileInfo.len, 0, doneCallback, priority);

		return true;
	}

	[[nodiscard]] std::optional<u32> GetFileLength(const std::string &path) {
		dvdfileinfo info;
		if (!DVD_Open(path.c_str(), &info))
			return std::nullopt;

		return info.len;
	}
} // namespace velvet::dvd
