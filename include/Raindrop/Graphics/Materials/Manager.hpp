#ifndef __RAINDROP_GRAPHICS_MATERIAL_MANAGER_HPP__
#define __RAINDROP_GRAPHICS_MATERIAL_MANAGER_HPP__

#include "common.hpp"
#include "Material.hpp"

namespace Raindrop::Graphics::Materials{
	class Manager{
		public:
			Manager(::Raindrop::Graphics::Context& context);
			~Manager();

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