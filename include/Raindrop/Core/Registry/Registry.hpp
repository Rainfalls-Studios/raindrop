#ifndef __RAINDROP_CORE_REGISTRY_REGISTRY_HPP__
#define __RAINDROP_CORE_REGISTRY_REGISTRY_HPP__

#include <Raindrop/Core/Registry/common.hpp>

namespace Raindrop::Core::Registry{
	class Registry{
		public:
			Registry() = default;
			Registry(const std::any& val) : _value{val}{}
			~Registry() = default;

			Registry& get(std::string name);
			const Registry& get(std::string name) const;

			Registry& operator[](const std::string& name){
				return get(name);
			}

			const Registry& operator[](const std::string& name) const{
				return get(name);
			}

			void remove(std::string name);
			std::string formatString(std::string str);

			template<typename T>
			T& as(){
				return std::any_cast<T&>(_value);
			}

			template<typename T>
			Registry& operator=(const T& t){
				_value = t;
				return *this;
			}

		private:
			std::unordered_map<std::string, Registry> _nameToRegistry;
			std::any _value;
	};
}

#endif