#ifndef __RAINDROP_CORE_EVENT_MOUSE_EVENTS_INL__
#define __RAINDROP_CORE_EVENT_MOUSE_EVENTS_INL__

#include "MouseEvents.hpp"

namespace Raindrop::Core::Event{
	inline Maths::vec2& MouseEvents::pos(){
		return _pos;
	}

	inline const Maths::vec2& MouseEvents::pos() const{
		return _pos;
	}

	inline Maths::vec2& MouseEvents::relPos(){
		return _relPos;
	}

	inline const Maths::vec2& MouseEvents::relPos() const{
		return _relPos;
	}

	inline Maths::vec2& MouseEvents::scroll(){
		return _scroll;
	}

	inline const Maths::vec2& MouseEvents::scroll() const{
		return _scroll;
	}
}

#endif