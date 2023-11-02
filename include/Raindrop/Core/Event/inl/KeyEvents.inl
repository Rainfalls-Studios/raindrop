inline KeyState& KeyEvents::operator[](Key key){
	return state(key);
}

inline const KeyState& KeyEvents::operator[](Key key) const{
	return state(key);
}