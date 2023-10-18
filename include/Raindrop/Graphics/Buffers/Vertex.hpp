#ifndef __RAINDROP_GRAPHICS_BUFFERS_VERTEX_HPP__
#define __RAINDROP_GRAPHICS_BUFFERS_VERTEX_HPP__

#include <Raindrop/Graphics/Buffers/common.hpp>

namespace Raindrop::Graphics::Buffers{
	class Vertex{
		public:
			Vertex(const VertexLayout& layout);
			Vertex(const VertexLayout& layout, const std::vector<void*>& attributes);
			~Vertex();

			const VertexLayout& layout() const;

			template<typename T> T& get(const std::string& attribute);
			template<typename T> const T& get(const std::string& attribute) const;
		
		private:
			std::unique_ptr<VertexLayout> _layout;

			bool _reference;
			std::vector<void*> _attributes;

			void* get(const std::string& attribute);
			const void* get(const std::string& attribute) const;
	};
}

#endif