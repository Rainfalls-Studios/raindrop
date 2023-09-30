// #include <Raindrop/Graphics/ShadowMap/Sun/Pipeline.hpp>
// #include <Raindrop/Graphics/GraphicsContext.hpp>
// #include <Raindrop/Graphics/GraphicsPipeline.hpp>
// #include <Raindrop/Graphics/Builders/GraphicsPipelineBuilder.hpp>
// #include <Raindrop/Graphics/Shader.hpp>

// namespace Raindrop::Graphics::ShadowMap::Sun{
// 	Pipeline::Pipeline(GraphicsContext& context) : _context{context}{
// 		createPipeline();
// 	}

// 	Pipeline::~Pipeline(){
// 		_pipeline.reset();
// 	}

// 	void Pipeline::createPipeline(){
// 		Builders::GraphicsPipelineBuilder builder;
// 		builder.addShader(std::static_pointer_cast<Shader>(_context.context.assetManager.loadOrGet("C:/Users/aalee/Documents/raindrop/tests/resources/shaders/shadowMaps/sunShadowMap.glsl.frag.spv").lock()));
// 		builder.addShader(std::static_pointer_cast<Shader>(_context.context.assetManager.loadOrGet("C:/Users/aalee/Documents/raindrop/tests/resources/shaders/shadowMaps/sunShadowMap.glsl.vert.spv").lock()));

// 		builder.setName("sun shadow map");
// 		builder.setRenderPass(_context.renderPasses.sun.get());

// 		builder.setAttachmentCount(1);

// 		// builder.setVertexAttribtes({});
// 		// builder.setVertexBindings({});:

// 		_pipeline = builder.build(_context);
// 	}

// 	GraphicsPipeline& Pipeline::get() const{
// 		return *_pipeline;
// 	}
// }