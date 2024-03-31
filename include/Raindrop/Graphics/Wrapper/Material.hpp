#ifndef __RAINDROP_GRAPHICS_WRAPPER_MATERIAL_HPP__
#define __RAINDROP_GRAPHICS_WRAPPER_MATERIAL_HPP__

#include "common.hpp"
#include "../Material/common.hpp"

namespace Raindrop::Graphics::Wrapper{
	class MaterialWrapper{
		public:
			MaterialWrapper(Context& context);
			~MaterialWrapper();

			Material::Material& get();
			const Material::Material& get() const;

		private:
			Context& _context;
	};
}

#endif