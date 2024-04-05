#ifndef __RAINDROP_GRAPHICS_WRAPPERS_MATERIAL_HPP__
#define __RAINDROP_GRAPHICS_WRAPPERS_MATERIAL_HPP__

#include "common.hpp"
#include "../Materials/common.hpp"

namespace Raindrop::Graphics::Wrappers{
	class MaterialWrapper{
		public:
			using MaterialID = Materials::MaterialID;
			using Material = Materials::Material;

			MaterialWrapper(Context& context);
			MaterialWrapper(Context& context, const MaterialID& ID);
			MaterialWrapper(const MaterialWrapper& other);

			MaterialWrapper& operator=(const MaterialWrapper& other);
			MaterialWrapper& operator=(const MaterialID& ID);

			~MaterialWrapper();

			Material& get();
			const Material& get() const;

			MaterialID ID() const;

			Material* operator->();
			const Material* operator->() const;

			void update();
			void update(const Material& other);
			void update(const MaterialWrapper& other);

		private:
			Context* _context;
			MaterialID _ID;
	};
}

#endif