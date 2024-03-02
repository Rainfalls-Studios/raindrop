#ifndef __RAINDROP_CORE_EVENT_KEY_EVENTS_HPP__
#define __RAINDROP_CORE_EVENT_KEY_EVENTS_HPP__

#include "common.hpp"

namespace Raindrop::Core::Event{
	class KeyEvents{
		public:
			KeyEvents();
			~KeyEvents() = default;

			KeyState& state(Key key);
			const KeyState& state(Key key) const;

			inline KeyState& operator[](Key key){
				return state(key);
			}

			inline const KeyState& operator[](Key key) const{
				return state(key);
			}

			void clear();
			
		private:
			KeyState _keys[static_cast<int>(KEY_COUNT)];
	};
}

#endif