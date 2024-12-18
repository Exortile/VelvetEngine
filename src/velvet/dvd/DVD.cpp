#include "DVD.hpp"
#include <malloc.h>

// TODO: Implement searching files through directories in libogc, right now only files that are in the root directory are found.

namespace velvet::dvd {
	[[nodiscard]] std::optional<dvdfileinfo> LoadFile(const char *path, s32 priority) {
		dvdfileinfo fileInfo;
		if (!DVD_Open(path, &fileInfo))
			return std::nullopt;

		auto buf = memalign(32, fileInfo.len);
		DVD_ReadPrio(&fileInfo, buf, fileInfo.len, 0, priority);

		DVD_Close(&fileInfo);
		return fileInfo;
	}

	[[nodiscard]] bool LoadFileAsync(const char *path, dvdcbcallback doneCallback, s32 priority) {
		dvdfileinfo fileInfo;
		if (!DVD_Open(path, &fileInfo))
			return false;

		auto buf = memalign(32, fileInfo.len);
		DVD_ReadAsyncPrio(&fileInfo, buf, fileInfo.len, 0, doneCallback, priority);

		return true;
	}

	[[nodiscard]] std::optional<u32> GetFileLength(const char *path) {
		dvdfileinfo info;
		if (!DVD_Open(path, &info))
			return std::nullopt;

		return info.len;
	}
} // namespace velvet::dvd
