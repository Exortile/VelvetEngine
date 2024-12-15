#include "Input.hpp"

namespace {
	std::array<velvet::input::Controller, PAD_CHANMAX> Controllers{
			velvet::input::Controller(PAD_CHAN0),
			velvet::input::Controller(PAD_CHAN1),
			velvet::input::Controller(PAD_CHAN2),
			velvet::input::Controller(PAD_CHAN3),
	};
}

namespace velvet::input {
	Controller &GetController(const u8 port) {
		return Controllers[port];
	}

	void UpdateControllers() {
		const auto connected = PAD_ScanPads();

		for (u32 port = 0; port < Controllers.size(); port++) {
			auto &controller = Controllers[port];

			controller.Update(input::controller::IsConnectedBitOn(connected, port));
		}
	}
} // namespace velvet::input
