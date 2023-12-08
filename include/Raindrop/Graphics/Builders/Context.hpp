#ifndef __RAINDROP_GRAPHICs_BUILDERS_CONTEXT_HPP__
#define __RAINDROP_GRAPHICs_BUILDERS_CONTEXT_HPP__

#include <spdlog/spdlog.h>

#include <Raindrop/Graphics/Builders/common.hpp>
#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics::Builders{
	class Context{
		public:
			Context(Graphics::Context& context);
			~Context();

			Graphics::Context& graphics();
			spdlog::logger& logger();

		private:
			Graphics::Context& _graphics;
			std::shared_ptr<spdlog::logger> _logger;

	};
}

#endif