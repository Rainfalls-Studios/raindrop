#include <Raindrop/TextureSubset.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/TextureSubset.hpp>
#include <Raindrop_internal/Texture.hpp>
#include <Raindrop_internal/Graphics/ImageView.hpp>
#include <Raindrop_internal/Graphics/Renderer.hpp>

#define LOGGER _impl->context->getInternalContext()->getLogger()
#define INFO _impl->info
#define GRAPHICS_CONTEXT _impl->context->getInternalContext()->getRenderer().getContext()

namespace Raindrop{
	TextureSubset TextureSubset::Create(Context& context, const Texture& source){
		TextureSubset textureSubset(context);
		textureSubset.setSource(source);
		textureSubset.setFormat(source.getFormat());

		switch (source.getType()){
			case Texture::Type::LINEAR:	textureSubset.setType(Type::LINEAR); break;
			case Texture::Type::PLANAR:	textureSubset.setType(Type::PLANAR); break;
			case Texture::Type::VOLUMETRIC:	textureSubset.setType(Type::VOLUMETRIC); break;
		}
		
		textureSubset.setRange(Range{
			.aspect = Texture::Aspect::COLOR,
			.mipmapLevel = 0,
			.mipmapCount = 1,
			.layer = 0,
			.layerCount = 1
		});

		return textureSubset;
	}

	TextureSubset::TextureSubset(Context& context) : _impl{nullptr}{
		_impl = new Impl(context);
	}

	TextureSubset::~TextureSubset(){
		delete _impl;
	}

	TextureSubset::TextureSubset(const TextureSubset& other) : _impl{nullptr}{
		_impl = new Impl(*other._impl->context);
		*_impl = *other._impl;
	}

	TextureSubset& TextureSubset::operator=(const TextureSubset& other){
		*_impl = *other._impl;
		return *this;
	}

	void TextureSubset::initialize(){
		LOGGER->info("Initializing new texture subset...");
		_impl->imageView = std::make_shared<Internal::Graphics::ImageView>(GRAPHICS_CONTEXT, _impl->info);
		LOGGER->info("Texture subset initialized with success !");
	}

	void TextureSubset::release(){
		_impl->imageView.reset();
		INFO.image.reset();
	}

	bool TextureSubset::isInitialized() const noexcept{
		return _impl->imageView != nullptr;
	}

	void* TextureSubset::getNativeHandle() const noexcept{
		return static_cast<void*>(_impl->imageView->get());
	}

	void TextureSubset::setSource(const Texture& source){
		INFO.image = source.getImpl()->image;
	}

	void TextureSubset::setRange(const Range& range){
		INFO.subResource = VkImageSubresourceRange{
			.aspectMask = static_cast<VkImageAspectFlags>(range.aspect.get()),
			.baseMipLevel = static_cast<uint32_t>(range.mipmapLevel),
			.levelCount = static_cast<uint32_t>(range.mipmapCount),
			.baseArrayLayer = static_cast<uint32_t>(range.layer),
			.layerCount = static_cast<uint32_t>(range.layerCount),
		};
	}

	void TextureSubset::swizzleComponent(const Color::Components::Bits& component, const Color::Swizzle& swizzle){
		VkComponentSwizzle vkSwizzle = VK_COMPONENT_SWIZZLE_IDENTITY;
		switch (swizzle){
			case Color::Swizzle::RED: vkSwizzle = VK_COMPONENT_SWIZZLE_R; break;
			case Color::Swizzle::GREEN: vkSwizzle = VK_COMPONENT_SWIZZLE_G; break;
			case Color::Swizzle::BLUE: vkSwizzle = VK_COMPONENT_SWIZZLE_B; break;
			case Color::Swizzle::ALPHA: vkSwizzle = VK_COMPONENT_SWIZZLE_A; break;
			case Color::Swizzle::ONE: vkSwizzle = VK_COMPONENT_SWIZZLE_ONE; break;
			case Color::Swizzle::ZERO: vkSwizzle = VK_COMPONENT_SWIZZLE_ZERO; break;
			case Color::Swizzle::IDENTITY: vkSwizzle = VK_COMPONENT_SWIZZLE_IDENTITY; break;
		}

		switch (component){
			case Color::Components::RED: INFO.componentMapping.r = vkSwizzle; break;
			case Color::Components::GREEN: INFO.componentMapping.g = vkSwizzle; break;
			case Color::Components::BLUE: INFO.componentMapping.b = vkSwizzle; break;
			case Color::Components::ALPHA: INFO.componentMapping.a = vkSwizzle; break;
		}
	}

