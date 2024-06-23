#ifndef __RAINDROP_INTERNAL_VERTEX_HPP__
#define __RAINDROP_INTERNAL_VERTEX_HPP__

#include <Raindrop/Vertex.hpp>
#include <Raindrop_internal/Graphics/common.hpp>

namespace Raindrop{
	constexpr VkVertexInputRate VertexInputRateToVulkan(const Vertex::InputRate& r){
		switch (r){
			case Vertex::InputRate::PER_INSTANCE: return VK_VERTEX_INPUT_RATE_INSTANCE;
			case Vertex::InputRate::PER_VERTEX: return VK_VERTEX_INPUT_RATE_VERTEX;
		}
		throw std::runtime_error("Invalid vertex input rate");
	}

	constexpr std::size_t IndexBufferTypeToSize(const Vertex::IndexBuffer::Type& type){
		switch (type){
			case Vertex::IndexBuffer::Type::NONE: return 0;
			case Vertex::IndexBuffer::Type::UINT8: return 8;
			case Vertex::IndexBuffer::Type::UINT16: return 16;
			case Vertex::IndexBuffer::Type::UINT32: return 32;
			default: throw std::runtime_error("Invalid index type");
		}
	}

	constexpr VkIndexType IndexBufferTypeToVulkan(const Vertex::IndexBuffer::Type& type){
		switch (type){
			// VK_KHR_acceleration_structure
			case Vertex::IndexBuffer::Type::NONE: return VK_INDEX_TYPE_NONE_KHR;

			// VK_KHR_index_type_uint8
			case Vertex::IndexBuffer::Type::UINT8: return VK_INDEX_TYPE_UINT8_EXT;

			case Vertex::IndexBuffer::Type::UINT16: return VK_INDEX_TYPE_UINT16;
			case Vertex::IndexBuffer::Type::UINT32: return VK_INDEX_TYPE_UINT32;
			default: throw std::runtime_error("Invalid index type");
		}
	}
}

#endif