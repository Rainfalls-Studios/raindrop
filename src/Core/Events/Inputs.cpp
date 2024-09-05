#include <Raindrop/Core/Events/Inputs.hpp>
#include <Raindrop/Core/Events/Context.hpp>

namespace Raindrop::Core::Events{
	Inputs::Inputs(Context& context) noexcept : _context{context}{}

	Inputs::~Inputs(){}

	const KeyStatus& Inputs::getKey(const Key& key) const{
		return _keys[static_cast<std::size_t>(key)];
	}

	const MouseButtonStatus& Inputs::getButton(const MouseButton& button) const{
		return _mouse.buttons[static_cast<std::size_t>(button)];
	}

	const glm::u32vec2& Inputs::getMousePosition() const noexcept{
		return _mouse.position;
	}

	const glm::i32vec2& Inputs::getMouseDelta() const noexcept{
		return _mouse.delta;
	}

	const float& Inputs::getMouseScrollDeltaY() const noexcept{
		return _mouse.scrollDeltaY;
	}
}