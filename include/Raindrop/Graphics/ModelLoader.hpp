#ifndef __RAINDROP_GRAPHICS_MODEL_LOADER_HPP__
#define __RAINDROP_GRAPHICS_MODEL_LOADER_HPP__

#include "types.hpp"
#include "pch.pch"

namespace Raindrop::Graphics{
	class ModelLoader{
		public:
			enum RequiredProperties{
				POSITION = 1 << 0,
				NORMAL = 1 << 1,
				COLOR = 1 << 2,
				UV = 1 << 3,
				WEIGHT = 1 << 4, // not supported
				TANGENT = 1 << 5,
				BITANGENT = 1 << 6,
				SMOOTH_NORMAL = 1 << 7
			};

			ModelLoader() noexcept;
			~ModelLoader();

			ModelLoader& prepare(Context& context);
			void initialize();
			void release();

			std::shared_ptr<Model> load(const std::filesystem::path& filepath, const std::uint32_t& properties = POSITION | NORMAL | UV);
			std::vector<MeshData> loadMeshes(const std::filesystem::path& filepath, const std::uint32_t& properties = POSITION | NORMAL | UV);

		private:
			Context* _context;

			VertexLayout createVertexLayout(std::uint32_t properties);
			
	};
}

#endif