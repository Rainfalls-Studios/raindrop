#ifndef __RAINDROP_CORE_REGISTRY_CONTEXT_HPP__
#define __RAINDROP_CORE_REGISTRY_CONTEXT_HPP__

#include <spdlog/spdlog.h>
#include <Raindrop/Core/Registry/common.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>

namespace Raindrop::Core::Registry{
	class Context{
		public:
			Context(Core::Context& core);
			~Context();

			Core::Context& core();
			spdlog::logger& logger();
			Registry& registry();

		private:
			Core::Context& _core;
			std::shared_ptr<spdlog::logger> _logger;

			std::unique_ptr<Registry> _registry;

	};
}

#endif