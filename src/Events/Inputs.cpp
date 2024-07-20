#include <Raindrop/Events/Inputs.hpp>
#include <Raindrop/Events/Context.hpp>

namespace Raindrop::Events{
	Inputs::Inputs() noexcept :
		_context{nullptr}
	{}

	Inputs::~Inputs(){
		release();
	}

	void Inputs::prepare(Context& context){
		_context = &context;
	}

	void Inputs::initialize(){

	}

	void Inputs::release(){
		_context = nullptr;
	}

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