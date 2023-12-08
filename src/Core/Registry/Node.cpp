#include <Raindrop/Core/Registry/Node.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>

namespace Raindrop::Core::Registry{
	Node::Node(Registry& registry, boost::property_tree::ptree &tree, const std::string& path) : 
		_registry{registry},
		_tree{tree},
		_path{path}
	{

	}

	Node::~Node(){

	}
	
	const std::string& Node::path() const{
		return _path;
	}

	template<typename T>
	T Node::get(const std::string& path) const{
		return _registry.get<T>(_path + "." + path);
	}
}