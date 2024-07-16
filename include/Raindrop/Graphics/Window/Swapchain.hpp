#ifndef __RAINDROP_GRAPHICS_WINDOW_SWAPCHAIN_HPP__
#define __RAINDROP_GRAPHICS_WINDOW_SWAPCHAIN_HPP__

#include "prototypes.hpp"
#include <Raindrop/Graphics/Core/prototypes.hpp>
#include <Raindrop/Graphics/RenderPass.hpp>
#include <Raindrop/Graphics/pch.pch>

namespace Raindrop::Graphics::Window{
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

			enum FrameCount{
				SINGLE_BUFFERING = 1,
				DOUBLE_BUFFERING = 2,
				TRIPLE_BUFFERING = 3
			};

			Swapchain() noexcept;
			~Swapchain();

			void prepare(Context& context, Core::Context& core, Graphics::Context& graphics);
			void initialize();
			void release();

			void rebuildSwapchain();

			Swapchain& wantExtent(VkExtent2D extent);
			Swapchain& wantFrameCount(uint32_t count);
			Swapchain& wantPresentMode(VkPresentModeKHR presentMode);
			Swapchain& wantSurfaceFormat(VkSurfaceFormatKHR surfaceFormat);

			VkResult acquireNextImage();
			VkResult submitCommandBuffer(std::vector<VkCommandBuffer> buffers);

			uint32_t getFrameCount() const;
			uint32_t getCurrentFrameIndex() const;

			const RenderPass& getRenderPass() const;

			void beginRenderPass(VkCommandBuffer commandBuffer);
			void endRenderPass(VkCommandBuffer commandBuffer);

			void setClearColor(VkClearColorValue color);

			Frame& getFrameData(uint32_t id);
			Frame& getCurrentFrameData();
			std::vector<Frame>& getFramesData();

		private:
			struct SwapchainData{
				Core::Context* context;
				VkSwapchainKHR swapchain;
				std::vector<Frame> frames;

				SwapchainData(Core::Context* context);
				~SwapchainData();
			};

			Context* _context;
			Core::Context* _core;
			Graphics::Context* _graphics;

			std::unique_ptr<SwapchainData> _swapchain;
			std::unique_ptr<SwapchainData> _oldSwapchain;

			RenderPass _renderPass;
			uint32_t _currentFrame;

			uint32_t _frameCount;
			VkExtent2D _extent;
			VkPresentModeKHR _presentMode;
			VkSurfaceFormatKHR _surfaceFormat;

			// Not released because the swapchain get continiously rebuilt
			struct{
				std::uint32_t frameCount;
				VkExtent2D extent;
				VkPresentModeKHR presentMode;
				VkSurfaceFormatKHR surfaceFormat;
			} _buildInfo;

			struct{
				VkSurfaceCapabilitiesKHR capabilities;
				std::vector<VkSurfaceFormatKHR> formats;
				std::vector<VkPresentModeKHR> presentModes;
			} _support;


			VkClearColorValue _clearColor;

			void querySupport();
			void querySupportCapabilities();
			void querySupportFormats();
			void querySupportPresentModes();
			
			VkSurfaceFormatKHR findSurfaceFormat();
			VkPresentModeKHR findPresentMode();
			VkExtent2D findExtent();
			std::uint32_t findFrameCount();

			void createRenderPass();
			void createImageViews();
			void getSwapchainImages();
			void createFramebuffers();
			void createSyncObjects();
	};
}

#endif