#ifndef __RAINDROP_CORE_EVENT_MOUSE_EVENTS_HPP__
#define __RAINDROP_CORE_EVENT_MOUSE_EVENTS_HPP__

#include "common.hpp"

namespace Raindrop::Core::Event{
	class MouseEvents{
		public:
			MouseEvents();
			~MouseEvents() = default;

			ButtonState& state(MouseButton button);
			const ButtonState& state(MouseButton button) const;

			Maths::vec2& pos();
			const Maths::vec2& pos() const;

			Maths::vec2& relPos();
			const Maths::vec2& relPos() const;

			Maths::vec2& scroll();
			const Maths::vec2& scroll() const;

		private:
			ButtonState _states[static_cast<std::size_t>(BUTTON_COUNT)];

			Maths::vec2 _pos;
			Maths::vec2 _relPos;
			Maths::vec2 _scroll;
			
	};
}

#endif