#include <Raindrop/Texture.hpp>
#include <Raindrop/GUID.hpp>

namespace Raindrop{
	const Texture::Operation Texture::Operation::DEFAULT = Texture::Operation(
		Texture::Operation::LOAD,
		Texture::Operation::STORE,
		Texture::Operation::DONT_LOAD,
		Texture::Operation::DONT_STORE
	);

	Texture::Operation::Operation(Load loadOp, Store storeOp, Load stencilLoadOp, Store stentilStorOp) noexcept : 
		_loadOp{loadOp},
		_storeOp{storeOp},
		_stencilLoadOp{stencilLoadOp},
		_stencilStoreOp{stentilStorOp}
	{}

	const Texture::Operation::Load& Texture::Operation::getLoadOp() const noexcept{
		return _loadOp;
	}

	const Texture::Operation::Store& Texture::Operation::getStoreOp() const noexcept{
		return _storeOp;
	}

	const Texture::Operation::Load& Texture::Operation::getStencilLoadOp() const noexcept{
		return _stencilLoadOp;
	}

	const Texture::Operation::Store& Texture::Operation::getStencilStoreOp() const noexcept{
		return _stencilStoreOp;
	}

	void Texture::Operation::setLoadOp(const Load& op) noexcept{
		_loadOp = op;;
	}

	void Texture::Operation::setStoreOp(const Store& op) noexcept{
		_storeOp = op;;
	}

	void Texture::Operation::setStencilLoadOp(const Load& op) noexcept{
		_stencilLoadOp = op;;
	}

	void Texture::Operation::setStencilStoreOp(const Store& op) noexcept{
		_stencilStoreOp = op;;
	}

	//--------------------------------------------------------------------
	//-----------------           TEXTURE                -----------------
	//--------------------------------------------------------------------

	Texture Texture::Create(Context& context, std::size_t width, std::size_t height, const Format& format, const Usage& usage){
		// TODO
	}

	Texture::Texture(){
		// TODO
	}

	Texture::Texture(const Texture& other){
		// TODO
	}

	Texture& Texture::operator=(const Texture& other){
		// TODO
		return *this;
	}

	void Texture::initialize(){
		// TODO
	}

	void Texture::release(){
		// TODO
	}


	void Texture::setFormat(const Format& format){
		// TODO
	}

	void Texture::setWidth(const std::size_t& width){
		// TODO
	}

	void Texture::setHeight(const std::size_t& height){
		// TODO
	}

	void Texture::setDepth(const std::size_t& depth){
		// TODO
	}

	void Texture::setUsage(const Usage& usage){
		// TODO
	}

	void Texture::setLayout(const Layout& layout){
		// TODO
	}

	void Texture::setTiling(const Tiling& tiling){
		// TODO
	}

	void Texture::setType(const Type& type){
		// TODO
	}

	void Texture::setMipmapCount(const std::size_t mip){
		// TODO
	}

	void Texture::setArrayLayers(const std::size_t layers){
		// TODO
	}

	void Texture::setFlags(const Flags& flags){
		// TODO
	}

	const Format& Texture::getFormat() const noexcept{
		// TODO
	}

	const std::size_t& Texture::getWidth() const noexcept{
		// TODO
	}

	const std::size_t& Texture::getHeight() const noexcept{
		// TODO
	}

	const std::size_t& Texture::getDepth() const noexcept{
		// TODO
	}

	const Texture::Usage& Texture::getUsage() const noexcept{
		// TODO
	}

	const Texture::Layout& Texture::getLayout() const noexcept{
		// TODO
	}

	const Texture::Tiling& Texture::getTiling() const noexcept{
		// TODO
	}

	const Texture::Type& Texture::getType() const noexcept{
		// TODO
	}

	const std::size_t& Texture::getMipmapCount() const noexcept{
		// TODO
	}

	const std::size_t& Texture::getArrayLayers() const noexcept{
		// TODO
	}

	const Texture::Flags& Texture::getFlags() const noexcept{
		// TODO
	}

	bool Texture::isInitialized() const noexcept{
		// TODO
	}

	void* Texture::getNativeHandle() const noexcept{
		// TODO
	}

	Texture::Handle Texture::getHandle() const noexcept{
		return _handle;
	}

	GUID Texture::getGUID() const noexcept{
		
	}
}