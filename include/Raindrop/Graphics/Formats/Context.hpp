#ifndef __RAINDROP_GRAPHICS_FORMATS_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_FORMATS_CONTEXT_HPP__

#include <Raindrop/Graphics/Formats/common.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Formats/FormatRegistry.hpp>
#include <Raindrop/Graphics/Formats/Loader.hpp>
#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::Formats{
	class Context{
		public:
			Context(Graphics::Context& graphics);
			~Context();

			Graphics::Context& graphics();
			spdlog::logger& logger();
			Loader& loader();
			FormatRegistry& registry();
		
		private:
			Graphics::Context& _graphics;

			std::shared_ptr<spdlog::logger> _logger;
			std::shared_ptr<Loader> _loader;
			std::unique_ptr<FormatRegistry> _registry;
	};
}

#endif