#ifndef __RAINDROP_GRAPHICS_UTILS_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_UTILS_CONTEXT_HPP__

#include <spdlog/spdlog.h>

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/Utils/common.hpp>
#include <Raindrop/Graphics/Utils/FormatUtilities.hpp>
#include <Raindrop/Graphics/Utils/StringToVulkan.hpp>

namespace Raindrop::Graphics::Utils{
	class Context{
		public:
			Context(Graphics::Context& graphics);
			
			Graphics::Context& graphics();
			spdlog::logger& logger();

			FormatUtilities& formats();

		private:
			Graphics::Context& _graphics;

			std::shared_ptr<spdlog::logger> _logger;
			std::unique_ptr<FormatUtilities> _formats;
	};
}

#include <Raindrop/Graphics/Context.hpp>

#endif