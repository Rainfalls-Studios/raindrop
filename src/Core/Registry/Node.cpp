#include <Raindrop/Core/Registry/Node.hpp>
#include <Raindrop/Core/Registry/Context.hpp>

namespace Raindrop::Core::Registry{
	Node::Node(const Node& node) : _context{node._context}{
		_impl = std::make_unique<Impl>(node._impl->tree); 
	}

	Node::Node(Context& context, Impl impl) : _context{context}{
		_impl = std::make_unique<Impl>(impl.tree);
	}

	Node::Node(Context& context) : _context{context}{
		_impl = std::make_unique<Impl>(std::make_shared<boost::property_tree::ptree>());
	}

	Node::~Node(){
		_impl.reset();
	}
	
	bool Node::same(const Node& other) const{
		return _impl->tree == other._impl->tree;
	}

	bool Node::different(const Node& other) const{
		return _impl->tree != other._impl->tree;
	}

	void Node::load(const std::string& path){
		_context.logger().info("Loading registry file from %s", path.c_str());

		try{
			boost::property_tree::read_json(path, *_impl->tree);
		} catch (const boost::property_tree::json_parser_error& e){
			_context.logger().error("Failed to load registry from %s : %s", path.c_str(), e.what());
			return;
		}

		_context.logger().info("Loaded %s with success !", path.c_str());
	}

	void Node::save(const std::string& path){
		_context.logger().info("Saving registry file at %s", path.c_str());

		try{
			boost::property_tree::write_json(path, *_impl->tree);
		} catch (const boost::property_tree::json_parser_error& e){
			_context.logger().error("Failed to save registry at %s : %s", path.c_str(), e.what());
			return;
		}
		
		_context.logger().info("Saved %s with success !", path.c_str());
	}

	std::string Node::format(const std::string& str){
		std::string result = str;
		size_t pos = 0;
		while ((pos = result.find("${", pos)) != std::string::npos) {
			size_t endPos = result.find("}", pos);
			if (endPos != std::string::npos) {
				std::string placeholder = result.substr(pos + 2, endPos - pos - 2);
				try {
					std::string path = placeholder;
					std::replace(path.begin(), path.end(), '.', '/');
					std::string value = _impl->tree->get_child(path).data();
					result.replace(pos, endPos - pos + 1, value);
					pos += value.length();

				} catch (const boost::property_tree::ptree_error&) {
					pos = endPos + 1;
				}
			} else {
				pos += 2;
			}
		}
		return result;
	}

	Node Node::get(const std::string& path){
		auto node = _impl->tree->get_child_optional(path);
		if (node.has_value()) return Node(_context, std::shared_ptr<boost::property_tree::ptree>(node.get_ptr()));
		return Node(_context, std::shared_ptr<boost::property_tree::ptree>(&_impl->tree->add_child(path, {})));
	}

	const Node Node::get(const std::string& path) const{
		return Node(_context, std::shared_ptr<boost::property_tree::ptree>(&_impl->tree->get_child(path)));
	}

	void Node::remove(const std::string& path){
		_impl->tree->erase(path);
	}
	
	void Node::sort(){
		_impl->tree->sort();
	}

	void Node::revers(){
		_impl->tree->reverse();
	}

	void Node::clear(){
		_impl->tree->clear();
	}

	uint32_t Node::size() const{
		return static_cast<uint32_t>(_impl->tree->size());
	}

	bool Node::empty() const{
		return _impl->tree->empty();
	}

	std::string& Node::data(){
		return _impl->tree->data();
	}

	const std::string& Node::data() const{
		return _impl->tree->data();
	}
}