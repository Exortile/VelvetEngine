#pragma once

#include <gccore.h>

namespace velvet::core::vtx {

	struct VtxDescription {
		GXVtxDesc desc{};
		struct {
			u8 type;
			u8 size;
		} fmt{};

		VtxDescription(u8 descAttr, u8 descType, u8 fmtType, u8 fmtSize) noexcept : desc(descAttr, descType), fmt(fmtType, fmtSize) {
		}

		void ApplyDescription() const {
			GX_SetVtxDesc(desc.attr, desc.type);
		}

		void ApplyFormat(const u8 vtxfmt) const {
			GX_SetVtxAttrFmt(vtxfmt, desc.attr, fmt.type, fmt.size, 0);
		}
	};

} // namespace velvet::core::vtx
