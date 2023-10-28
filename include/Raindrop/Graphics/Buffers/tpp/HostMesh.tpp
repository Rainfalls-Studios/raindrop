template<typename T>
T& HostMesh::get(const std::string& attribute, std::size_t vertex){
	return *static_cast<T*>(get(attribute, vertex));
}

template<typename T>
const T& HostMesh::get(const std::string& attribute, std::size_t vertex) const{
	return *static_cast<T*>(get(attribute, vertex));
}