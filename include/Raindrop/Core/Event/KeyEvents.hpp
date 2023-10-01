#ifndef __RAINDROP_CORE_EVENT_KEY_EVENTS_HPP__
#define __RAINDROP_CORE_EVENT_KEY_EVENTS_HPP__

#include <Raindrop/Core/Event/common.hpp>

namespace Raindrop::Core::Event{
	class KeyEvents{
		public:
			KeyEvents(Context& context);
			~KeyEvents() = default;

			KeyState& state(Key key);
			const KeyState& state(Key key) const;
			KeyState& operator[](Key key);
			const KeyState& operator[](Key key) const;

			void clear();
			
		private:
			Context& _context;
			KeyState _keys[static_cast<int>(KEY_COUNT)];
	};
}

#endif