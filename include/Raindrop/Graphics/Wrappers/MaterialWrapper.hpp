#ifndef __RAINDROP_GRAPHICS_WRAPPERS_MATERIAL_HPP__
#define __RAINDROP_GRAPHICS_WRAPPERS_MATERIAL_HPP__

#include "common.hpp"
#include "../Materials/common.hpp"
#include "../../Utils/Wrapper.hpp"

namespace Raindrop::Graphics::Wrappers{
	class MaterialWrapper : public Utils::Wrapper<Context, Materials::Material, Materials::MaterialID, Materials::PLACEHOLDER_MATERIAL_ID>{
		public:
			using MaterialID = Materials::MaterialID;
			using Material = Materials::Material;

			using Wrapper::Wrapper;
			using Wrapper::operator=;
			using Wrapper::operator->;
			using Wrapper::ID;

			virtual Material& get() override;
			virtual const Material& get() const override;

			void update();
			void update(const Material& other);
			void update(const MaterialWrapper& other);
	};
}

#endif