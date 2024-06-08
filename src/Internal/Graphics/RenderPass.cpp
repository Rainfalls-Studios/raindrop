#include <Raindrop_internal/Graphics/RenderPass.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Internal::Graphics{
	RenderPass::RenderPass(Context& context, const RenderPassConfigInfo& info) : 
		_context{context},
		_renderPass{VK_NULL_HANDLE}
	{
		auto& device = _context.getDevice();
		const auto& allocationCallbacks = _context.getAllocationCallbacks();

		_context.getLogger()->info("Creating new render pass...");

		Exceptions::checkVkCreation<VkRenderPass>(
			vkCreateRenderPass(device.get(), &info, allocationCallbacks, &_renderPass),
			"Failed to create render pass"
		);


		_context.getLogger()->info("Render pass created successfully !");
	}

	RenderPass::~RenderPass(){
		_context.getLogger()->info("Destroying render pass ...");
		auto& device = _context.getDevice();
		const auto& allocationCallbacks = _context.getAllocationCallbacks();

		if (_renderPass != VK_NULL_HANDLE){
			vkDestroyRenderPass(device.get(), _renderPass, allocationCallbacks);
			_renderPass = VK_NULL_HANDLE;
		}
		_context.getLogger()->info("Render pass destroyed successfully !");
	}

	VkRenderPass RenderPass::get() const{
		return _renderPass;
	}
}