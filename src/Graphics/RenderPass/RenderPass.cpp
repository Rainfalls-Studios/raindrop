#include <Raindrop/Graphics/RenderPass/RenderPass.hpp>
#include <Raindrop/Graphics/RenderPass/RenderPassBuilder.hpp>
#include <Raindrop/Graphics/RenderPass/Context.hpp>

namespace Raindrop::Graphics::RenderPass{
	RenderPass::RenderPass(Context& context, const RenderPassBuilder& builder) : _context{context}{
		_context.logger().info("Builing a render pass \"{}\"", builder.name());

		auto& device = _context.graphics().internal().device();
		auto& allocationCallbacks = _context.graphics().allocationCallbacks();

		_renderPass = VK_NULL_HANDLE;

		VkRenderPassCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.flags = builder.flags();

		info.attachmentCount = builder.attachmentCount();
		info.pAttachments = builder.attachments();

		info.dependencyCount = builder.dependencyCount();
		info.pDependencies = builder.dependencies();

		info.subpassCount = builder.subpassCount();
		info.pSubpasses = builder.subpasses();

		if (vkCreateRenderPass(device.get(), &info, allocationCallbacks, &_renderPass) != VK_SUCCESS){
			_context.logger().error("Failed to create a render pass");
			throw std::runtime_error("Failed to create a render pass");
		}
	}

	RenderPass::~RenderPass(){
		auto& device = _context.graphics().internal().device();
		auto& allocationCallbacks = _context.graphics().allocationCallbacks();

		if (_renderPass != VK_NULL_HANDLE){
			vkDestroyRenderPass(device.get(), _renderPass, allocationCallbacks);
			_renderPass = VK_NULL_HANDLE;
		}
	}

	VkRenderPass RenderPass::get() const{
		return _renderPass;
	}
}