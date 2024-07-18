#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <spdlog/spdlog.h>
#include <iostream>

class Renderer{
	public:
		Renderer() : 
			_context{nullptr}
		{}

		~Renderer(){
			_context->getDevice().waitIdle();
			mainCommandPool.release();
		}

		void initialize(Raindrop::Graphics::Context& context){
			_context = &context;

			initCommandPool();
			allocateCommandBuffers();
		}

		Raindrop::Graphics::CommandBuffer* beginFrame(){
			auto& swapchain = _context->window.swapchain;

			VkResult result = swapchain.acquireNextImage();
			
			if (result == VK_ERROR_OUT_OF_DATE_KHR){
				rebuildSwapchain();
				return nullptr;
			}

			if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
				_context->logger->error("Failed to aquire the swapchain image");
				throw std::runtime_error("failed to acquire the swapchain image");
			}

			std::uint32_t currentFrameIndex = swapchain.getCurrentFrameIndex();
			Raindrop::Graphics::CommandBuffer& commandBuffer = _commandBuffers[currentFrameIndex];

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

		void endFrame(Raindrop::Graphics::CommandBuffer* commandBuffer){
			VkResult result = vkEndCommandBuffer(commandBuffer->get());

			auto& swapchain = _context->window.swapchain;

			if (result != VK_SUCCESS){
				_context->logger->info("Failed to end command buffer");
				throw std::runtime_error("Failed to end command buffer");
			}
			
			result = swapchain.submitCommandBuffer({commandBuffer->get()});

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR){
				rebuildSwapchain();
			} else if (result != VK_SUCCESS) {
				_context->logger->info("Failed to submit command buffer");
				throw std::runtime_error("Failed to submit command buffer");
			}
		}

		void beginSwapchainRenderPass(Raindrop::Graphics::CommandBuffer* commandBuffer){
			_context->window.swapchain.beginRenderPass(commandBuffer->get());
		}

		void endSwapchainRenderPass(Raindrop::Graphics::CommandBuffer* commandBuffer){
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
		Raindrop::Graphics::Context* _context;
		Raindrop::Graphics::CommandPool mainCommandPool;
		std::vector<Raindrop::Graphics::CommandBuffer> _commandBuffers;

		void rebuildSwapchain(){
			auto& swapchain = _context->window.swapchain;
			auto& window = _context->window.window;

			swapchain.wantExtent(window.getExtent());
			swapchain.rebuildSwapchain();
		}

		void initCommandPool(){
			mainCommandPool.prepare(*_context);
			mainCommandPool.setCreateFlags(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
			mainCommandPool.setQueueFamily(_context->getDevice().graphicsQueue.familyIndex);

			mainCommandPool.initialize();
		}

		void allocateCommandBuffers(){
			std::uint32_t count = _context->window.swapchain.getFrameCount();
			_commandBuffers = mainCommandPool.allocate(count, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		}
		
};

class Testbed : public Raindrop::Engine, public Raindrop::Events::Listener{
	public:
		bool closeEvent(const Raindrop::Events::WindowCloseRequest& event){
			_run = false;
			return true;
		}

		Testbed() : Engine(), Listener(){
			Engine::initialize(INIT_EVERYTHING | INIT_EVENTS);
			_renderer.initialize(getGraphicsContext());

			auto& eventManager = Engine::getEventsContext().manager;
			eventManager.subscribe<Raindrop::Events::WindowCloseRequest>(this, &Testbed::closeEvent);
		}

		void run(){
			_run = true;
			auto start = std::chrono::steady_clock::now();

			while (_run){
				auto now = std::chrono::steady_clock::now();

				std::chrono::duration<float> duration = now - start;
				getGraphicsContext().window.window.events();


				auto commandBuffer = _renderer.beginFrame();
				if (commandBuffer){

					glm::vec3 clearColor;
					clearColor.r = glm::sin(duration.count());
					clearColor.g = glm::cos(duration.count() * 3.14);
					clearColor.b = glm::sin(duration.count() * 1.5);

					_renderer.setSwapchainColor(clearColor);
					
					_renderer.beginSwapchainRenderPass(commandBuffer);
					_renderer.endSwapchainRenderPass(commandBuffer);

					_renderer.endFrame(commandBuffer);
				}
			}
		
		}

	private:
		Renderer _renderer;
		bool _run;
};

int main(){
	Testbed testbed;

	testbed.run();

	return EXIT_SUCCESS;
}