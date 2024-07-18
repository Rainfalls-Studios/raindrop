#ifndef __RAINDROP_EVENTS_LISTENER_HPP__
#define __RAINDROP_EVENTS_LISTENER_HPP__

namespace Raindrop::Events{
	class Listener{
		public:
			Listener() = default;
			virtual ~Listener() = default;
	};
}

#endif