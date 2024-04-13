#include <Raindrop/Graphics/Materials/Material.hpp>
#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics::Materials{
	
	Material::Material(Context& context) : 
		properties{
			.diffuseColor = Maths::vec4(1.f),
			.specularColor = Maths::vec4(0.f),
			.shininess = 0.f	
		},
		textures{context}
	{}

	Material::Material(const Material& other) :
		properties{other.properties},
		textures{other.textures}
	{}

	Material& Material::operator=(const Material& other){
		assert(&textures.context == &other.textures.context && "Can't copy two materials from two different game instances");
		properties = other.properties;
		textures = other.textures;
		return *this;
	}

	bool Material::operator==(const Material& other) const noexcept{
		return properties == other.properties && textures == other.textures;
	}

	// PROPERTIES

	bool Material::Properties::operator==(const Properties& other) const{
		return (diffuseColor == other.diffuseColor) && (specularColor == other.specularColor) && (shininess == other.shininess);
	}

	// TEXTURES

	Material::Textures::Textures(Context& context) : context{context}{}

	Material::Textures::Textures(const Textures& other) : 
		context{other.context},
		diffuse{other.diffuse}
	{}

	bool Material::Textures::operator==(const Textures& other) const{
		return (diffuse == other.diffuse);
	}

	Material::Textures& Material::Textures::operator=(const Textures& other){
		assert(&context == &other.context && "Can't copy two material textures from two different game instances");
		diffuse = other.diffuse;
		return *this;
	}

	const Material::Texture& Material::Textures::getDiffuse() const{
		if (diffuse == nullptr){
			return *context.white.get();
		} else {
			return *diffuse;
		}
	}
}