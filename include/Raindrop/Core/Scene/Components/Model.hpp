#ifndef __RAINDROP_CORE_SCENE_COMPONENTS_MODEL_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENTS_MODEL_HPP__

#include <Raindrop/Core/Scene/Component.hpp>
#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Core::Scene::Components{
	class Model : public Component{
		public:
			Model() = default;
			virtual ~Model() override = default;
			
			std::weak_ptr<Graphics::Model> _model;
			std::weak_ptr<Graphics::GraphicsPipeline> _pipeline;
			std::weak_ptr<Graphics::Texture> _texture;
			
			virtual void UI(EngineContext& context) override;

		private:
			void modelUI(EngineContext& context);
			void pipelineUI(EngineContext& context);
			void textureUI(EngineContext& context);
	};
}

#endif