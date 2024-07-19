#ifndef __RAINDROP_EVENTS_MOUSE_EVENTS_HPP__
#define __RAINDROP_EVENTS_MOUSE_EVENTS_HPP__

#include "WindowEvents.hpp"

namespace Raindrop::Events{
	enum MouseButton{
		BUTTON_LEFT = 0,
		BUTTON_MIDDLE = 1,
		BUTTON_RIGH = 2
	};

	class MouseMovedEvent : public WindowEvent{
		public:
			MouseMovedEvent(Window& window, const glm::u32vec2& position, const glm::i32vec2& delta) :
				WindowEvent(window),
				_position{position},
				_delta{delta}
			{}

			~MouseMovedEvent() = default;

			const glm::u32vec2& getPosition() const noexcept{
				return _position;
			}

			const glm::i32vec2& getDelta() const noexcept{
				return _delta;
			}

		protected:
			glm::u32vec2 _position;
			glm::i32vec2 _delta;
	};

	class MouseButtonDownEvent : public WindowEvent{
		public:
			MouseButtonDownEvent(Window& window, const MouseButton& button, const std::uint32_t& clicks, const glm::u32vec2& position) : 
				WindowEvent(window),
				_button{button},
				_clicks{clicks},
				_position{position}
			{}

			~MouseButtonDownEvent() = default;

			const MouseButton& getButton() const noexcept{
				return _button;
			}

			const std::uint32_t& getClicks() const noexcept{
				return _clicks;
			}

			const glm::u32vec2& getPosition() const noexcept{
				return _position;
			}

		private:
			MouseButton _button;
			std::uint32_t _clicks;
			glm::u32vec2 _position;
	};

	class MouseButtonUpEvent : public WindowEvent{
		public:
			MouseButtonUpEvent(Window& window, const MouseButton& button, const glm::u32vec2& position) : 
				WindowEvent(window),
				_button{button},
				_position{position}
			{}

			~MouseButtonUpEvent() = default;

			const MouseButton& getButton() const noexcept{
				return _button;
			}

			const glm::u32vec2& getPosition() const noexcept{
				return _position;
			}

		private:
			MouseButton _button;
			glm::u32vec2 _position;
	};

	class MouseScrollEvent : public WindowEvent{
		public:
			MouseScrollEvent(Window& window, const float& y) :
				WindowEvent(window),
				_y{y}
			{}

			const float& getY() const noexcept{
				return _y;
			}

		private:
			float _y;
	};
}

#endif