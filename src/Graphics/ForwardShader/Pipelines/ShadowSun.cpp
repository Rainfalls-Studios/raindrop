// #include <Raindrop/Graphics/ForwardShader/Pipelines/ShadowSun.hpp>
// #include <Raindrop/Graphics/ForwardShader/PushContants/ShadowSun.hpp>
// #include <Raindrop/Graphics/GraphicsContext.hpp>
// #include <Raindrop/Graphics/Builders/GraphicsPipelineBuilder.hpp>
// #include <Raindrop/Graphics/GraphicsPipeline.hpp>
// #include <Raindrop/Graphics/Shader.hpp>

// namespace Raindrop::Graphics::ForwardShader::Pipelines{
// 	ShadowSun::ShadowSun(GraphicsContext& context, VkDescriptorSetLayout descriptorLayout) : _context{context}{
// 		createPipeline(descriptorLayout);
// 	}

// 	ShadowSun::~ShadowSun(){
// 		_pipeline.reset();
// 	}

// 	void ShadowSun::createPipeline(VkDescriptorSetLayout descriptorLayout){
// 		Builders::GraphicsPipelineBuilder builder;
// 		builder.addShader(std::static_pointer_cast<Shader>(_context.context.assetManager.loadOrGet("C:/Users/aalee/Documents/raindrop/tests/resources/shaders/forwardShading/sun_shadow.glsl.frag.spv").lock()));
// 		builder.addShader(std::static_pointer_cast<Shader>(_context.context.assetManager.loadOrGet("C:/Users/aalee/Documents/raindrop/tests/resources/shaders/forwardShading/forwardShading.glsl.vert.spv").lock()));

// 		builder.setName("forward shading sun with shadow");
// 		builder.setRenderPass(_context.renderPasses.forwardShader.get());
// 		builder.setAttachmentCount(1);

// 		VkPushConstantRange pushConstant;
// 		pushConstant.offset = 0;
// 		pushConstant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
// 		pushConstant.size = sizeof(PushConstants::ShadowSun);

// 		builder.addPushConstant(pushConstant);
// 		builder.addDescriptorSetLayout(descriptorLayout);
// 		builder.addDescriptorSetLayout(_context.layouts.sunShadowMapLayout);

// 		auto& blend = builder.attachmentState(0);
// 		blend.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
// 		blend.blendEnable = VK_TRUE;
// 		blend.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
// 		blend.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
// 		blend.colorBlendOp = VK_BLEND_OP_ADD;
// 		blend.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
// 		blend.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
// 		blend.alphaBlendOp = VK_BLEND_OP_ADD;

// 		builder.setVertexAttribtes({});
// 		builder.setVertexBindings({});

// 		_pipeline = builder.build(_context);
// 	}

// 	GraphicsPipeline& ShadowSun::pipeline(){
// 		return *_pipeline;
// 	}
// }