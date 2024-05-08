#ifndef __RAINDROP_INTERNAL_EVENTS_COMMON_HPP__
#define __RAINDROP_INTERNAL_EVENTS_COMMON_HPP__

#include "../common.hpp"

namespace Raindrop::Internal::Events{
	using EventID = std::size_t;
	static constexpr EventID INVALID_EVENT_ID = EventID(~0ULL);

	class Manager;
}

#endif