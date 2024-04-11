#ifndef __RAINDROP_GRAPHICS_MODELS_MODEL_HPP__
#define __RAINDROP_GRAPHICS_MODELS_MODEL_HPP__

#include "common.hpp"
#include <Raindrop/Core/Assets/Asset.hpp>
#include <Raindrop/Graphics/Textures/Texture.hpp>
#include <Raindrop/Graphics/Materials/Material.hpp>

namespace Raindrop::Graphics::Models{
	class Model : public ::Raindrop::Core::Assets::Asset{
		public:
			Model(Context& context, const Path& path);
			virtual ~Model() override;

			void render(VkCommandBuffer commandBuffer);

			std::vector<std::unique_ptr<Mesh>>::iterator begin();
			std::vector<std::unique_ptr<Mesh>>::iterator end();

			std::size_t meshCount() const;
			Mesh& mesh(const std::size_t& ID) const;

		private:
			Context& _context;
			std::vector<std::unique_ptr<Mesh>> _meshes;
			std::vector<Materials::MaterialID> _materials;
	};
}

#endif