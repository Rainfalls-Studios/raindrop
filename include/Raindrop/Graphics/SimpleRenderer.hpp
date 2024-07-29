#ifndef __RAINDROP_GRAPHICS_SIMPLE_RENDERE_HPP__
#define __RAINDROP_GRAPHICS_SIMPLE_RENDERE_HPP__

#include "common.hpp"
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics{
	class SimpleRenderer{
		public:
			SimpleRenderer() : 
				_context{nullptr}
			{}

			~SimpleRenderer(){
				_context->getDevice().waitIdle();
				mainCommandPool.release();
			}

			void initialize(Context& context){
				_context = &context;

				initCommandPool();
				allocateCommandBuffers();
			}

			CommandBuffer* beginFrame(){
				auto& swapchain = _context->window.swapchain;
				auto& window = _context->window.window;

				VkResult result = swapchain.acquireNextImage();
				
				if (window.wasResized() || result == VK_ERROR_OUT_OF_DATE_KHR){
					rebuildSwapchain();
					return nullptr;
				}

				if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
					_context->logger->error("Failed to aquire the swapchain image");
					throw std::runtime_error("failed to acquire the swapchain image");
				}

				std::uint32_t currentFrameIndex = swapchain.getCurrentFrameIndex();
				CommandBuffer& commandBuffer = _commandBuffers[currentFrameIndex];

				VkCommandBufferBeginInfo info{};
				info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
				
				Raindrop::Exceptions::checkVkOperation<VkCommandBuffer>(
					vkBeginCommandBuffer(commandBuffer.get(), &info),
					"Failed to aquire the swapchain image",
					Raindrop::Exceptions::VulkanOperationType::BEGIN,
					_context->logger
				);

				return &commandBuffer;
			}

			void endFrame(CommandBuffer* commandBuffer){
				VkResult result = vkEndCommandBuffer(commandBuffer->get());

				auto& swapchain = _context->window.swapchain;
				auto& window = _context->window.window;

				if (result != VK_SUCCESS){
					_context->logger->info("Failed to end command buffer");
					throw std::runtime_error("Failed to end command buffer");
				}
				
				result = swapchain.submitCommandBuffer({commandBuffer->get()});

				if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasResized()){
					rebuildSwapchain();
				} else if (result != VK_SUCCESS) {
					_context->logger->info("Failed to submit command buffer");
					throw std::runtime_error("Failed to submit command buffer");
				}
			}

			void beginSwapchainRenderPass(CommandBuffer* commandBuffer){
				_context->window.swapchain.beginRenderPass(commandBuffer->get());
			}

			void endSwapchainRenderPass(CommandBuffer* commandBuffer){
				_context->window.swapchain.endRenderPass(commandBuffer->get());
			}

			void setSwapchainColor(const glm::vec3& color){
				VkClearColorValue clear;
				clear.float32[0] = color.r;
				clear.float32[1] = color.g;
				clear.float32[2] = color.b;
				clear.float32[3] = 0.f;
				_context->window.swapchain.setClearColor(clear);
			}

		private:
			Context* _context;
			CommandPool mainCommandPool;
			std::vector<CommandBuffer> _commandBuffers;

			void rebuildSwapchain(){
				auto& swapchain = _context->window.swapchain;
				auto& window = _context->window.window;

				swapchain.wantExtent(window.getExtent());
				swapchain.rebuildSwapchain();
			}

			void initCommandPool(){
				mainCommandPool.prepare(*_context);
				mainCommandPool.setCreateFlags(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
				mainCommandPool.setQueueFamily(_context->getDevice().graphicsQueue.familyIndex);

				mainCommandPool.initialize();
			}

			void allocateCommandBuffers(){
				std::uint32_t count = _context->window.swapchain.getFrameCount();
				_commandBuffers = mainCommandPool.allocate(count, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
			}
	};
}

#endif