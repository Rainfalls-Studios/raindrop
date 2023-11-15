#ifndef __RAINDROP_EVENT_HPP__
#define __RAINDROP_EVENT_HPP__

#include <Raindrop/common.hpp>
#include <Raindrop/Core/Event/common.hpp>

namespace Raindrop{
	using EventCallback = Core::Event::Callback;
	using Key = Core::Event::Key;
	using KeyState = Core::Event::KeyState;
	using MouseButton = Core::Event::MouseButton;
	using ButtonState = Core::Event::ButtonState;
}

#endif