#include <Raindrop/TextureSubset.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/TextureSubset.hpp>
#include <Raindrop_internal/Texture.hpp>
#include <Raindrop_internal/Graphics/ImageView.hpp>
#include <Raindrop_internal/Graphics/Engine.hpp>
#include <Raindrop_internal/Color.hpp>
#include <Raindrop_internal/Format.hpp>

#define LOGGER _impl->context->getInternalContext()->getLogger()
#define GRAPHICS_CONTEXT _impl->context->getInternalContext()->getEngine().getContext()

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

		return std::move(textureSubset);
	}

	TextureSubset::TextureSubset(Context& context) : _impl{std::make_unique<Impl>()}{
		_impl->context = &context;
		_impl->builder = std::make_unique<Impl::Builder>();
	}

	TextureSubset::~TextureSubset(){
		_impl.reset();
	}

	TextureSubset::TextureSubset(TextureSubset&& other) : _impl{std::move(other._impl)}{}

	TextureSubset& TextureSubset::operator=(TextureSubset&& other){
		_impl = std::move(other._impl);
		return* this;
	}

	void TextureSubset::initialize(){
		if (!_impl->builder){
			// ERROR
			return;
		}

		LOGGER->info("Initializing new texture subset...");

		_impl->builder->info.update();

		_impl->imageView = std::make_shared<Internal::Graphics::ImageView>(GRAPHICS_CONTEXT, _impl->builder->info);
		_impl->builder.reset();

		LOGGER->info("Texture subset initialized with success !");
	}

	void TextureSubset::release(){
		_impl->imageView.reset();
		_impl->builder = std::make_unique<Impl::Builder>();
	}

	bool TextureSubset::isInitialized() const noexcept{
		return _impl->imageView != nullptr;
	}

	void* TextureSubset::getNativeHandle() const noexcept{
		return static_cast<void*>(_impl->imageView->get());
	}

	void TextureSubset::setSource(const Texture& source){
		_impl->builder->info.image = source.getImpl()->image;
	}

	void TextureSubset::setRange(const Range& range){
		_impl->builder->info.subresourceRange = VkImageSubresourceRange{
			.aspectMask = TextureAspectToVulkan(range.aspect),
			.baseMipLevel = static_cast<uint32_t>(range.mipmapLevel),
			.levelCount = static_cast<uint32_t>(range.mipmapCount),
			.baseArrayLayer = static_cast<uint32_t>(range.layer),
			.layerCount = static_cast<uint32_t>(range.layerCount),
		};
	}

	void TextureSubset::swizzleComponent(const Color::Components::Bits& component, const Color::Swizzle& swizzle){
		VkComponentSwizzle vkSwizzle = ComponentSwizzleToVulkan(swizzle);

		switch (component){
			case Color::Components::RED: _impl->builder->info.components.r = vkSwizzle; break;
			case Color::Components::GREEN: _impl->builder->info.components.g = vkSwizzle; break;
			case Color::Components::BLUE: _impl->builder->info.components.b = vkSwizzle; break;
			case Color::Components::ALPHA: _impl->builder->info.components.a = vkSwizzle; break;
		}
	}

	void TextureSubset::setType(const Type& type){
		_impl->builder->info.viewType = TextureSubsetTypeToVulkan(type);
	}

	void TextureSubset::setFormat(const Format& format){
		_impl->builder->info.format = FormatToVulkan(format);
	}

	void TextureSubset::setFlags(const Flags& flags){
		_impl->builder->info.flags = TextureSubsetFlagsToVulkan(flags);
	}

	Texture TextureSubset::getSource() const noexcept{
		std::unique_ptr<Texture::Impl> temp = std::make_unique<Texture::Impl>();

		temp->context = _impl->context;
		temp->image = _impl->imageView->getImage();

		return Texture(std::move(temp));
	}

	TextureSubset::Range TextureSubset::getRange() const noexcept{
		const auto& range = _impl->imageView->getSubresourceRange();
		return TextureSubset::Range{
			.aspect = TextureAspectToRaindrop(range.aspectMask),
			.mipmapLevel = range.baseMipLevel,
			.mipmapCount = range.levelCount,
			.layer = range.baseArrayLayer,
			.layerCount = range.layerCount
		};
	}

	Color::Swizzle TextureSubset::getComponentSwizzle(const Color::Components::Bits& component) const noexcept{
		VkComponentSwizzle swizzle = VK_COMPONENT_SWIZZLE_IDENTITY;
		switch (component){
			case Color::Components::RED: swizzle = _impl->builder->info.components.r; break;
			case Color::Components::GREEN: swizzle = _impl->builder->info.components.g; break;
			case Color::Components::BLUE: swizzle = _impl->builder->info.components.b; break;
			case Color::Components::ALPHA: swizzle = _impl->builder->info.components.a; break;
		}

		return ComponentSwizzleToRaindrop(swizzle);
	}

	TextureSubset::Type TextureSubset::getType() const noexcept{
		return TextureSubsetTypeToRaindrop(_impl->imageView->getViewType());
	}

	Format TextureSubset::getFormat() const noexcept{
		return FormatToRaindrop(_impl->imageView->getFormat());
	}

	TextureSubset::Flags TextureSubset::getFlags() const noexcept{
		return TextureSubsetFlagsToRaindrop(_impl->imageView->getFlags());
	}

	TextureSubset::Impl* TextureSubset::getImpl() const noexcept{
		return _impl.get();
	}
}