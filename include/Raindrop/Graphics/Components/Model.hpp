#ifndef __RAINDROP_GRAPHICS_COMPONENTS_MODEL_HPP__
#define __RAINDROP_GRAPHICS_COMPONENTS_MODEL_HPP__

#include <Raindrop/Graphics/Components/common.hpp>

namespace Raindrop::Graphics::Components{
	class Model : public Component{
		public:
			Model(GraphicsContext& context);
			virtual ~Model() override;

			Graphics::Texture* texture() const;
			Graphics::Model* model() const;
			VkDescriptorSet descriptorSet() const;

			void updateTexture(std::weak_ptr<Graphics::Texture> texture);

			virtual void UI(Core::EngineContext& context) override;

		private:
			GraphicsContext& _context;

			std::weak_ptr<Graphics::Texture> _texture;
			std::weak_ptr<Graphics::Model> _model;
			VkDescriptorSet _descriptorSet;

			void modelUI();
			void textureUI();
	};
};

#endif