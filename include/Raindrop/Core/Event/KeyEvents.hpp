#ifndef __RAINDROP_CORE_EVENT_KEY_EVENTS_HPP__
#define __RAINDROP_CORE_EVENT_KEY_EVENTS_HPP__

#include "common.hpp"

namespace Raindrop::Core::Event{
	class KeyEvents{
		public:
			KeyEvents();

			KeyState& state(Key key);
			const KeyState& state(Key key) const;

			inline KeyState& operator[](Key key);
			inline const KeyState& operator[](Key key) const;

			void clear();
			
		private:
			KeyState _keys[static_cast<int>(KEY_COUNT)];
	};
}

#include "KeyEvents.inl"

#endif