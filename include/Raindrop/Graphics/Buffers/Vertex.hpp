#ifndef __RAINDROP_GRAPHICS_BUFFERS_VERTEX_HPP__
#define __RAINDROP_GRAPHICS_BUFFERS_VERTEX_HPP__

#include <Raindrop/Graphics/Buffers/common.hpp>


namespace Raindrop::Graphics::Buffers{
	class Vertex{
		public:
			Vertex();
			Vertex(const VertexLayout& layout);
			Vertex(const VertexLayout& layout, const std::vector<void*>& attributes);
			~Vertex();

			Vertex(const Vertex& other);
			const Vertex& operator=(const Vertex& other) const;

			bool operator==(const Vertex& other) const;

			const VertexLayout& layout() const;

			template<typename T> T& get(const std::string& attribute);
			template<typename T> const T& get(const std::string& attribute) const;

			std::size_t hash(std::size_t seed = 0) const;
		
		private:
			std::unique_ptr<VertexLayout> _layout;

			bool _reference;
			std::vector<void*> _attributes;

			void* get(const std::string& attribute);
			const void* get(const std::string& attribute) const;
	};
}


namespace std{
	template<>
	struct hash<Raindrop::Graphics::Buffers::Vertex>{
		std::size_t operator()(const Raindrop::Graphics::Buffers::Vertex& vertex) const{
			return vertex.hash();
		}
	};
}

#endif