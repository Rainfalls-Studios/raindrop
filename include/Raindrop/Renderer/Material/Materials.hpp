#ifndef __RAINDROP_RENDERER_MATERIAL_MATERIALS_HPP__
#define __RAINDROP_RENDERER_MATERIAL_MATERIALS_HPP__

#include "common.hpp"
#include "Material.hpp"

namespace Raindrop::Renderer::Material{
	class Materials{
		public:
			Materials(::Raindrop::Renderer::Context& context);
			~Materials();

			MaterialID registerMaterial(const Material& material);
			void unregisterMaterial(const MaterialID& ID);

			Material& getMaterial(const MaterialID& ID);
			VkDescriptorSet getDescriptorSet(const MaterialID& ID);

			void updateMaterial(const MaterialID& ID);
			void updateMaterial(const MaterialID& ID, const Material& material);

			SetLayout& layout() const;

		private:
			Context* _context;
	};
}

#endif