#include <Raindrop/CommandBuffer.hpp>
#include <Raindrop_internal/CommandBuffer.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop_internal/Graphics/Renderer.hpp>
#include <Raindrop/GUID.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop/Context.hpp>

#define LOGGER _impl->context->getInternalContext()->getLogger()
#define INFO _impl->info
#define GRAPHICS_CONTEXT _impl->context->getInternalContext()->getRenderer().getContext()

namespace Raindrop{
	VkCommandBufferUsageFlags toVulkan(const CommandBuffer::Flags& flags){
		VkCommandBufferUsageFlags out = 0;

		if (flags.has(CommandBuffer::Flags::SINGLE_SUBMIT)) out |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		if (flags.has(CommandBuffer::Flags::RENDER_PASS_CONTINUE)) out |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		if (flags.has(CommandBuffer::Flags::SIMULTANEOUS_USE)) out |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		return out;
	}

	VkQueueFlags toVulkan(const CommandBuffer::Usage& usage){
		VkQueueFlags out = 0;

		if (usage.has(CommandBuffer::Usage::GRAPHICS)) out |= VK_QUEUE_GRAPHICS_BIT;
		if (usage.has(CommandBuffer::Usage::COMPUTE)) out |= VK_QUEUE_COMPUTE_BIT;
		if (usage.has(CommandBuffer::Usage::TRANSFER)) out |= VK_QUEUE_TRANSFER_BIT;
		if (usage.has(CommandBuffer::Usage::SPARSE_BINDING)) out |= VK_QUEUE_SPARSE_BINDING_BIT;
		if (usage.has(CommandBuffer::Usage::PROTECTED)) out |= VK_QUEUE_PROTECTED_BIT;

		// VK_KHR_video_decode_queue
		if (usage.has(CommandBuffer::Usage::VIDEO_DECODE)) out |= VK_QUEUE_VIDEO_DECODE_BIT_KHR;

		// VK_KHR_video_encode_queue
		if (usage.has(CommandBuffer::Usage::VIDEO_ENCODE)) out |= VK_QUEUE_VIDEO_ENCODE_BIT_KHR;

		// VK_NV_optical_flow
		if (usage.has(CommandBuffer::Usage::OPTICAL_FLOW)) out |= VK_QUEUE_OPTICAL_FLOW_BIT_NV;

		return out;
	}

	VkCommandPoolCreateFlags toVulkan(const CommandBuffer::Pool::Flags& flags){
		VkCommandPoolCreateFlags out = 0;

		if (flags.has(CommandBuffer::Pool::Flags::SHORT_LIVED)) out |= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
		if (flags.has(CommandBuffer::Pool::Flags::INDEPENDENT_RESET)) out |= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		if (flags.has(CommandBuffer::Pool::Flags::PROTECTED)) out |= VK_COMMAND_POOL_CREATE_PROTECTED_BIT;

		return out;
	}

	//--------------------------------------------------------------------
	//-----------------          COMMAND POOL            -----------------
	//--------------------------------------------------------------------

	CommandBuffer::Pool::Pool(Context& context, const Usage& usage, const Flags& flags){
		_impl = new Impl(context);

		Internal::Graphics::CommandPoolConfigInfo info;
		info.flags = toVulkan(flags);
		
		// info.familyIndex = 
		auto families = GRAPHICS_CONTEXT.getQueues().findSuitable(toVulkan(usage));
		
		if (families.empty()){
			context.getInternalContext()->getLogger()->error("Could not find a family queue that supports requirement (TODO: print required capabilities)");
			throw std::runtime_error("Could not find a family that fulfills requirements");
		}

		info.familyIndex = families.front().first.getIndex();

		_impl->commandPool = std::make_shared<Internal::Graphics::CommandPool>(GRAPHICS_CONTEXT, info);
	}

	CommandBuffer::Pool::~Pool(){
		delete _impl;
		_impl = nullptr;
	}

