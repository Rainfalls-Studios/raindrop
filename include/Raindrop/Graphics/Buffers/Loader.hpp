#ifndef __RAINDROP_GRAPHICS_BUFFERS_LOADER_HPP__
#define __RAINDROP_GRAPHICS_BUFFERS_LOADER_HPP__

#include <Raindrop/Graphics/Buffers/common.hpp>

namespace Raindrop::Graphics::Buffers{
	class Loader{
		public:
			Loader(Context& context);
			~Loader();

			void loadVertexLayouts(const YAML::Node& node);

			static std::string VertexAttributeTypeToStr(const VertexAttributeType& value);
			static VertexAttributeType strToVertexAttributeType(const std::string& str);

		private:
			Context& _context;
			
			void loadVertexLayout(const YAML::Node& node);
	};
}

namespace YAML{
	template<>
	struct convert<Raindrop::Graphics::Buffers::VertexAttributeType>{

		static Node encode(const Raindrop::Graphics::Buffers::VertexAttributeType& value){
			Node node;
			node.push_back(Raindrop::Graphics::Buffers::Loader::VertexAttributeTypeToStr(value));
			return node;
		}

		static bool decode(const Node& node, Raindrop::Graphics::Buffers::VertexAttributeType& value){
			value = Raindrop::Graphics::Buffers::Loader::strToVertexAttributeType(node.as<std::string>());
			return true;
		}
	};
}

#endif