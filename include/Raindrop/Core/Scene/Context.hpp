#ifndef __RAINDROP_CORE_SCENE_CONTEXT_HPP__
#define __RAINDROP_CORE_SCENE_CONTEXT_HPP__

#include <spdlog/spdlog.h>
#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core::Scene{
	class Context{
		public:
			Context(Core::Context& core);

			Core::Context& core();
			spdlog::logger& logger();

		private:
			Core::Context& _core;
			std::shared_ptr<spdlog::logger> _logger;

	};
}

#endif