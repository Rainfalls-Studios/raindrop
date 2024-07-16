#ifndef __RAINDROP_GRAPHICS_COMMAND_POOL_HPP__
#define __RAINDROP_GRAPHICS_COMMAND_POOL_HPP__

#include "prototypes.hpp"
#include "pch.pch"

namespace Raindrop::Graphics{
	class CommandPool{
		public:
			CommandPool();
			~CommandPool();

			void prepare(Context& context);
			void initialize();
			void release();

			CommandPool& setCreateFlags(const VkCommandPoolCreateFlags& flags);
			CommandPool& setQueueFamily(const std::uint32_t& index);

			const VkCommandPool& get() const noexcept;

		private:
			Context* _context;
			VkCommandPool _pool;

			VkCommandPoolCreateFlags _flags;
			std::uint32_t _familyIndex;
	};
}

#endif