	void TextureSubset::setType(const Type& type){
		switch (type){
			case Type::LINEAR: INFO.viewType = VK_IMAGE_VIEW_TYPE_1D; break;
			case Type::PLANAR: INFO.viewType = VK_IMAGE_VIEW_TYPE_2D; break;
			case Type::VOLUMETRIC: INFO.viewType = VK_IMAGE_VIEW_TYPE_3D; break;
			case Type::CUBEMAP: INFO.viewType = VK_IMAGE_VIEW_TYPE_CUBE; break;
			case Type::LINEAR_ARRAY: INFO.viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY; break;
			case Type::PLANAR_ARRAY: INFO.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY; break;
			case Type::CUBEMAP_ARRAY: INFO.viewType = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY; break;
		}
	}

	void TextureSubset::setFormat(const Format& format){
		INFO.format = static_cast<VkFormat>(format.get());
	}

	void TextureSubset::setFlags(const Flags& flags){
		INFO.flags = static_cast<VkImageViewCreateFlags>(flags.get());
	}

	Texture TextureSubset::getSource() const noexcept{
		// TODO
	}

	TextureSubset::Range TextureSubset::getRange() const noexcept{
		return TextureSubset::Range{
			.aspect = static_cast<Texture::Aspect>(INFO.subResource.aspectMask),
			.mipmapLevel = static_cast<std::size_t>(INFO.subResource.baseMipLevel),
			.mipmapCount = static_cast<std::size_t>(INFO.subResource.levelCount),
			.layer = static_cast<std::size_t>(INFO.subResource.baseArrayLayer),
			.layerCount = static_cast<std::size_t>(INFO.subResource.layerCount)
		};
	}

	Color::Swizzle TextureSubset::getComponentSwizzle(const Color::Components::Bits& component) const noexcept{
		VkComponentSwizzle swizzle = VK_COMPONENT_SWIZZLE_IDENTITY;
		switch (component){
			case Color::Components::RED: swizzle = INFO.componentMapping.r; break;
			case Color::Components::GREEN: swizzle = INFO.componentMapping.g; break;
			case Color::Components::BLUE: swizzle = INFO.componentMapping.b; break;
			case Color::Components::ALPHA: swizzle = INFO.componentMapping.a; break;
		}

		switch (swizzle){
			case VK_COMPONENT_SWIZZLE_R: return Color::Swizzle::RED; 
			case VK_COMPONENT_SWIZZLE_G: return Color::Swizzle::GREEN; 
			case VK_COMPONENT_SWIZZLE_B: return Color::Swizzle::BLUE; 
			case VK_COMPONENT_SWIZZLE_A: return Color::Swizzle::ALPHA; 
			case VK_COMPONENT_SWIZZLE_ONE: return Color::Swizzle::ONE; 
			case VK_COMPONENT_SWIZZLE_ZERO: return Color::Swizzle::ZERO; 
			case VK_COMPONENT_SWIZZLE_IDENTITY: return Color::Swizzle::IDENTITY; 
		}
		return Color::Swizzle::IDENTITY;
	}

	TextureSubset::Type TextureSubset::getType() const noexcept{
		switch (INFO.viewType){
			case VK_IMAGE_VIEW_TYPE_1D: return Type::LINEAR;
			case VK_IMAGE_VIEW_TYPE_2D: return Type::PLANAR;
			case VK_IMAGE_VIEW_TYPE_3D: return Type::VOLUMETRIC;
			case VK_IMAGE_VIEW_TYPE_CUBE: return Type::CUBEMAP;
			case VK_IMAGE_VIEW_TYPE_1D_ARRAY: return Type::LINEAR_ARRAY;
			case VK_IMAGE_VIEW_TYPE_2D_ARRAY: return Type::PLANAR_ARRAY;
			case VK_IMAGE_VIEW_TYPE_CUBE_ARRAY: return Type::CUBEMAP_ARRAY;
		}
		return Type::PLANAR;
	}

	Format TextureSubset::getFormat() const noexcept{
		return static_cast<Format::Type>(INFO.format);
	}

	TextureSubset::Flags TextureSubset::getFlags() const noexcept{
		return static_cast<Flags::Bitset>(INFO.flags);
	}

	TextureSubset::Impl* TextureSubset::getImpl() const noexcept{
		return _impl;
	}
}