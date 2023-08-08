#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Graphics/Instance.hpp>
#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/Window.hpp>
#include <Raindrop/Graphics/Texture.hpp>

namespace Raindrop::Graphics{
	GraphicsContext::GraphicsContext(Core::EngineContext& context, Core::Scene::Scene& scene) :
		scene{scene},
		context{context},
		gRegistry{},
		window{*this},
		instance{*this},
		device{*this},
		swapchain{*this},
		graphicsCommandPool{*this},
		transfertCommandPool{*this},
		layouts{*this},
		renderPasses{*this}{

		createPool();
		createWhiteTexture();
	}

	const Texture& GraphicsContext::whiteTexture() const{
		return *_whiteTexture;
	}

	void GraphicsContext::createPool(){
		VkDescriptorPoolSize pool_sizes[] ={
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000;
		pool_info.poolSizeCount = std::size(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;

		if (vkCreateDescriptorPool(device.get(), &pool_info, nullptr, &pool) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics") << "Failed to create main descriptor pool";
			throw std::runtime_error("failed to create main descriptor pool");
		}
	}

	void GraphicsContext::destroyPool(){
		vkResetDescriptorPool(device.get(), pool, 0);
		vkDestroyDescriptorPool(device.get(), pool, allocationCallbacks);
	}

	void GraphicsContext::createWhiteTexture(){
		uint32_t pixel = 0xFFFFFFFF;

		_whiteTexture = std::make_unique<Texture>(
			*this,
			static_cast<void*>(&pixel),
			1,
			1
		);
	}

	GraphicsContext::~GraphicsContext(){
		destroyPool();	
	}

	DescriptorLayouts::DescriptorLayouts(GraphicsContext& context) : _context{context}{
		createLightsLayouts();
	}

	DescriptorLayouts::~DescriptorLayouts(){
		destroyLightsLayouts();
	}
	
	void DescriptorLayouts::createLightsLayouts(){
		VkDescriptorSetLayoutBinding binding = {};
		binding.binding = 0;
		binding.descriptorCount = 1;
		binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		binding.pImmutableSamplers = nullptr;
		binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		VkDescriptorSetLayoutCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		info.bindingCount = 1;
		info.pBindings = &binding;
		
		auto device = _context.device.get();
		auto allocationCallbacks = _context.allocationCallbacks;
		
		if (vkCreateDescriptorSetLayout(device, &info, allocationCallbacks, &sunShadowMapLayout) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics") << "Failed to create lights descriptor set layouts";
			throw std::runtime_error("failed to create light descriptor set layouts");
		}

		lightPointShadowMapLayout = sunShadowMapLayout;
		spotlightShadowMapLayout = sunShadowMapLayout;
	}

	void DescriptorLayouts::destroyLightsLayouts(){
		auto device = _context.device.get();
		auto allocationCallbacks = _context.allocationCallbacks;

		if (sunShadowMapLayout) vkDestroyDescriptorSetLayout(device, sunShadowMapLayout, allocationCallbacks);

		// since they are all the same
		// if (lightPointShadowMapLayout) vkDestroyDescriptorSetLayout(device, lightPointShadowMapLayout, allocationCallbacks);
		// if (spotlightShadowMapLayout) vkDestroyDescriptorSetLayout(device, spotlightShadowMapLayout, allocationCallbacks);
	}

	RenderPasses::RenderPasses(GraphicsContext& context) : 
		sun{context},
		forwardShader{context}
	{}

	RenderPasses::~RenderPasses(){}
}