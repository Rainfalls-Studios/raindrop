#ifndef __RAINDROP_EVENTS_EVENT_HPP__
#define __RAINDROP_EVENTS_EVENT_HPP__

namespace Raindrop::Events{
	class Event{
		public:
			Event() = default;
			virtual ~Event() = default;
	};
}

#endif