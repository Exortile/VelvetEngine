#pragma once

#include <array>
#include <gccore.h>

#include "Controller.hpp"

namespace velvet::input {
	[[nodiscard]] Controller &GetController(u8 port);
	extern void UpdateControllers();
} // namespace velvet::input
