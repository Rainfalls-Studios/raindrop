#include <Raindrop/Core/Event/KeyEvents.hpp>

namespace Raindrop::Core::Event{
	KeyEvents::KeyEvents(){
		clear();
	}

	KeyState& KeyEvents::state(Key key){
		return _keys[static_cast<std::size_t>(key)];
	}

	const KeyState& KeyEvents::state(Key key) const{
		return _keys[static_cast<std::size_t>(key)];
	}

	void KeyEvents::clear(){
		for (std::size_t i=0; i<static_cast<std::size_t>(KEY_COUNT); i++){
			_keys[i] = KEY_RELEASED;
		}
	}
}