#ifndef __RAINDROP_CORE_EVENT_MOUSE_EVENTS_HPP__
#define __RAINDROP_CORE_EVENT_MOUSE_EVENTS_HPP__

#include <Raindrop/Core/Event/common.hpp>

namespace Raindrop::Core::Event{
	class MouseEvents{
		public:
			MouseEvents(Context& context);
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
			Context& _context;
			ButtonState _states[static_cast<int>(BUTTON_COUNT)];

			glm::vec2 _pos;
			glm::vec2 _relPos;
			glm::vec2 _scroll;
			
	};
}

#endif