#ifndef __RAINDROP_GRAPHICS_MESH_DATA_HPP__
#define __RAINDROP_GRAPHICS_MESH_DATA_HPP__

#include "types.hpp"
#include "pch.pch"

namespace Raindrop::Graphics{
	class MeshData{
		public:
			MeshData() noexcept;
			~MeshData();

			void prepare(Context& context);

			

		private:
			struct BindingBuffer{
				std::vector<uint8_t> data;
				std::size_t stride;
			};

			struct AttributeData{
				std::size_t binding;
				std::size_t offset;
			};

			Context* _context;
			std::vector<BindingBuffer> _buffers;
			std::vector<std::size_t> _indices;
			std::unordered_map<std::string, AttributeData> _attributes;
	};
}

#endif