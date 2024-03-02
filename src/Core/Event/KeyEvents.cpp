#include <Raindrop/Core/Event/KeyEvents.hpp>

namespace Raindrop::Core::Event{
	KeyEvents::KeyEvents(){
		clear();
	}

	KeyState& KeyEvents::state(Key key){
		return _keys[static_cast<int>(key)];
	}

	const KeyState& KeyEvents::state(Key key) const{
		return _keys[static_cast<int>(key)];
	}

	void KeyEvents::clear(){
		for (int i=0; i<static_cast<int>(KEY_COUNT); i++){
			_keys[i] = KEY_RELEASED;
		}
	}
}