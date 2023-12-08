#ifndef __RAINDROP_CORE_REGISTRY_REGISTRY_HPP__
#define __RAINDROP_CORE_REGISTRY_REGISTRY_HPP__

#include <Raindrop/Core/Registry/common.hpp>
#include <Raindrop/Core/Registry/Node.hpp>
#include <boost/property_tree/ptree.hpp>
#include <muParser.h>

namespace Raindrop::Core::Registry{
	class Registry{
		public:
			Registry(Context& context);
			~Registry();

			void load(const std::filesystem::path& path);

			template<typename T>
			T get(const std::string& base){
				std::string value = format(base);

				if constexpr (std::is_same<T, std::string>::value){
					return value;
				}
				
				if constexpr (std::is_integral<T>::value || std::is_floating_point<T>::value){
					return static_cast<T>(calculateExpresion(value));
				}

				T result;
				std::istringstream iss(value);
    			iss >> result;
				return result;
			}

			template<typename T>
			T getFromNode(const YAML::Node& node, const std::string& d = ""){
				return get<T>(node.as<std::string>(d));
			}

			template<typename T>
			void set(const std::string& path, const T& value){
				std::stringstream iss;
				iss << value;
				_set(path, iss.str());
			}

			bool exists(const std::string& path);

			Node getNode(const std::string& path);
		
		private:
			Context& _context;

			boost::property_tree::ptree _propertyTree;
			mu::Parser _exprParser;

			std::string format(const std::string& base);
			std::string pathToNode(const std::filesystem::path& path);
			double calculateExpresion(const std::string& base);

			void _set(const std::string& path, const std::string& value);

	};
}

#endif