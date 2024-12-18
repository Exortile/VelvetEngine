#include "ModelLoader.hpp"
#include <cstring>

namespace velvet::core::loaders {
	std::optional<formats::VObject *> InitVOBJ(void *file) {
		using namespace formats::util;

		auto infoHeader = reinterpret_cast<formats::VHeader *>(file);

		if (!ValidateHeader(*infoHeader, formats::id::INFO))
			return std::nullopt;

		CalculateOffset(file, &infoHeader->nextHeader);

		auto vinfo = GetHeaderData<formats::VInfo *>(infoHeader);
		if (!vinfo->isModelFile)
			return std::nullopt;

		auto objHeader = infoHeader->nextHeader;
		if (!ValidateHeader(*objHeader, formats::id::OBJECT))
			return std::nullopt;

		CalculateOffset(file, &objHeader->nextHeader);

		auto vobj = GetHeaderData<formats::VObject *>(objHeader);
		vobj->calculateOffsets(file);

		auto curHeader = objHeader->nextHeader;
		char id[5];
		while (!ValidateHeader(*curHeader, formats::id::END)) {
			CalculateOffset(file, &curHeader->nextHeader);

			GetIDTerminated(*curHeader, id);
			if (std::strcmp(id, formats::id::VERTEX_DATA) == 0) {
				// Setup offsets for vertex data
				auto vvtx = GetHeaderData<formats::VVertexData *>(curHeader);
				vvtx->calculateOffsets(file);
			}

			curHeader = curHeader->nextHeader;
		}

		return vobj;
	}
} // namespace velvet::core::loaders
