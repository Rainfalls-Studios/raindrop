#include <Raindrop/Graphics/Factory/Factories.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

#include <Raindrop/Graphics/Factory/GraphicsPipelineFactory.hpp>
#include <Raindrop/Graphics/Factory/ModelFactory.hpp>
#include <Raindrop/Graphics/Factory/ShaderFactory.hpp>
#include <Raindrop/Graphics/Factory/TextureFactory.hpp>

namespace Raindrop::Graphics::Factory{
	Factories::Factories(GraphicsContext& context){
		auto& assetManager = context.context.assetManager;

		assetManager.registerFactory<GraphicsPipelineFactory, GraphicsContext&>(context);
		assetManager.registerFactory<ModelFactory, GraphicsContext&>(context);
		assetManager.registerFactory<ShaderFactory, GraphicsContext&>(context);
		assetManager.registerFactory<TextureFactory, GraphicsContext&>(context);
	}
}