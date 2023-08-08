#ifndef __RAINDROP_GRAPHICS_FORWARD_SHADER_DESCRIPTOR_SET_HPP__
#define __RAINDROP_GRAPHICS_FORWARD_SHADER_DESCRIPTOR_SET_HPP__

#include <Raindrop/Graphics/ForwardShader/common.hpp>

namespace Raindrop::Graphics::ForwardShader{
	class DescriptorSet{
		public:
			DescriptorSet(GraphicsContext& context);
			~DescriptorSet();

			VkDescriptorSet set() const;
			VkDescriptorSetLayout layout() const;

			void update(WorldFramebuffer& worldFramebuffer);

		private:
			GraphicsContext& _context;

			std::shared_ptr<DescriptorSetLayout> _setLayout;
			VkDescriptorSet _set;

			void createSetLayout();
			void createSet();

	};
}

#endif