#include <Raindrop/TextureSubset.hpp>

namespace Raindrop{

	//--------------------------------------------------------------------
	//-----------------         TEXTURE SUBSET           -----------------
	//--------------------------------------------------------------------

	TextureSubset TextureSubset::Create(Context& context, const Texture& origin){
		// TODO
	}

	TextureSubset::TextureSubset(){
		// TODO
	}

	TextureSubset::TextureSubset(const TextureSubset& other){
		// TODO
	}

	TextureSubset& TextureSubset::operator=(const TextureSubset& other){
		// TODO
		return *this;
	}

	void TextureSubset::initialize(){
		// TODO
	}

	void TextureSubset::release(){
		// TODO
	}

	bool TextureSubset::isInitialized() const noexcept{
		// TODO
	}

	void* TextureSubset::getNativeHandle() const noexcept{
		// TODO
	}

	// TextureSubsetID getHandle() const noexcept; TODO: unique runtime texture subset scope id
	// InstanceID getInstanceID() const noexcept; TODO: unique runtime engine scope id

	void TextureSubset::setSource(const Texture& source){
		// TODO
	}

	void TextureSubset::setRange(const Range& range){
		// TODO
	}

	void TextureSubset::swizzleComponent(const Color::Component& component, const Color::Swizzle& swizzle){
		// TODO
	}

	void TextureSubset::setType(const Type& type){
		// TODO
	}

	void TextureSubset::setFormat(const Format& format){
		// TODO
	}

	void TextureSubset::setFlags(const Flags& flags){
		// TODO
	}

	const Texture& TextureSubset::getSource() const noexcept{
		// TODO
	}

	const TextureSubset::Range& TextureSubset::getRange() const noexcept{
		// TODO
	}

	const Color::Swizzle& TextureSubset::getComponentSwizzle(const Color::Component& component) const noexcept{
		// TODO
	}

	const TextureSubset::Type& TextureSubset::getType() const noexcept{
		// TOOD
	}

	const Format& TextureSubset::getFormat() const noexcept{
		// TOOD
	}

	const TextureSubset::Flags& TextureSubset::getFlags() const noexcept{
		// TOOD
	}

}