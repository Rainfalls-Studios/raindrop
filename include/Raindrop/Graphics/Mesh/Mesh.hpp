#ifndef __RAINDROP_GRAPHICS_MESH_MESH_HPP__
#define __RAINDROP_GRAPHICS_MESH_MESH_HPP__

#include <Raindrop/Graphics/Mesh/common.hpp>

namespace Raindrop::Graphics::Mesh{
	class Mesh{
		public:
			Mesh() = default;
			virtual ~Mesh() = default;

			virtual bool hasVertexBuffer() const = 0;
			virtual bool hasIndexBuffer() const = 0;

			virtual const Buffer& vertexBuffer() const = 0;
			virtual const Buffer& indexBuffer() const = 0;
	};
}

#endif