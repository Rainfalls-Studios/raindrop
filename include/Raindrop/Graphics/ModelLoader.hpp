#ifndef __RAINDROP_GRAPHICS_MODEL_LOADER_HPP__
#define __RAINDROP_GRAPHICS_MODEL_LOADER_HPP__

#include "types.hpp"
#include "pch.pch"
#include "Formats.hpp"

namespace Raindrop::Graphics{
	class ModelLoader{
		public:
			ModelLoader() noexcept;
			~ModelLoader();

			ModelLoader& prepare(Context& context);
			void initialize();
			void release();

			std::shared_ptr<Model> load(const std::filesystem::path& filepath, const ModelLayoutConfig& config);
			std::vector<MeshData> loadMeshes(const std::filesystem::path& filepath, const ModelLayoutConfig& config);

		private:
			Context* _context;

			

			
	};
}

#endif