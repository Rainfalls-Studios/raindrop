inline Node Node::operator[](const std::string& path){
	return get(path);
}

inline const Node Node::operator[](const std::string& path) const{
	return get(path);
}

inline bool Node::operator==(const Node& other) const{
	return same(other);
}

inline bool Node::operator!=(const Node& other) const{
	return different(other);
}