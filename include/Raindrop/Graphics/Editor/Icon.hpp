#ifndef __RAIDNROP_GRAPHICS_EDITOR_ICON_HPP__
#define __RAIDNROP_GRAPHICS_EDITOR_ICON_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>
#include <Raindrop/Graphics/Editor/Image.hpp>

namespace Raindrop::Graphics::Editor{
	class Icon{
		friend class IconAtlas;
		public:
			const ImVec2& uv1() const;
			const ImVec2& uv2() const;
			ImTextureID texture() const;

		private:
			ImVec2 _uv1;
			ImVec2 _uv2;
			ImTextureID _id;

	};
}

#endif