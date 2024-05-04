#ifndef __RAINDROP_CORE_EVENT_KEY_EVENTS_INL__
#define __RAINDROP_CORE_EVENT_KEY_EVENTS_INL__

#include "KeyEvents.hpp"

namespace Raindrop::Core::Event{
	inline KeyState& KeyEvents::operator[](Key key){
		return state(key);
	}

	inline const KeyState& KeyEvents::operator[](Key key) const{
		return state(key);
	}
}

#endif