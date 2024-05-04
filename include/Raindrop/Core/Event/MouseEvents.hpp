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

			inline Maths::vec2& pos();
			inline const Maths::vec2& pos() const;

			inline Maths::vec2& relPos();
			inline const Maths::vec2& relPos() const;

			inline Maths::vec2& scroll();
			inline const Maths::vec2& scroll() const;

		private:
			ButtonState _states[static_cast<std::size_t>(BUTTON_COUNT)];

			Maths::vec2 _pos;
			Maths::vec2 _relPos;
			Maths::vec2 _scroll;
			
	};
}

#include "MouseEvents.inl"

#endif