#ifndef __RAINDROP_GRAPHICS_MATERIAL_MATERIAL_HPP__
#define __RAINDROP_GRAPHICS_MATERIAL_MATERIAL_HPP__

#include "common.hpp"
#include <Raindrop/Graphics/Textures/Texture.hpp>
#include <Raindrop/Core/Assets/common.hpp>

namespace Raindrop::Graphics::Materials{
	struct Material{
		using Texture = Textures::Texture;
		using Context = ::Raindrop::Graphics::Context;

		Material(Context& context);
		Material(const Material& other);
		Material& operator=(const Material& other);

		struct Properties{
			Maths::vec4 ambientColor;
			Maths::vec4 diffuseColor;
			Maths::vec4 specularColor;
			float shininess;
			float specularFactor;

			bool operator==(const Properties& other) const;
		} properties;
		
		struct Textures{
			Textures(Context& context);
			Textures(const Textures& other);
			Textures& operator=(const Textures& other);

			Context& context;
				
			const Texture& getDiffuse() const;
			std::shared_ptr<Texture> diffuse = nullptr;

			bool operator==(const Textures& other) const;
		} textures;

		bool operator==(const Material& other) const noexcept;

	};

	using __MaterialClass__ = Material;
}

#endif