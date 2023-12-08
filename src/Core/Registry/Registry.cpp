#include <Raindrop/Core/Registry/Registry.hpp>
#include <Raindrop/Core/Registry/Context.hpp>

#include <regex>
#include <boost/type.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/info_parser.hpp>

namespace Raindrop::Core::Registry{
	Registry::Registry(Context& context) : _context{context}{

	}
	
	Registry::~Registry(){

	}

	Node Registry::getNode(const std::string& path){
		return Node(*this, _propertyTree.get_child(path), path);
	}

	std::string Registry::pathToNode(const std::filesystem::path& path){
		std::string str = path;	
		std::replace(str.begin(), str.end(), '/', '.');
		std::replace(str.begin(), str.end(), '\\', '.');
		return str;
	}

	void Registry::load(const std::filesystem::path& path){
		_context.logger().info("Loading {} into the registry", path.string());
		
		auto extention = path.extension();

		if (!std::filesystem::exists(path)){
			_context.logger().error("Cannot load {}, the file does not exists", path.string());
			throw std::runtime_error("Non existant file");
		}

		std::string nodePath = pathToNode(path);
		auto& tree = _propertyTree.add_child(nodePath, boost::property_tree::ptree());

		if (extention == ".json"){
			boost::property_tree::read_json(path, tree);
		} else if (extention == ".xml"){
			boost::property_tree::read_xml(path, tree);
		} else if (extention == ".ini"){
			boost::property_tree::read_ini(path, tree);
		} else if (extention == ".info"){
			boost::property_tree::read_info(path, tree);
		} else {
			_context.logger().error("Cannot load {}, the file extensions {} is not supported", path.string(), extention.string());
			throw std::runtime_error("invalid file extention");
		}
	}

	std::string Registry::format(const std::string& base){
		std::regex pattern("\\$\\{([^{}]*)\\}");
		std::smatch match;
		std::string result = base;

		while (std::regex_search(result, match, pattern)) {
			std::string placeholder = match[0];
			std::string key = match[1];

			auto child = _propertyTree.get_child_optional(key);
			if (child.has_value()){
				result.replace(match.position(), placeholder.length(), child->data());
			} else {
				result = result.substr(match.position() + placeholder.length());
			}
		}
		return result;
	}

	double Registry::calculateExpresion(const std::string& base){
		_exprParser.SetExpr(base);
		return _exprParser.Eval();
	}

	bool Registry::exists(const std::string& path){
		return _propertyTree.get_child_optional(path).has_value();
	}

	void Registry::_set(const std::string& path, const std::string& value){
		_propertyTree.add_child(path, boost::property_tree::ptree(value));
	}
}