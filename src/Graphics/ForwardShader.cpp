#include <Raindrop/Graphics/ForwardShader.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Graphics/builders/DescriptorPoolBuilder.hpp>
#include <Raindrop/Graphics/builders/DescriptorSetLayoutBuilder.hpp>
#include <Raindrop/Graphics/DescriptorPool.hpp>
#include <Raindrop/Graphics/DescriptorSetLayout.hpp>
#include <Raindrop/Graphics/WorldFramebuffer.hpp>
#include <Raindrop/Graphics/Shader.hpp>
#include <Raindrop/Graphics/builders/GraphicsPipelineBuilder.hpp>
#include <Raindrop/Graphics/GraphicsPipeline.hpp>
#include <Raindrop/Core/Scene/Components/LightPoint.hpp>
#include <Raindrop/Core/Scene/Components/Spotlight.hpp>
#include <Raindrop/Core/Scene/Components/Sun.hpp>
#include <Raindrop/Core/Scene/Components/Transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Raindrop::Graphics{

	struct SpotlightPushConstant{
		alignas(16) glm::vec3 cameraPosition;
		alignas(16) glm::vec3 cameraDirection;
		alignas(16) glm::vec3 color;
		alignas(16) glm::vec3 position;
		alignas(16) glm::vec3 direction;
		float maxAngle;
		float intensity;
	};

	struct LightPointPushConstant{
		alignas(16) glm::vec3 cameraPosition;
		alignas(16) glm::vec3 cameraDirection;
		alignas(16) glm::vec3 color;
		alignas(16) glm::vec3 position;
		float intensity;
	};

	struct SunPushConstant{
		alignas(16) glm::vec3 cameraPosition;
		alignas(16) glm::vec3 cameraDirection;
		alignas(16) glm::vec3 color;
		alignas(16) glm::vec3 direction;
	};

	struct AttachmentInfo{
		VkAttachmentDescription description;
		VkImageViewCreateInfo imageView;
		VkImageCreateInfo image;

		// if none of the formats are available, throw an exception
		std::vector<VkFormat> formats; // ordered from best to worst case
		VkFormatFeatureFlags requiredFeatures;
		VkClearValue clear;
	};

	static AttachmentInfo attachment = {
		.description = VkAttachmentDescription{	
			.flags = 0,
			.format = VK_FORMAT_UNDEFINED,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		},

		.imageView = VkImageViewCreateInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.image = VK_NULL_HANDLE, // Set afterward
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = VK_FORMAT_UNDEFINED,
			.components = VkComponentMapping{},
			.subresourceRange = VkImageSubresourceRange{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
		},

		.image = VkImageCreateInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = VK_FORMAT_UNDEFINED,
			.extent = VkExtent3D{0, 0, 1}, // Set afterward
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,	// Same, set afterward
			.queueFamilyIndexCount = 0,					// again.
			.pQueueFamilyIndices = nullptr,				// again
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		},

		.formats = {
			VK_FORMAT_R8G8B8A8_UNORM,
			VK_FORMAT_R8G8B8A8_UINT,
			VK_FORMAT_B8G8R8A8_UNORM,
			VK_FORMAT_A8B8G8R8_UINT_PACK32,
			VK_FORMAT_R8G8B8A8_SRGB,
		},

		.requiredFeatures = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT,

		.clear = VkClearValue{
			.color = VkClearColorValue{
				.uint32 = {0U, 0U, 0U, 0U},
			}
		}
	};

	ForwardShader::ForwardShader(GraphicsContext& context, uint32_t width, uint32_t height) : _context{context}, _width{width}, _height{height}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.ForwadShader");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		CLOG(INFO, "Engine.Graphics.ForwardShader") << "Creating forward shader...";

		checkDefaultFormats();
		createRenderPass();
		createAttachment();
		createFramebuffer();

		createDescriptorPool();
		createSetLayout();
		createDescriptorSet();
		createLightPointPipeline();
		createSpotlightPipeline();
		createSunPipeline();

		CLOG(INFO, "Engine.Graphics.ForwardShader") << "Forward shader created with success !";
	}

	void ForwardShader::destroyResizable(){
		if (_framebuffer) vkDestroyFramebuffer(_context.device.get(), _framebuffer, _context.allocationCallbacks);
		if (_imageView) vkDestroyImageView(_context.device.get(), _imageView, _context.allocationCallbacks);
		if (_image) vkDestroyImage(_context.device.get(), _image, _context.allocationCallbacks);
		if (_memory) vkFreeMemory(_context.device.get(), _memory, _context.allocationCallbacks);
		if (_sampler) vkDestroySampler(_context.device.get(), _sampler, _context.allocationCallbacks);

	}

	ForwardShader::~ForwardShader(){
		destroyResizable();

		vkResetDescriptorPool(_context.device.get(), _descriptorPool->get(), 0);
		_descriptorPool.reset();
		_setLayout.reset();

		if (_renderPass) vkDestroyRenderPass(_context.device.get(), _renderPass, _context.allocationCallbacks);
	}

	void ForwardShader::resize(uint32_t width, uint32_t height){
		_width = width;
		_height = height;

		destroyResizable();
		createAttachment();
		createFramebuffer();
	}

	void ForwardShader::updateDescriptor(WorldFramebuffer& worldFramebuffer){

		VkDescriptorImageInfo infos[3];
		VkWriteDescriptorSet writes[3];

		infos[0] = worldFramebuffer.getAttachmentInfo(1);
		infos[1] = worldFramebuffer.getAttachmentInfo(3);
		infos[2] = worldFramebuffer.getAttachmentInfo(4); 
		
		for (int i=0; i<sizeof(writes)/sizeof(VkWriteDescriptorSet); i++){
			writes[i] = {};
			writes[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writes[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writes[i].dstBinding = i;
			writes[i].pImageInfo = &infos[i];
			writes[i].descriptorCount = 1;
			writes[i].dstSet = _descriptorSet;
		}

		vkUpdateDescriptorSets(_context.device.get(), sizeof(writes)/sizeof(VkWriteDescriptorSet), writes, 0, nullptr);	
	}

	VkRenderPass ForwardShader::renderPass() const{
		return _renderPass;
	}

	uint32_t ForwardShader::attachmentCount() const{
		return 1;
	}

	void ForwardShader::createRenderPass(){
		VkAttachmentReference attachmentRef{};
		attachmentRef.attachment = 0;
		attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &attachmentRef;
		subpass.pDepthStencilAttachment = nullptr;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.srcAccessMask = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstSubpass = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		VkAttachmentDescription description = attachment.description;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &description;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(_context.device.get(), &renderPassInfo, _context.allocationCallbacks, &_renderPass) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.ForwardShader") << "Failed to create forward shader render pass";
			throw std::runtime_error("Failed to create forward shader render pass");
		}
	}

	void ForwardShader::createAttachment(){
		createImage();
		createImageView();
		createSampler();
	}

	void ForwardShader::createFramebuffer(){
		VkFramebufferCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

		info.width = _width;
		info.height = _height;
		info.layers = 1;

		info.pAttachments = &_imageView;
		info.attachmentCount = 1;
		info.renderPass = _renderPass;
		
		if (vkCreateFramebuffer(_context.device.get(), &info, _context.allocationCallbacks, &_framebuffer) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.WorldFramebuffer") << "Failed to create world framebuffer";
			throw std::runtime_error("Failed to create world framebuffer");
		}
	}

	void ForwardShader::createImage(){
		VkImageCreateInfo imageInfo{};
		imageInfo = attachment.image;
		imageInfo.extent.width = _width;
		imageInfo.extent.height = _height;
		
		uint32_t familyIndices[] = {_context.graphicsFamily};

		imageInfo.pQueueFamilyIndices = familyIndices;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.queueFamilyIndexCount = sizeof(familyIndices) / sizeof(uint32_t);

		if (vkCreateImage(_context.device.get(), &imageInfo, _context.allocationCallbacks, &_image) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.ForwardShader") << "Failed to create forward shader attachment image";
			throw std::runtime_error("Failed to create forward shader attachment image");
		}

		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(_context.device.get(), _image, &requirements);

		VkMemoryAllocateInfo allocationInfo{};
		allocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocationInfo.memoryTypeIndex = _context.device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		allocationInfo.allocationSize = requirements.size;
		

		if (vkAllocateMemory(_context.device.get(), &allocationInfo, _context.allocationCallbacks, &_memory) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.ForwardShader") << "Failed to allocate forward shader attachment memory";
			throw std::runtime_error("Failed to allocate forward shader attachment memory");
		}

		if (vkBindImageMemory(_context.device.get(), _image, _memory, 0) != VK_SUCCESS){
			CLOG(ERROR, "Engine.graphics.ForwardShader") << "Failed to bind forward shader attachment image memory";
			throw std::runtime_error("Failed to bind forward shader attachment image memory");
		}
	}

	void ForwardShader::createImageView(){
		VkImageViewCreateInfo imageViewInfo{};
		imageViewInfo = attachment.imageView;
		imageViewInfo.image = _image;

		if (vkCreateImageView(_context.device.get(), &imageViewInfo, _context.allocationCallbacks, &_imageView) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.ForwardShader") << "Failed to create forward shader attachment image view";
			throw std::runtime_error("Failed to create forward shader attachment image view");
		}
	}

	void ForwardShader::createSampler(){
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.anisotropyEnable = VK_FALSE;
		samplerInfo.maxAnisotropy = 1.0f;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;
		samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;

		if (vkCreateSampler(_context.device.get(), &samplerInfo, _context.allocationCallbacks, &_sampler) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.ForwardShader") << "Failed to create forward shader attachment sampler";
			throw std::runtime_error("Failed to create forward shader sampler");
		}
	}
	
	void ForwardShader::createDescriptorPool(){
		Builders::DescriptorPoolBuilder builder;
		builder.setMaxSets(1);
		builder.pushPoolSize({VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 3});

		_descriptorPool = builder.build(_context);

	}

	void ForwardShader::createSetLayout(){
		Builders::DescriptorSetLayoutBuilder builder;

		for (int i=0; i<3; i++){
			VkDescriptorSetLayoutBinding binding{};
			binding.binding = i;
			binding.descriptorCount = 1;
			binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
			binding.pImmutableSamplers = nullptr;
			builder.pushBinding(binding);
		}

		_setLayout = builder.build(_context);
	}

	void ForwardShader::createDescriptorSet(){
		VkDescriptorSetAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.descriptorSetCount = 1;
		info.descriptorPool = _descriptorPool->get();
		
		VkDescriptorSetLayout layout = _setLayout->get();
		info.pSetLayouts = &layout;
		
		if (vkAllocateDescriptorSets(_context.device.get(), &info, &_descriptorSet) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.ForwardShader") << "Failed to create forward shader descriptor set";
			throw std::runtime_error("Failed to create forward shader descriptor set");
		}
	}
	
	void ForwardShader::createLightPointPipeline(){
		Builders::GraphicsPipelineBuilder builder;
		builder.addShader(std::static_pointer_cast<Shader>(_context.context.assetManager.loadOrGet("C:/Users/aalee/Documents/raindrop/tests/resources/shaders/forwardShading/lightPoint.glsl.frag.spv").lock()));
		builder.addShader(std::static_pointer_cast<Shader>(_context.context.assetManager.loadOrGet("C:/Users/aalee/Documents/raindrop/tests/resources/shaders/forwardShading/forwardShading.glsl.vert.spv").lock()));

		builder.setName("forward shading");
		builder.setRenderPass(_renderPass);

		builder.addDescriptorSetLayout(_setLayout->get());
		builder.setAttachmentCount(1);

		VkPushConstantRange pushConstant;
		pushConstant.offset = 0;
		pushConstant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstant.size = sizeof(LightPointPushConstant);

		builder.addPushConstant(pushConstant);

		builder.setAttachmentCount(1);
		auto& blend = builder.attachmentState(0);
		blend.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		blend.blendEnable = VK_TRUE;
		blend.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;    // Source color factor (alpha premultiplied)
		blend.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;          // Destination color factor (alpha premultiplied)
		blend.colorBlendOp = VK_BLEND_OP_ADD;                     // Blend operation for color components
		blend.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;          // Source alpha factor
		blend.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;          // Destination alpha factor
		blend.alphaBlendOp = VK_BLEND_OP_ADD;                     // Blend operation for alpha component

		builder.setVertexAttribtes({});
		builder.setVertexBindings({});

		_lightPointPipeline = builder.build(_context);
	}

	void ForwardShader::createSpotlightPipeline(){
		Builders::GraphicsPipelineBuilder builder;
		builder.addShader(std::static_pointer_cast<Shader>(_context.context.assetManager.loadOrGet("C:/Users/aalee/Documents/raindrop/tests/resources/shaders/forwardShading/spotlight.glsl.frag.spv").lock()));
		builder.addShader(std::static_pointer_cast<Shader>(_context.context.assetManager.loadOrGet("C:/Users/aalee/Documents/raindrop/tests/resources/shaders/forwardShading/forwardShading.glsl.vert.spv").lock()));

		builder.setName("forward shading");
		builder.setRenderPass(_renderPass);

		builder.addDescriptorSetLayout(_setLayout->get());
		builder.setAttachmentCount(1);

		VkPushConstantRange pushConstant;
		pushConstant.offset = 0;
		pushConstant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstant.size = sizeof(SpotlightPushConstant);

		builder.addPushConstant(pushConstant);

		builder.setAttachmentCount(1);
		auto& blend = builder.attachmentState(0);
		blend.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		blend.blendEnable = VK_TRUE;
		blend.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;    // Source color factor (alpha premultiplied)
		blend.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;          // Destination color factor (alpha premultiplied)
		blend.colorBlendOp = VK_BLEND_OP_ADD;                     // Blend operation for color components
		blend.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;          // Source alpha factor
		blend.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;          // Destination alpha factor
		blend.alphaBlendOp = VK_BLEND_OP_ADD;                     // Blend operation for alpha component

		builder.setVertexAttribtes({});
		builder.setVertexBindings({});

		_spotlightPipeline = builder.build(_context);
	}

	void ForwardShader::createSunPipeline(){
		Builders::GraphicsPipelineBuilder builder;
		builder.addShader(std::static_pointer_cast<Shader>(_context.context.assetManager.loadOrGet("C:/Users/aalee/Documents/raindrop/tests/resources/shaders/forwardShading/sun.glsl.frag.spv").lock()));
		builder.addShader(std::static_pointer_cast<Shader>(_context.context.assetManager.loadOrGet("C:/Users/aalee/Documents/raindrop/tests/resources/shaders/forwardShading/forwardShading.glsl.vert.spv").lock()));

		builder.setName("forward shading");
		builder.setRenderPass(_renderPass);

		builder.addDescriptorSetLayout(_setLayout->get());
		builder.setAttachmentCount(1);

		VkPushConstantRange pushConstant;
		pushConstant.offset = 0;
		pushConstant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstant.size = sizeof(SunPushConstant);

		builder.addPushConstant(pushConstant);

		builder.setAttachmentCount(1);
		auto& blend = builder.attachmentState(0);
		blend.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		blend.blendEnable = VK_TRUE;
		blend.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;    // Source color factor (alpha premultiplied)
		blend.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;          // Destination color factor (alpha premultiplied)
		blend.colorBlendOp = VK_BLEND_OP_ADD;                     // Blend operation for color components
		blend.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;          // Source alpha factor
		blend.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;          // Destination alpha factor
		blend.alphaBlendOp = VK_BLEND_OP_ADD;                     // Blend operation for alpha component

		builder.setVertexAttribtes({});
		builder.setVertexBindings({});

		_sunPipeline = builder.build(_context);
	}

	VkDescriptorImageInfo ForwardShader::getAttachmentInfo() const{
		VkDescriptorImageInfo info;
		info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		info.imageView = _imageView;
		info.sampler = _sampler;
		return info;
	}

	void ForwardShader::beginRenderPass(VkCommandBuffer commandBuffer){
		VkRenderPassBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.framebuffer = _framebuffer;
		info.renderPass = _renderPass;
		info.renderArea = VkRect2D{0, 0, _width, _height};

		info.clearValueCount = 1;
		info.pClearValues = &attachment.clear;

		VkImageMemoryBarrier layoutTransitionBarrier = {};
		layoutTransitionBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		layoutTransitionBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		layoutTransitionBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		layoutTransitionBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		layoutTransitionBarrier.newLayout = attachment.description.initialLayout;
		layoutTransitionBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		layoutTransitionBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		layoutTransitionBarrier.image = _image;
		layoutTransitionBarrier.subresourceRange.baseMipLevel = 0;
		layoutTransitionBarrier.subresourceRange.levelCount = 1;
		layoutTransitionBarrier.subresourceRange.baseArrayLayer = 0;
		layoutTransitionBarrier.subresourceRange.layerCount = 1;
		layoutTransitionBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			0, 0, nullptr, 0, nullptr, 1, &layoutTransitionBarrier);

		vkCmdBeginRenderPass(commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	void ForwardShader::endRenderPass(VkCommandBuffer commandBuffer){
		vkCmdEndRenderPass(commandBuffer);
	}

	void ForwardShader::render(VkCommandBuffer commandBuffer, const glm::vec3& cameraDirection, const glm::vec3& cameraPosition){
		beginRenderPass(commandBuffer);

		renderLightPoints(commandBuffer, cameraDirection, cameraPosition);
		renderSpotlights(commandBuffer, cameraDirection, cameraPosition);
		renderSuns(commandBuffer, cameraDirection, cameraPosition);

		endRenderPass(commandBuffer);
	}
	
	void ForwardShader::renderLightPoints(VkCommandBuffer commandBuffer, const glm::vec3& cameraDirection, const glm::vec3& cameraPosition){
		auto lights = _context.scene.componentEntities<Core::Scene::Components::LightPoint>();
		if (lights.empty()) return;

		_lightPointPipeline->bind(commandBuffer);

		vkCmdBindDescriptorSets(
			commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			_lightPointPipeline->layout(),
			0,
			1,
			&_descriptorSet,
			0,
			nullptr
		);

		LightPointPushConstant pushConstant;

		pushConstant.cameraDirection = cameraDirection;
		pushConstant.cameraPosition = cameraPosition;


		for (const auto& l : lights){
			auto& transform = _context.scene.getComponent<Core::Scene::Components::Transform>(l);
			auto& light = _context.scene.getComponent<Core::Scene::Components::LightPoint>(l);

			pushConstant.position = transform.translation;
			pushConstant.color = light.color;
			pushConstant.intensity = light.intensity;

			vkCmdPushConstants(commandBuffer, _lightPointPipeline->layout(), VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(LightPointPushConstant), &pushConstant);
			vkCmdDraw(commandBuffer, 6, 1, 0, 0);
		}
	}

	void ForwardShader::renderSpotlights(VkCommandBuffer commandBuffer, const glm::vec3& cameraDirection, const glm::vec3& cameraPosition){
		auto lights = _context.scene.componentEntities<Core::Scene::Components::Spotlight>();
		if (lights.empty()) return;

		_spotlightPipeline->bind(commandBuffer);

		vkCmdBindDescriptorSets(
			commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			_spotlightPipeline->layout(),
			0,
			1,
			&_descriptorSet,
			0,
			nullptr
		);

		SpotlightPushConstant pushConstant;

		pushConstant.cameraDirection = cameraDirection;
		pushConstant.cameraPosition = cameraPosition;

		for (const auto& l : lights){
			auto& transform = _context.scene.getComponent<Core::Scene::Components::Transform>(l);
			auto& light = _context.scene.getComponent<Core::Scene::Components::Spotlight>(l);

			pushConstant.position = transform.translation;
			pushConstant.color = light.color;
			pushConstant.intensity = light.intensity;
			pushConstant.maxAngle = light.maxAngle;

			pushConstant.direction = glm::rotate(transform.rotation, glm::vec3(0.0f, 0.0f, -1.0f));
			pushConstant.direction = glm::normalize(pushConstant.direction);

			vkCmdPushConstants(commandBuffer, _spotlightPipeline->layout(), VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SpotlightPushConstant), &pushConstant);
			vkCmdDraw(commandBuffer, 6, 1, 0, 0);
		}
	}

	void ForwardShader::renderSuns(VkCommandBuffer commandBuffer, const glm::vec3& cameraDirection, const glm::vec3& cameraPosition){
		auto lights = _context.scene.componentEntities<Core::Scene::Components::Sun>();
		if (lights.empty()) return;

		_sunPipeline->bind(commandBuffer);

		vkCmdBindDescriptorSets(
			commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			_sunPipeline->layout(),
			0,
			1,
			&_descriptorSet,
			0,
			nullptr
		);

		SunPushConstant pushConstant;

		pushConstant.cameraDirection = cameraDirection;
		pushConstant.cameraPosition = cameraPosition;

		for (const auto& l : lights){
			auto& transform = _context.scene.getComponent<Core::Scene::Components::Transform>(l);
			auto& light = _context.scene.getComponent<Core::Scene::Components::Sun>(l);

			pushConstant.color = light.color;
			pushConstant.direction = glm::rotate(transform.rotation, glm::vec3(0.0f, 0.0f, -1.0f));
			pushConstant.direction = glm::normalize(pushConstant.direction);

			vkCmdPushConstants(commandBuffer, _sunPipeline->layout(), VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SunPushConstant), &pushConstant);
			vkCmdDraw(commandBuffer, 6, 1, 0, 0);
		}
	}

	void ForwardShader::checkDefaultFormats(){
		auto &a = attachment;
		VkFormat bestCase = VK_FORMAT_UNDEFINED;

		if (bestCase == VK_FORMAT_UNDEFINED){
			for (auto &f : a.formats){
				VkFormatProperties properties;
				vkGetPhysicalDeviceFormatProperties(_context.device.getPhysicalDevice(), f, &properties);

				if (properties.optimalTilingFeatures & a.requiredFeatures){
					a.image.tiling = VK_IMAGE_TILING_OPTIMAL;
					bestCase = f;
					break;
				} else if (properties.linearTilingFeatures & a.requiredFeatures){
					a.image.tiling = VK_IMAGE_TILING_LINEAR;
					bestCase = f;
					break;
				}
			}
		}
		
		for (auto &f : a.formats){
			VkFormatProperties properties;
			vkGetPhysicalDeviceFormatProperties(_context.device.getPhysicalDevice(), f, &properties);

			if (a.image.tiling == VK_IMAGE_TILING_OPTIMAL){
				if (properties.optimalTilingFeatures & a.requiredFeatures){
					bestCase = f;
					break;
				}
			} else if (a.image.tiling == VK_IMAGE_TILING_LINEAR){
				if (properties.linearTilingFeatures & a.requiredFeatures){
					bestCase = f;
					break;
				}
			}
		}


		if (bestCase == VK_FORMAT_UNDEFINED){
			CLOG(ERROR, "Engine.Graphics.WorldFramebuffer") << "failed to find a format for world framebuffer attachment";
			throw std::runtime_error("failed to find a format for world framebuffer attachment");
		}

		a.imageView.format = bestCase;
		a.description.format = bestCase;
		a.image.format = bestCase;
	}
}