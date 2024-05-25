#ifndef __RAINDROP_INTERNAL_VERTEX_HPP__
#define __RAINDROP_INTERNAL_VERTEX_HPP__

#include <Raindrop/Vertex.hpp>
#include <Raindrop_internal/Graphics/common.hpp>

namespace Raindrop{
	VkVertexInputRate toVulkan(const Vertex::InputRate& r){
		switch (r){
			case Vertex::InputRate::PER_INSTANCE: return VK_VERTEX_INPUT_RATE_INSTANCE;
			case Vertex::InputRate::PER_VERTEX: return VK_VERTEX_INPUT_RATE_VERTEX;
		}
		throw std::runtime_error("Invalid vertex input rate");
	}
}

#endif