#include <Raindrop/Core/Event/MouseEvents.hpp>

namespace Raindrop::Core::Event{
	MouseEvents::MouseEvents(){
		for (int i=0; i<static_cast<int>(BUTTON_COUNT); i++){
			_states[i] = BUTTON_DOWN;
		}
	}

	ButtonState& MouseEvents::state(MouseButton button){
		return _states[static_cast<int>(button)];
	}

	const ButtonState& MouseEvents::state(MouseButton button) const{
		return _states[static_cast<int>(button)];
	}
}