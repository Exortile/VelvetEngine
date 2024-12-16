#pragma once

#include <gccore.h>
#include <gctypes.h>
#include <vector>
#include "VtxDescription.hpp"

namespace velvet::core::vtx {

	struct VtxFormat {
		std::vector<VtxDescription> description;
		u8 vtxfmt;

		bool hasPositions : 1 = false;
		bool hasNormals : 1 = false;
		bool hasColors : 1 = false;
		bool hasUVs : 1 = false;

		VtxFormat(const std::vector<VtxDescription> &descs, const u8 fmt) : description(descs), vtxfmt(fmt) {
			for (const auto &desc: descs) {
				switch (desc.desc.attr) {
					case GX_VA_POS: hasPositions = true; break;
					case GX_VA_NRM: hasNormals = true; break;
					case GX_VA_CLR0 ... GX_VA_CLR1: hasColors = true; break;
					case GX_VA_TEX0 ... GX_VA_TEX7: hasUVs = true; break;
					default: break;
				}
			}
		}

		void Apply() const {
			GX_ClearVtxDesc();

			for (const auto &desc: description) {
				desc.ApplyDescription();
				desc.ApplyFormat(vtxfmt);
			}
		}
	};

} // namespace velvet::core::vtx
