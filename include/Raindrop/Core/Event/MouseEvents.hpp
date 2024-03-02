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

			glm::vec2& pos();
			const glm::vec2& pos() const;

			glm::vec2& relPos();
			const glm::vec2& relPos() const;

			glm::vec2& scroll();
			const glm::vec2& scroll() const;

		private:
			ButtonState _states[static_cast<std::size_t>(BUTTON_COUNT)];

			glm::vec2 _pos;
			glm::vec2 _relPos;
			glm::vec2 _scroll;
			
	};
}

#endif