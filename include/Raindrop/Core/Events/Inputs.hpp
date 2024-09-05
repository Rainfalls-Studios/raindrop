#pragma once

#include "KeyEvents.hpp"
#include "MouseEvents.hpp"
#include "WindowEvents.hpp"

namespace Raindrop::Core::Events{
	class Context;
	
	class Inputs{
		public:
			Inputs(Context& context) noexcept;
			~Inputs();

			const KeyStatus& getKey(const Key& key) const;

			const MouseButtonStatus& getButton(const MouseButton& button) const;
			const glm::u32vec2& getMousePosition() const noexcept;
			const glm::i32vec2& getMouseDelta() const noexcept;
			const float& getMouseScrollDeltaY() const noexcept;
			

		private:
			Context& _context;

			// Keys
			KeyStatus _keys[static_cast<std::size_t>(__KEY_COUNT__)];

			// Mouse
			struct{
				MouseButtonStatus buttons[static_cast<std::size_t>(__BUTTON_COUNT__)];
				
				glm::u32vec2 position;
				glm::i32vec2 delta;
				float scrollDeltaY;
			} _mouse;

			// Window
	};
}