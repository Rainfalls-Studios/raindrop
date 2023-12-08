#ifndef __RAINDROP_GRAPHICS_SHADERS_GRAPHICS_PIPELINE_HPP__
#define __RAINDROP_GRAPHICS_SHADERS_GRAPHICS_PIPELINE_HPP__

#include <Raindrop/Graphics/Shaders/common.hpp>
#include <Raindrop/Core/Asset/Asset.hpp>

namespace Raindrop::Graphics::Shaders{
	class GraphicsPipeline : public Core::Asset::Asset{
		public:
			GraphicsPipeline(Context& context, VkGraphicsPipelineCreateInfo info, VkPipelineLayoutCreateInfo layoutInfo, std::vector<std::shared_ptr<Shader>> shaders, std::string name);
			virtual ~GraphicsPipeline() override;

			void bind(VkCommandBuffer commandBuffer);
			
			VkPipeline pipeline() const;
			VkPipelineLayout layout() const;
			const std::string& name() const;

		private:
			Context& _context;
			std::string _name;
			std::vector<std::shared_ptr<Shader>> _shaders;

			VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
			VkPipeline _pipeline = VK_NULL_HANDLE;
	};
}

#endif