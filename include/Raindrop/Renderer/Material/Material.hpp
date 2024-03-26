#ifndef __RAINDROP_RENDERER_MATERIAL_MATERIAL_HPP__
#define __RAINDROP_RENDERER_MATERIAL_MATERIAL_HPP__

#include "common.hpp"
#include <Raindrop/Renderer/Texture/Texture.hpp>

namespace Raindrop::Renderer::Material{
	struct Material{
		using Texture = Texture::Texture;
		using Context = ::Raindrop::Renderer::Context;

		Material(Context& context);
		Material(const Material& other);
		Material& operator=(const Material& other);

		struct Properties{
			glm::vec3 diffuseColor;
			glm::vec3 specularColor;
			float shininess;

			bool operator==(const Properties& other) const;
		} properties;
		
		struct Textures{
			Textures(Context& context);
			Textures(const Textures& other);
			Textures& operator=(const Textures& other);

			Context& context;
				
			const Texture& getDiffuse() const;
			std::weak_ptr<Texture> diffuse;

			bool operator==(const Textures& other) const;
		} textures;

		bool operator==(const Material& other) const noexcept;

	};
}

#endif