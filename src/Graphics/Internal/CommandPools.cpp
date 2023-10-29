#include <Raindrop/Graphics/Internal/CommandPools.hpp>
#include <Raindrop/Graphics/Internal/Context.hpp>
#include <Raindrop/Graphics/Internal/CommandPool.hpp>
#include <Raindrop/Graphics/Internal/QueueFamily.hpp>

namespace Raindrop::Graphics::Internal{


	CommandPools::CommandPools(Context& context) : _context{context}, _commandPools(10, PoolInfo::hash){}
	CommandPools::~CommandPools(){}

	CommandPool& CommandPools::pool(const QueueProperties& properties, VkCommandPoolCreateFlags flags){
		PoolInfo info;
		info.familyIndex = _context.queueHandler().getByProperies(properties).front().get().index();
		info.flags = flags;

		auto it = _commandPools.find(info);
		if (it != _commandPools.end()) return *it->second;
		return create(info);
	}

	const CommandPool& CommandPools::pool(const QueueProperties& properties, VkCommandPoolCreateFlags flags) const{
		PoolInfo info;
		info.familyIndex = _context.queueHandler().getByProperies(properties).front().get().index();
		info.flags = flags;

		auto it = _commandPools.find(info);
		if (it != _commandPools.end()) return *it->second;
		throw std::runtime_error("Failed to find a suitable command buffer");
	}

	std::size_t CommandPools::PoolInfo::hash(const PoolInfo& info){
		return std::hash<uint32_t>{}(info.familyIndex) ^ std::hash<VkFlags>{}(info.flags);
	}

	bool CommandPools::PoolInfo::operator==(const PoolInfo& other) const{
		return familyIndex == other.familyIndex && flags != other.flags;
	}

	CommandPool& CommandPools::create(const PoolInfo& info){
		auto device = _context.device().get();
		auto& allocationCallbacks = _context.graphics().allocationCallbacks();

		VkCommandPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.queueFamilyIndex = info.familyIndex;
		createInfo.flags = info.flags;

		VkCommandPool pool;

		if (vkCreateCommandPool(device, &createInfo, allocationCallbacks, &pool) != VK_SUCCESS){
			_context.logger().error("Failed to create a command pool");
			throw std::runtime_error("Failed to create a command pool");
		}

		std::unique_ptr<CommandPool> commandPool = std::make_unique<CommandPool>(_context, pool, info.familyIndex, info.flags);
		CommandPool& ref = *commandPool;
		_commandPools[info] = std::move(commandPool);

		return ref;
	}

}