#pragma once

#include <cstring>
#include <gctypes.h>

namespace velvet::formats {

	namespace id {
		constexpr auto INFO = "VINF";
		constexpr auto OBJECT = "VOBJ";
		constexpr auto VERTEX_DATA = "VVTX";
		constexpr auto MATERIALS = "VMAT";

		constexpr auto GENERAL_DATA = "VDAT";

		constexpr auto END = "VEND";
	} // namespace id

	struct ColorRGB {
		u8 r;
		u8 g;
		u8 b;
	};

	struct VHeader {
		char id[4];
		VHeader *nextHeader;
		[[maybe_unused]] char _pad[8];
	};

	struct VInfo {
		u32 version;

		// File type
		u32 isModelFile : 1;
		u32 isSceneFile : 1;
	};

	namespace util {
		inline void CopyID(formats::VHeader &src, char &dest) {
			std::memcpy(&dest, src.id, sizeof(formats::VHeader::id));
		}

		/**
		 * Converts a offset in the file into a valid pointer from the given start address.
		 *
		 * @param start The start address to calculate the offset from
		 * @param offset The address of the offset, modified in place
		 */
		inline void CalculateOffset(const void *start, void *offset) {
			// cursed casts but need to be done
			const auto offsetAddr = reinterpret_cast<u32 *>(offset);
			const auto calcOffset = *offsetAddr;
			*offsetAddr = reinterpret_cast<u32>(reinterpret_cast<const u8 *>(start) + calcOffset);
		}

		inline void GetIDTerminated(formats::VHeader &header, char *outID) {
			outID[4] = 0;
			CopyID(header, *outID);
		}

		[[nodiscard]] inline bool ValidateHeader(formats::VHeader &header, const char *id) {
			char idTerminated[5];
			GetIDTerminated(header, idTerminated);
			return std::strcmp(idTerminated, id) == 0;
		}

		template<typename T>
		[[nodiscard]] inline T GetHeaderData(formats::VHeader *header) {
			return reinterpret_cast<T>(reinterpret_cast<u8 *>(header) + sizeof(formats::VHeader));
		}
	} // namespace util

} // namespace velvet::formats
