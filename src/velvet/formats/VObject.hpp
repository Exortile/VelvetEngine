#pragma once

#include <gccore.h>
#include "VelvetCommon.hpp"

namespace velvet::formats {

	struct VMaterials {
		u32 version;

		ColorRGB diffuse;
		ColorRGB specular;

		f32 shininess;
	};

	struct VVertexData {
		u32 version;

		void *positions;
		void *normals;
		void *uvs;
		void *colors;

		void calculateOffsets(void *start) {
			if (positions != nullptr)
				util::CalculateOffset(start, &positions);
			if (normals != nullptr)
				util::CalculateOffset(start, &normals);
			if (uvs != nullptr)
				util::CalculateOffset(start, &uvs);
			if (colors != nullptr)
				util::CalculateOffset(start, &colors);
		}
	};

	struct VObject {
		u32 version;

		u32 hasNormals : 1;
		u32 hasUVs : 1;
		u32 hasColors : 1;

		VVertexData *vertexData;
		VMaterials *materials;

		void *displayList;
		u32 displayListSize;

		void calculateOffsets(void *start) {
			util::CalculateOffset(start, &vertexData);
			util::CalculateOffset(start, &materials);
			util::CalculateOffset(start, &displayList);
		}
	};

} // namespace velvet::formats