	CommandBuffer CommandBuffer::Pool::allocate(const CommandBuffer::AllocationFlags& flags){
		VkCommandBufferLevel level = flags.has(CommandBuffer::AllocationFlags::SECONDARY) ? VK_COMMAND_BUFFER_LEVEL_SECONDARY : VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		auto commandBuffer = _impl->commandPool->allocate(1, level);

		CommandBuffer::Impl* impl = new CommandBuffer::Impl(*_impl->context);
		impl->commandBuffer = commandBuffer[0];

		// The pointer ownership is transfered to the command buffer
		return CommandBuffer(impl);
	}

	std::vector<CommandBuffer> CommandBuffer::Pool::allocate(const uint32_t& count, const CommandBuffer::AllocationFlags& flags){
		VkCommandBufferLevel level = flags.has(CommandBuffer::AllocationFlags::SECONDARY) ? VK_COMMAND_BUFFER_LEVEL_SECONDARY : VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		auto commandBuffers = _impl->commandPool->allocate(count, level);
		std::vector<CommandBuffer> out;

		for (const auto& commandBuffer : commandBuffers){
			CommandBuffer::Impl* impl = new CommandBuffer::Impl(*_impl->context);
			impl->commandBuffer = commandBuffer;

			// The pointer ownership is transfered to the command buffer
			out.emplace_back(impl);
		}

		return out;
	}

	void CommandBuffer::Pool::free(const CommandBuffer& commandBuffer){
		_impl->commandPool->free({commandBuffer._impl->commandBuffer});
	}

	void CommandBuffer::Pool::free(const std::vector<CommandBuffer>& commandBuffers){
		std::vector<VkCommandBuffer> vkCmdBuffers(commandBuffers.size());

		for (std::size_t i=0; i<commandBuffers.size(); i++){
			vkCmdBuffers[i] = commandBuffers[i]._impl->commandBuffer;
		}

		_impl->commandPool->free(vkCmdBuffers);
	}

	void CommandBuffer::Pool::reset(const bool& release){
		VkCommandPoolResetFlags flags = release ? VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT : 0;
		_impl->commandPool->reset(flags);
	}

	//--------------------------------------------------------------------
	//-----------------        COMMAND BUFFER            -----------------
	//--------------------------------------------------------------------


	bool CommandBuffer::isInitialized() const noexcept{
		return _impl != nullptr;
	}

	void* CommandBuffer::getNativeHandle() const{
		return static_cast<void*>(_impl->commandBuffer);
	}

	CommandBuffer::Impl* CommandBuffer::getImpl() const noexcept{
		return _impl;
	}

	GUID CommandBuffer::getGUID() const noexcept{

	}

	CommandBuffer::CommandBuffer(Impl* impl) : _impl{impl}{}

	// CommandBuffer::CommandBuffer(const CommandBuffer& other) : _impl{nullptr}{
	// 	_impl = new Impl(*other._impl);
	// }

	CommandBuffer::CommandBuffer(CommandBuffer&& other) noexcept : _impl(other._impl) {
		other._impl = nullptr;
	}

	CommandBuffer& CommandBuffer::operator=(CommandBuffer&& other) noexcept {
		if (this != &other) {
			delete _impl;
			_impl = other._impl;
			other._impl = nullptr;
		}
		return *this;
	}

	CommandBuffer::~CommandBuffer(){
		delete _impl;
		_impl = nullptr;
	}

	void CommandBuffer::begin(const Flags& flags){
		VkCommandBufferBeginInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags = toVulkan(flags);

		Exceptions::checkVkOperation<VkCommandBuffer>(
			vkBeginCommandBuffer(_impl->commandBuffer, &info),
			"Failed to begin command buffer",
			Exceptions::VulkanOperationType::BEGIN,
			LOGGER
		);
	}

	void CommandBuffer::end(){
		vkEndCommandBuffer(_impl->commandBuffer);

		Exceptions::checkVkOperation<VkCommandBuffer>(
			vkEndCommandBuffer(_impl->commandBuffer),
			"Failed to end command buffer",
			Exceptions::VulkanOperationType::END,
			LOGGER
		);
	}
}