#ifndef __RAINDROP_GRAPHICS_INTERNAL_STRUCTS_HPP__
#define __RAINDROP_GRAPHICS_INTERNAL_STRUCTS_HPP__

#include <Raindrop/Graphics/Internal/common.hpp>

namespace Raindrop::Graphics::Internal{
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