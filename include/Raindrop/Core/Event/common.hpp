#ifndef __RAINDROP_CORE_EVENT_COMMON_HPP__
#define __RAINDROP_CORE_EVENT_COMMON_HPP__

#include "../common.hpp"
#include "enums.hpp"

namespace Raindrop::Core::Event{
	using Callback = std::function<void()>;

	class Manager;
	class KeyEvents;
	class MouseEvents;
}

#endif