#ifndef __RAINDROP_GRAPHICS_SHADERS_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_SHADERS_CONTEXT_HPP__

#include <spdlog/spdlog.h>

#include <Raindrop/Graphics/Shaders/common.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <Raindrop/Graphics/Shaders/ShaderCompiler.hpp>
#include <Raindrop/Graphics/Shaders/ShaderFactory.hpp>
#include <Raindrop/Graphics/Shaders/GraphicsPipelineManager.hpp>
#include <Raindrop/Graphics/Shaders/PipelineLayoutManager.hpp>
#include <Raindrop/Graphics/Shaders/Loader.hpp>

namespace Raindrop::Graphics::Shaders{
	class Context{
		public:
			Context(Graphics::Context& graphics);
			~Context();

			Graphics::Context& graphics();
			spdlog::logger& logger();
			ShaderCompiler& compiler();
			ShaderFactory& shaderFactory();
			GraphicsPipelineManager& graphicsPipelineManager();
			PipelineLayoutManager& pipelineLayoutManager();
			Loader& loader();

		private:
			Graphics::Context& _graphics;

			std::shared_ptr<spdlog::logger> _logger;
			std::unique_ptr<ShaderCompiler> _compiler;
			std::shared_ptr<ShaderFactory> _shaderFactory;
			std::unique_ptr<GraphicsPipelineManager> _graphicsPipelineManager;
			std::unique_ptr<PipelineLayoutManager> _pipelineLayoutManager;
			std::unique_ptr<Loader> _loader;
	};
}

#endif