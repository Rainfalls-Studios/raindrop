#ifndef __RAINDROP_GRAPHICS_MATERIAL_REGISTRY_HPP__
#define __RAINDROP_GRAPHICS_MATERIAL_REGISTRY_HPP__

#include "common.hpp"
#include "Material.hpp"

namespace Raindrop::Graphics::Materials{
	class Registry{
		public:
			struct MaterialPair{
				using Context = ::Raindrop::Graphics::Context;

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
		
		private:
			Context& _context;

			std::vector<MaterialPair> _handles;
			std::queue<MaterialID> _freeHandles;

			MaterialID popNextID();
			
	};
}

#endif