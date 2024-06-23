#include <Raindrop/TextureSubset.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/TextureSubset.hpp>
#include <Raindrop_internal/Texture.hpp>
#include <Raindrop_internal/Graphics/ImageView.hpp>
#include <Raindrop_internal/Graphics/Engine.hpp>
#include <Raindrop_internal/Color.hpp>
#include <Raindrop_internal/Format.hpp>

#define LOGGER _impl->context->getLogger()
#define GRAPHICS_CONTEXT _impl->context->getEngine().getContext()

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
		_impl->context = context.getInternalContext();
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
		LOGGER->info("Initializing new texture subset...");

		Internal::Graphics::ImageViewConfigInfo info{};

		info.image = _impl->source->getImpl()->internal;
		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = TextureSubsetFlagsToVulkan(_impl->flags);
		info.VkImageViewCreateInfo::image = _impl->source->getImpl()->internal->get();
		info.viewType = TextureSubsetTypeToVulkan(_impl->type);
		info.format = FormatToVulkan(_impl->format);

		info.components = {
			.r = ComponentSwizzleToVulkan(_impl->componentSwizzle.r),
			.g = ComponentSwizzleToVulkan(_impl->componentSwizzle.g),
			.b = ComponentSwizzleToVulkan(_impl->componentSwizzle.b),
			.a = ComponentSwizzleToVulkan(_impl->componentSwizzle.a),
		};

		info.subresourceRange = {
			.aspectMask = TextureAspectToVulkan(_impl->range.aspect),
			.baseMipLevel = static_cast<uint32_t>(_impl->range.mipmapLevel),
			.levelCount = static_cast<uint32_t>(_impl->range.mipmapCount),
			.baseArrayLayer = static_cast<uint32_t>(_impl->range.layer),
			.layerCount = static_cast<uint32_t>(_impl->range.layerCount)
		};

		_impl->internal = std::make_shared<Internal::Graphics::ImageView>(GRAPHICS_CONTEXT, info);

		LOGGER->info("Texture subset initialized with success !");
	}

	void TextureSubset::release(){
		_impl->internal.reset();
	}

	bool TextureSubset::isInitialized() const noexcept{
		return _impl->internal != nullptr;
	}

	void* TextureSubset::getNativeHandle() const noexcept{
		return static_cast<void*>(_impl->internal->get());
	}

	void TextureSubset::setSource(const Texture& source){
		_impl->source = &source;
	}

	void TextureSubset::setRange(const Range& range){
		_impl->range = range;
	}

	void TextureSubset::swizzleComponent(const Color::Components::Bits& component, const Color::Swizzle& swizzle){
		switch (component){
			case Color::Components::RED: _impl->componentSwizzle.r = swizzle; break;
			case Color::Components::GREEN: _impl->componentSwizzle.g = swizzle; break;
			case Color::Components::BLUE: _impl->componentSwizzle.b = swizzle; break;
			case Color::Components::ALPHA: _impl->componentSwizzle.a = swizzle; break;
		}
	}

	void TextureSubset::setType(const Type& type){
		_impl->type = type;
	}

	void TextureSubset::setFormat(const Format& format){
		_impl->format = format;
	}

	void TextureSubset::setFlags(const Flags& flags){
		_impl->flags = flags;
	}

	const Texture& TextureSubset::getSource() const noexcept{
		return *_impl->source;
	}

	const TextureSubset::Range& TextureSubset::getRange() const noexcept{
		return _impl->range;
	}

	const Color::Swizzle& TextureSubset::getComponentSwizzle(const Color::Components::Bits& component) const{
		switch (component){
			case Color::Components::RED: return _impl->componentSwizzle.r;
			case Color::Components::GREEN: return _impl->componentSwizzle.g;
			case Color::Components::BLUE: return _impl->componentSwizzle.b;
			case Color::Components::ALPHA: return _impl->componentSwizzle.a;
			default: throw std::runtime_error("Invalid color component");
		}
	}

	const TextureSubset::Type& TextureSubset::getType() const noexcept{
		return _impl->type;
	}

	const Format& TextureSubset::getFormat() const noexcept{
		return _impl->format;
	}

	const TextureSubset::Flags& TextureSubset::getFlags() const noexcept{
		return _impl->flags;
	}

	TextureSubset::Impl* TextureSubset::getImpl() const noexcept{
		return _impl.get();
	}
}