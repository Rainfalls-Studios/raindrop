#ifndef __RAINDROP_GRAPHICS_SHADERS_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_SHADERS_CONTEXT_HPP__

#include <spdlog/spdlog.h>

#include <Raindrop/Graphics/Shaders/common.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <Raindrop/Graphics/Shaders/ShaderCompiler.hpp>
#include <Raindrop/Graphics/Shaders/ShaderFactory.hpp>

namespace Raindrop::Graphics::Shaders{
	class Context{
		public:
			Context(Graphics::Context& graphics);
			~Context();

			Graphics::Context& graphics();
			spdlog::logger& logger();
			ShaderCompiler& compiler();
			ShaderFactory& shaderFactory();

		private:
			Graphics::Context& _graphics;

			std::shared_ptr<spdlog::logger> _logger;
			std::unique_ptr<ShaderCompiler> _compiler;
			std::shared_ptr<ShaderFactory> _shaderFactory;
	};
}

#endif