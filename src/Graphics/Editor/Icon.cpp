#include <Raindrop/Graphics/Editor/Icon.hpp>

namespace Raindrop::Graphics::Editor{
	const ImVec2& Icon::uv1() const{
		return _uv1;
	}

	const ImVec2& Icon::uv2() const{
		return _uv2;
	}

	ImTextureID Icon::texture() const{
		return _id;
	}
}