#ifndef __RAINDROP_CORE_MEMORY_PROXY_ALLOCATOR_HPP__
#define __RAINDROP_CORE_MEMORY_PROXY_ALLOCATOR_HPP__

#include <Core/Memory/allocators/Allocator.hpp>

namespace Raindrop::Core::Memory{
	class RAINDROP_API ProxyAllocator : public Allocator {
		public: 
			ProxyAllocator(Allocator& allocator); 
			~ProxyAllocator(); 

			void* allocate(usize size, uint8 alignment) override; 
			void deallocate(void* p) override; 
		private: 
			Allocator& allocator; 
	}; 
}

#endif