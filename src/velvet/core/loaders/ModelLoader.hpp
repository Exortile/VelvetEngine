#pragma once

#include <gccore.h>
#include <optional>
#include "velvet/formats/VObject.hpp"

namespace velvet::core::loaders {
	std::optional<formats::VObject *> InitVOBJ(void *file);
}
