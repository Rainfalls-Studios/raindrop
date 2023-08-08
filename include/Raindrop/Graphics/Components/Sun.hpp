#ifndef __RAINDROP_GRAPHICS_COMPONENTS_SUN_HPP__
#define __RAINDROP_GRAPHICS_COMPONENTS_SUN_HPP__

#include <Raindrop/Graphics/Components/common.hpp>
#include <Raindrop/Graphics/ShadowMap/Sun/common.hpp>

namespace Raindrop::Graphics::Components{
	class Sun : public Component{
		public:
			Sun(GraphicsContext& context);
			virtual ~Sun() override;
			
			bool hasShadowMap() const;
			const glm::vec3 color() const;
			
			const ShadowMap::Sun::ShadowMap& shadowMap() const;
			VkDescriptorSet descriptorSet() const;
			
			virtual void UI(Core::EngineContext& context) override;
		
		private:
			GraphicsContext& _context;
			glm::vec3 _color;
			std::unique_ptr<ShadowMap::Sun::ShadowMap> _shadowMap;
			VkDescriptorSet _descriptorSet;

			void createDescriptorSet();
			void destroyDescriptorSet();
			void updateDescriptorSet();

			void createShadowMap();
			void destroyShadowMap();
	};
}

#endif