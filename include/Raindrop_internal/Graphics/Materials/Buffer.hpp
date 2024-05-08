#ifndef __RAINDROP_INTERNAL_GRAPHICS_MATERIAL_BUFFER_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_MATERIAL_BUFFER_HPP__

#include "common.hpp"
#include "Material.hpp"

namespace Raindrop::Internal::Graphics::Materials{
	class Buffer{
		public:
			Buffer(Context& context);
			~Buffer();

			VkBuffer get() const;
			VkDeviceMemory memory() const;

			void write(const std::size_t& index, const Material::Properties& data);

			std::size_t alignedInstanceSize() const;

			VkDescriptorBufferInfo info(const std::size_t& index);

		private:
			Context& _context;

			VkBuffer _buffer;
			VkDeviceMemory _memory;

			std::size_t _instanceCount;
			std::size_t _instanceSize;
			std::size_t _alignedInstanceSize;

			void allocate(const std::size_t instanceCount);
	};
}

#endif