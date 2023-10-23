#ifndef __RAINDROP_CORE_EVENT_COMMON_HPP__
#define __RAINDROP_CORE_EVENT_COMMON_HPP__

#include <Raindrop/Core/common.hpp>
#include <Raindrop/Core/Event/enums.hpp>

namespace Raindrop::Core::Event{
	using Callback = std::function<void()>;
	
	struct Context;
	class EventManager;
	class KeyEvents;
	class MouseEvents;
}

#endif