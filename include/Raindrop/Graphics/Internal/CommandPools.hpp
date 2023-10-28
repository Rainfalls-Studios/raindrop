#ifndef __RAINDROP_GRAPHICS_INTERNAL_COMMAND_POOLS_HPP__
#define __RAINDROP_GRAPHICS_INTERNAL_COMMAND_POOLS_HPP__

#include <Raindrop/Graphics/Internal/common.hpp>

namespace Raindrop::Graphics::Internal{
	class CommandPools{
		public:
			CommandPools(Context& context);
			~CommandPools();

			CommandPool& pool(const QueueProperties& properties, VkCommandPoolCreateFlags flags);
			const CommandPool& pool(const QueueProperties& properties, VkCommandPoolCreateFlags flags) const;
			
			
		private:
			struct PoolInfo{
				uint32_t familyIndex;
				VkCommandPoolCreateFlags flags;

				static std::size_t hash(const PoolInfo& info);
			};

			Context& _context;
			std::unordered_map<PoolInfo, std::unique_ptr<CommandPool>, decltype(PoolInfo::hash)> _commandPools;

			CommandPool& create(const PoolInfo& info);
	};
}

#endif