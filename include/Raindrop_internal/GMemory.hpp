#ifndef __RAINDROP_INTERNAL_GMEMORY_HPP__
#define __RAINDROP_INTERNAL_GMEMORY_HPP__

#include <Raindrop/GMemory.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop_internal/Graphics/Memory.hpp>

namespace Raindrop{
	struct GMemory::Impl{
		std::shared_ptr<Internal::Graphics::Memory> memory;
		Context* context;

		Impl() : 
			context{nullptr},
			memory{nullptr}
		{}
	};

	struct GMemory::Type::Impl{
		std::size_t index;
		Internal::Graphics::Context* context;

		Internal::Graphics::MemoryType& get();
		const Internal::Graphics::MemoryType& get() const;

		Impl() : 
			context{nullptr},
			index{SIZE_MAX}
		{}
	};

	VkMemoryPropertyFlags toVulkan(const GMemory::Type::Flags& flags);
}


#endif