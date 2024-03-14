#include <Raindrop/Renderer/Scene/Scene.hpp>
#include <Raindrop/Renderer/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Renderer::Scene{
	Scene::Scene(::Raindrop::Renderer::Context& renderer) : 
		_renderer{renderer},
		_context{renderer}
	{
		for (std::size_t i=0; i<4; i++){
			clearColor.color.float32[i] = 0.f;
		}
	}
		
	Scene::~Scene(){}

	void Scene::resize(const glm::uvec2& size){
		spdlog::info("resizing scene framebuffer ...");
		_context.framebuffer.resize(size);
	}

	void Scene::swapchainRender(VkCommandBuffer commandBuffer){
		_context.fullscreenQuad.render(commandBuffer);
	}

	void Scene::beginRenderPass(VkCommandBuffer commandBuffer){
		
		{
			VkImageMemoryBarrier barriers[2] = {};

			// DEPTH
			barriers[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barriers[0].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED; // Initial layout before the barrier
			barriers[0].newLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL; // Layout after the barrier (assuming color attachment)
			barriers[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barriers[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barriers[0].image = _context.framebuffer.depth().image;

			barriers[0].subresourceRange = VkImageSubresourceRange{
				.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			};

			barriers[0].srcAccessMask = 0; // No access in the previous stage
			barriers[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			// COLOR
			barriers[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barriers[1].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED; // Initial layout before the barrier
			barriers[1].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Layout after the barrier (assuming color attachment)
			barriers[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barriers[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barriers[1].image = _context.framebuffer.color().image;

			barriers[1].subresourceRange = VkImageSubresourceRange{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			};

			barriers[1].srcAccessMask = 0; // No access in the previous stage
			barriers[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;


			vkCmdPipelineBarrier(commandBuffer,
				VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
				0,
				0, nullptr, 
				0, nullptr,
				2, barriers
			);
		}

		VkRenderPassBeginInfo info{};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.framebuffer = _context.framebuffer.framebuffer();
		
		VkClearValue clearDepth{};
		clearDepth.depthStencil.depth = 1.f;

		VkClearValue clearValues[] = {
			clearDepth,
			clearColor
		};

		info.pClearValues = clearValues;
		info.clearValueCount = 2;

		info.renderArea.extent.width = _context.framebuffer.size().x;
		info.renderArea.extent.height = _context.framebuffer.size().y;
		info.renderArea.offset = {0, 0};

		info.renderPass = _context.renderPass.get();
		
		vkCmdBeginRenderPass(commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	void Scene::endRenderPass(VkCommandBuffer commandBuffer){
		vkCmdEndRenderPass(commandBuffer);
	}
}