#ifndef __RAINDROP_GRAPHICS_WINDOW_SWAPCHAIN_HPP__
#define __RAINDROP_GRAPHICS_WINDOW_SWAPCHAIN_HPP__

#include "common.hpp"
#include "SwapchainSupport.hpp"
#include "../Queues.hpp"

namespace Raindrop::Internal::Graphics::Core{
	class Swapchain{
		public:
			struct Frame{
				VkFramebuffer framebuffer = VK_NULL_HANDLE;
				VkSemaphore imageAvailable = VK_NULL_HANDLE;
				VkSemaphore imageFinished = VK_NULL_HANDLE;
				VkFence inFlightFence = VK_NULL_HANDLE;
				VkFence imageInFlight = VK_NULL_HANDLE;
				VkImage image = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
			};

			Swapchain(Context& context);
			~Swapchain();

			void rebuildSwapchain();

			void setExtent(VkExtent2D extent);
			void setFrameCount(uint32_t count);
			void setPresentMode(VkPresentModeKHR presentMode);
			void setSurfaceFormat(VkSurfaceFormatKHR surfaceFormat);

			VkResult acquireNextImage();
			VkResult submitCommandBuffer(VkCommandBuffer* buffers);

			uint32_t frameCount() const;
			uint32_t currentFrame() const;

			const Raindrop::RenderPass& renderPass() const;

			void beginRenderPass(VkCommandBuffer commandBuffer);
			void endRenderPass(VkCommandBuffer commandBuffer);

			void setClearColor(VkClearColorValue color);

			Frame& getFrameData(uint32_t id);
			Frame& getCurrentFrameData();
			std::vector<Frame>& getFramesData();

		private:
			struct SwapchainData{
				Context* _context;
				VkSwapchainKHR _swapchain = VK_NULL_HANDLE;
				std::vector<Frame> _frames;

				~SwapchainData();
				SwapchainData(Context& _context);
			};

			Context* _context;

			Core::SwapchainSupport _swapchainSupport;
			std::unique_ptr<SwapchainData> _swapchain;
			std::unique_ptr<SwapchainData> _oldSwapchain;

			VkRenderPass _renderPass;
			uint32_t _currentFrame;
			VkFormat _imageFormat;

			uint32_t _frameCount;
			VkExtent2D _extent;
			VkPresentModeKHR _presentMode;
			VkSurfaceFormatKHR _surfaceFormat;

			uint32_t _wantedFrameCount;
			VkExtent2D _wantedExtent;
			VkPresentModeKHR _wantedPresentMode;
			VkSurfaceFormatKHR _wantedSurfaceFormat;

			VkClearColorValue _clearColor;

			void findSurfaceFormat();
			void findPresentMode();
			void findExtent();
			void findFrameCount();

			void createRenderPass();
			void createImageViews();
			void getSwapchainImages();
			void createFramebuffers();
			void createSyncObjects();
	};
}

#endif