#ifndef __RIANDROP_GRAPHICS_SHADERS_PIPELINE_LAYOUT_BUILDER_HPP__
#define __RIANDROP_GRAPHICS_SHADERS_PIPELINE_LAYOUT_BUILDER_HPP__

#include <Raindrop/Graphics/Shaders/common.hpp>

namespace Raindrop::Graphics::Shaders{
	class PipelineLayoutBuilder{
		public:
			PipelineLayoutBuilder(Context& context);
			~PipelineLayoutBuilder();

			void loadFromNode(const YAML::Node& node);

			void setFlags(const VkPipelineLayoutCreateFlags& flags);
			const VkPipelineLayoutCreateFlags& flags() const;

			void setName(const std::string& name);
			const std::string& name() const;

			const VkPushConstantRange* pushConstants() const;
			uint32_t pushConstantCount() const;

			const VkDescriptorSetLayout* setLayouts() const;
			uint32_t setLayoutCount() const;


		private:
			Context& _context;

			std::string _name;
			VkPipelineLayoutCreateFlags _flags;
			std::vector<VkDescriptorSetLayout> _sets;
			std::vector<VkPushConstantRange> _pushConstants;

			void loadDescriptorSet(const YAML::Node& node);
			void loadPushConstant(const YAML::Node& node);
	};
}

#endif