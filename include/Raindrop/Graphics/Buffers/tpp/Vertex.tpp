template<typename T>
T& Vertex::get(const std::string& attribute){
	return *static_cast<T*>(get(name, vertex));
}

template<typename T>
const T& Vertex::get(const std::string& attribute) const{
	return *static_cast<T*>(get(name));
}