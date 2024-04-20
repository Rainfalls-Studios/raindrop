#ifndef __RAINDROP_CORE_MODULES_MODULE_HPP__
#define __RAINDROP_CORE_MODULES_MODULE_HPP__

#include "common.hpp"
#include <Raindrop/Core/Assets/Asset.hpp>

namespace Raindrop::Core::Modules{
	class Module : public Assets::Asset{
		public:
			Module(Context& context, const Path& path);
			virtual ~Module();

			ModuleInterface* interface() const;

			/**
			 * @brief returns a const char* array of lenght 'l' that contains each aliases given to the module
			 * 
			 * @param l the number of element in the array
			 * @return const const char** 
			 */
			const char** aliases(std::size_t& l) const;

		private:
			struct __Module__;

			__Module__* _module;
			ModuleInterface* _interface;

	};
}

#endif