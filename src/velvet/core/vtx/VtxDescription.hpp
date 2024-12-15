#pragma once

#include <gccore.h>

namespace velvet::core::vtx {
	struct VtxDescription {
		GXVtxDesc desc{};
		struct {
			u8 type;
			u8 size;
		} fmt{};

		VtxDescription(u8 descAttr, u8 descType, u8 fmtType, u8 fmtSize) {
			desc.attr = descAttr;
			desc.type = descType;
			fmt.type = fmtType;
			fmt.size = fmtSize;
		}
	};
}
