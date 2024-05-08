#ifndef __RAINDROP_INTERNAL_GRAPHICS_MATERIAL_REGISTRY_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_MATERIAL_REGISTRY_HPP__

#include "common.hpp"
#include "Material.hpp"

namespace Raindrop::Internal::Graphics::Materials{
	class Registry{
		public:
			struct MaterialPair{
				using Context = ::Raindrop::Internal::Graphics::Context;

				MaterialPair(Context& context);
				MaterialPair(const Material& material, VkDescriptorSet set);

				Material material;
				VkDescriptorSet set;
			};

			Registry(Context& context);
			~Registry();

			MaterialID registerMaterial();
			void updateMaterial(const MaterialID& ID, const MaterialPair& data);

			void unregisterMaterial(const MaterialID& ID);

			MaterialPair* get(const MaterialID& id);

			std::size_t size();
			std::size_t freeSize();
		
		private:
			Context& _context;

			std::vector<MaterialPair> _handles;
			std::queue<MaterialID> _freeHandles;

			MaterialID popNextID();
			
	};
}

#endif