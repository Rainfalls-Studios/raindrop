#ifndef __RAINDROP_GRAPHICS_BUFFERS_VERTEX_LAYOUT_REGISTRY_HPP__
#define __RAINDROP_GRAPHICS_BUFFERS_VERTEX_LAYOUT_REGISTRY_HPP__

#include <Raindrop/Graphics/Buffers/common.hpp>

namespace Raindrop::Graphics::Buffers{
	class VertexLayoutRegistry{
		public:
			VertexLayoutRegistry(Context& context);
			~VertexLayoutRegistry();

			const std::shared_ptr<VertexLayout> get(const std::string& name) const;
			std::shared_ptr<VertexLayout>& get(const std::string& name);
			void remove(const std::string& name);

		private:
			Context& _context;
			std::unordered_map<std::string, std::shared_ptr<VertexLayout>> _layouts;

	};
}

#endif