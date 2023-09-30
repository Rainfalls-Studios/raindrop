#include <Raindrop/Core/Configuration.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace Raindrop::Core{
	Configuration::Configuration(const std::filesystem::path& configFile){
		boost::property_tree::ptree pt;

		try {
			boost::property_tree::ini_parser::read_ini(configFile.string(), pt);
		} catch (const boost::property_tree::ptree_error& e) {
			throw std::runtime_error(e.what());
		}

		auto get = [&](const char* id){
			try{
				return pt.get<std::string>(id);
			} catch (const boost::property_tree::ptree_error& e){
				std::cerr << e.what() << std::endl;
			}
			return std::string("");
		};

		shaderDirectory = get("Directories.shaderDirectory");
		compiledShaderDirectory = get("Directories.compiledShaderDirectory");
	}
}