#ifndef __RAINDROP_RENDERER_MATERIAL_MATERIAL_HPP__
#define __RAINDROP_RENDERER_MATERIAL_MATERIAL_HPP__

#include <Raindrop/Renderer/Context.hpp>
#include <Raindrop/Renderer/Texture/Texture.hpp>

namespace Raindrop::Renderer::Material{
	struct Material{
		using Texture = Texture::Texture;

		// glm::vec3 ambientColor;
		// glm::vec3 diffuseColor;
		// glm::vec3 specularColor;
		// float shininess;
		
		std::weak_ptr<Texture> diffuseTexture;
	};
}

#endif