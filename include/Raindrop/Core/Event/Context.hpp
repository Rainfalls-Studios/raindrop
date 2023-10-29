#ifndef __RAINDROP_CORE_EVENT_CONTEXT_HPP__
#define __RAINDROP_CORE_EVENT_CONTEXT_HPP__

#include <spdlog/spdlog.h>
#include <Raindrop/Core/Event/common.hpp>

namespace Raindrop::Core::Event{
	class Context{
		public:
			Context(Core::Context& context);

			Core::Context& core();
			spdlog::logger& logger();
		
		private:
			Core::Context& _core;
			spdlog::logger _logger;

	};
}

#endif