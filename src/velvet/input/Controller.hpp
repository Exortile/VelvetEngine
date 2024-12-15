#pragma once

#include <gccore.h>
#include <gctypes.h>
#include <optional>
#include <tuple>

namespace velvet::input {
	namespace controller {
		struct Stick {
			s8 x = 0;
			s8 y = 0;
		};

		[[nodiscard]] inline bool IsConnectedBitOn(const u32 connectedBits, const u32 port) {
			return (connectedBits & (1 << port)) != 0;
		}
	} // namespace controller

	class Controller {
	public:
		explicit Controller(u8 port);

		void Update(bool connected);

		[[nodiscard]] bool IsButtonHeld(u16 button) const;
		[[nodiscard]] bool IsButtonDown(u16 button) const;
		[[nodiscard]] bool IsButtonUp(u16 button) const;
		[[nodiscard]] std::optional<std::tuple<controller::Stick, controller::Stick>> GetSticks() const;
		[[nodiscard]] bool IsConnected() const;

	private:
		u8 _port;
		bool _isConnected = false;

		/// For buttons that are currently being held down.
		u16 _heldButtons = 0;

		/// For buttons that were pressed down this frame.
		u16 _downButtons = 0;

		/// For buttons that were released from being held this frame.
		u16 _upButtons = 0;

		controller::Stick _lStick;
		controller::Stick _cStick;
	};
} // namespace velvet::input
