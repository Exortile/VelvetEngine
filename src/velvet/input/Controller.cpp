#include "Controller.hpp"

namespace velvet::input {
	Controller::Controller(const u8 port) : _port(port) {
	}

	void Controller::Update(const bool connected) {
		_isConnected = connected;

		if (!_isConnected)
			return;

		_heldButtons = PAD_ButtonsHeld(_port);
		_downButtons = PAD_ButtonsDown(_port);
		_upButtons = PAD_ButtonsUp(_port);
		_lStick.x = PAD_StickX(_port);
		_lStick.y = PAD_StickY(_port);
		_cStick.x = PAD_SubStickX(_port);
		_cStick.y = PAD_SubStickY(_port);
	}

	bool Controller::IsButtonHeld(u16 button) const {
		return _isConnected && (_heldButtons & button) == button;
	}

	bool Controller::IsButtonDown(u16 button) const {
		return _isConnected && (_downButtons & button) == button;
	}

	bool Controller::IsButtonUp(u16 button) const {
		return _isConnected && (_upButtons & button) == button;
	}

	std::optional<std::tuple<controller::Stick, controller::Stick>> Controller::GetSticks() const {
		if (!_isConnected)
			return std::nullopt;

		return {{_lStick, _cStick}};
	}

	bool Controller::IsConnected() const {
		return _isConnected;
	}
} // namespace velvet::input
