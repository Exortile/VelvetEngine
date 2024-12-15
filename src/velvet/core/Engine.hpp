#pragma once

#include <array>
#include <gccore.h>
#include <gctypes.h>
#include "velvet/Renderer.hpp"
#include "velvet/input/Controller.hpp"

namespace velvet::core {

	class Engine {
	public:
		velvet::Renderer renderer;

		void Init();
		static void BeginDraw();
		void EndDraw();

	private:
		u32 _currFb;
		std::array<void *, 2> _xfb;
	};

} // namespace velvet::core
