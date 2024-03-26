#ifndef __RAINDROP_RENDERER_CORE_STRUCTS_HPP__
#define __RAINDROP_RENDERER_CORE_STRUCTS_HPP__

#include "common.hpp"

namespace Raindrop::Renderer::Core{
	struct QueueProperties{
		bool present;
		VkQueueFlags flags;

		QueueProperties(VkQueueFlags flags, bool present = false);
		QueueProperties& operator=(VkQueueFlags flags);

		bool operator==(const QueueProperties& other) const;
		bool operator!=(const QueueProperties& other) const;

		static std::size_t hash(const QueueProperties& properties);
	};
}

#endif