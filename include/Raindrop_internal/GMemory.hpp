#ifndef __RAINDROP_INTERNAL_GMEMORY_HPP__
#define __RAINDROP_INTERNAL_GMEMORY_HPP__

#include <Raindrop/GMemory.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop_internal/Graphics/Memory.hpp>

namespace Raindrop{
	struct GMemory::Impl{
		Internal::Context* context;

		std::size_t size;
		std::size_t typeIndex;
		std::shared_ptr<Internal::Graphics::Memory> internal;

		Impl() : 
			context{nullptr},
			internal{nullptr}
		{}
	};

	struct GMemory::Type::Impl{
		Internal::Context* context;
		std::size_t index;

		Internal::Graphics::MemoryType& get();
		const Internal::Graphics::MemoryType& get() const;

		Impl() : 
			context{nullptr},
			index{SIZE_MAX}
		{}
	};

	constexpr VkMemoryPropertyFlags GMemoryTypeFlagsToVulkan(const GMemory::Type::Flags& flags){
		VkMemoryPropertyFlags out = 0;

		if (flags.has(GMemory::Type::Flags::GPU_LOCAL)) out |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		if (flags.has(GMemory::Type::Flags::CPU_VISIBLE)) out |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		if (flags.has(GMemory::Type::Flags::CPU_COHERENT)) out |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		if (flags.has(GMemory::Type::Flags::CPU_CACHED)) out |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
		if (flags.has(GMemory::Type::Flags::LAZY_ALLOCATION)) out |= VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
		if (flags.has(GMemory::Type::Flags::PROTECTED)) out |= VK_MEMORY_PROPERTY_PROTECTED_BIT;

		// VK_AMD_device_coherent_memory
		if (flags.has(GMemory::Type::Flags::GPU_COHERENT)) out |= VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD;
		if (flags.has(GMemory::Type::Flags::GPU_UNCACHED)) out |= VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD;

		return out;
	}

	constexpr GMemory::Type::Flags GMemoryTypeFlagsToRaindrop(const VkMemoryPropertyFlags& flags){
		GMemory::Type::Flags out = GMemory::Type::Flags::NONE;

		if (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) out |= GMemory::Type::Flags::GPU_LOCAL;
		if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) out |= GMemory::Type::Flags::CPU_VISIBLE;
		if (flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) out |= GMemory::Type::Flags::CPU_COHERENT;
		if (flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) out |= GMemory::Type::Flags::CPU_CACHED;
		if (flags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) out |= GMemory::Type::Flags::LAZY_ALLOCATION;
		if (flags & VK_MEMORY_PROPERTY_PROTECTED_BIT) out |= GMemory::Type::Flags::PROTECTED;
		if (flags & VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD) out |= GMemory::Type::Flags::GPU_COHERENT;
		if (flags & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD) out |= GMemory::Type::Flags::GPU_UNCACHED;

		return out;
	}

	constexpr GMemory::Type::Flags::Bits GMemoryTypeFlagsToVulkan(const VkMemoryPropertyFlagBits& flags){
		switch (flags){
			case VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT: return GMemory::Type::Flags::GPU_LOCAL;
			case VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT: return GMemory::Type::Flags::CPU_VISIBLE;
			case VK_MEMORY_PROPERTY_HOST_COHERENT_BIT: return GMemory::Type::Flags::CPU_COHERENT;
			case VK_MEMORY_PROPERTY_HOST_CACHED_BIT: return GMemory::Type::Flags::CPU_CACHED;
			case VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT: return GMemory::Type::Flags::LAZY_ALLOCATION;
			case VK_MEMORY_PROPERTY_PROTECTED_BIT: return GMemory::Type::Flags::PROTECTED;
			case VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD: return GMemory::Type::Flags::GPU_COHERENT;
			case VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD: return GMemory::Type::Flags::GPU_UNCACHED;
			default: return GMemory::Type::Flags::NONE;
		}
	}

	constexpr VkMemoryPropertyFlagBits GMemoryTypeFlagsToVulkan(const GMemory::Type::Flags::Bits& flags){
		switch (flags){
			case GMemory::Type::Flags::GPU_LOCAL: return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			case GMemory::Type::Flags::CPU_VISIBLE: return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			case GMemory::Type::Flags::CPU_COHERENT: return VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			case GMemory::Type::Flags::CPU_CACHED: return VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
			case GMemory::Type::Flags::LAZY_ALLOCATION: return VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
			case GMemory::Type::Flags::PROTECTED: return VK_MEMORY_PROPERTY_PROTECTED_BIT;
			case GMemory::Type::Flags::GPU_COHERENT: return VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD;
			case GMemory::Type::Flags::GPU_UNCACHED: return VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD;
			default: return VkMemoryPropertyFlagBits(0);
		}
	}
}


#endif