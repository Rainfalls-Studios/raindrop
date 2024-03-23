#ifndef __RAINDROP_RENDERER_MATERIAL_BUFFER_HPP__
#define __RAINDROP_RENDERER_MATERIAL_BUFFER_HPP__

#include "common.hpp"
#include "Material.hpp"

namespace Raindrop::Renderer::Material{
	class Buffer{
		public:
			Buffer(Context& context);
			~Buffer();

			VkBuffer get() const;
			VkDeviceMemory memory() const;

			void write(const std::size_t& index, const Material::Properties& data);

		private:
			Context& _context;

			VkBuffer _buffer;
			VkDeviceMemory _memory;

			void allocate(const std::size_t instanceCount);
	};
}

#endif