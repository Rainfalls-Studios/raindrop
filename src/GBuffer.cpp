#include <Raindrop/GBuffer.hpp>
#include <Raindrop_internal/GBuffer.hpp>
#include <Raindrop_internal/GMemory.hpp>
#include <Raindrop_internal/Graphics/Engine.hpp>
#include <Raindrop/GUID.hpp>

namespace Raindrop{
	GBuffer GBuffer::Create(Context& context){
		return GBuffer(context);
	}

	GBuffer::GBuffer(Context& context){
		_impl = std::make_unique<Impl>();
		_impl->context = context.getInternalContext();
		_impl->internal = std::make_shared<Internal::Graphics::Buffer>(context.getInternalContext()->getEngine().getContext());
	}

	GBuffer::~GBuffer(){
		_impl.reset();
	}

	bool GBuffer::isInitialized() const noexcept{
		return _impl != nullptr;
	}

	void* GBuffer::getNativeHandle() const{
		return _impl->internal != nullptr ? static_cast<void*>(_impl->internal->get()) : nullptr;
	}

	GBuffer::Impl* GBuffer::getImpl() const noexcept{
		return _impl.get();
	}

	GUID GBuffer::getGUID() const noexcept{

	}

	void GBuffer::allocate(const std::size_t& size, const Usage& usage, const Flags& flags, const GMemory::Type::Flags& memoryFlags, const std::size_t& alignement){
		_impl->internal->allocate(
			static_cast<VkDeviceSize>(size),
			GBufferUsageFlagsToVulkan(usage),
			GBufferFlagsToVulkan(flags),
			GMemoryTypeFlagsToVulkan(memoryFlags),
			static_cast<VkDeviceSize>(alignement)
		);

		_impl->size = size;
		_impl->usage = usage;
		_impl->flags = flags;
		_impl->memoryFlags = memoryFlags;
		_impl->alignement = alignement;
	}

	void GBuffer::free(){
		_impl->internal->free();

		_impl->size = 0;
		_impl->usage = Usage::NONE;
		_impl->flags = Flags::NONE;
		_impl->memoryFlags = GMemory::Type::Flags::NONE;
		_impl->alignement = 0;

		_impl->mappedSize = 0;
		_impl->mappedOffset = 0;
		_impl->mappedPointer = nullptr;
	}

	void GBuffer::map(const std::size_t& size, const std::size_t& offset){
		_impl->internal->map(size == WHOLE_SIZE ? VK_WHOLE_SIZE : static_cast<VkDeviceSize>(size), static_cast<VkDeviceSize>(offset));

		_impl->mappedSize = size;
		_impl->mappedOffset = offset;
		_impl->mappedPointer = _impl->internal->mapped();
	}

	void GBuffer::unmap(){
		_impl->internal->unmap();

		_impl->mappedSize = 0;
		_impl->mappedOffset = 0;
		_impl->mappedPointer = nullptr;
	}
	
	bool GBuffer::isMapped() const noexcept{
		return _impl->mappedPointer != nullptr;
	}

	void* GBuffer::getMappedPointer() noexcept{
		return _impl->mappedPointer;
	}

	const std::size_t& GBuffer::mappedSize() const noexcept{
		return _impl->mappedSize;
	}

	const std::size_t& GBuffer::mappedOffset() const noexcept{
		return _impl->mappedOffset;
	}

	void GBuffer::flush(const std::size_t& size, const std::size_t& offset){
		_impl->internal->flush(size == WHOLE_SIZE ? VK_WHOLE_SIZE : static_cast<VkDeviceSize>(size), static_cast<VkDeviceSize>(offset));
	}

	void GBuffer::invalidate(const std::size_t& size, const std::size_t& offset){
		_impl->internal->invalidate(size == WHOLE_SIZE ? VK_WHOLE_SIZE : static_cast<VkDeviceSize>(size), static_cast<VkDeviceSize>(offset));
	}

	const std::size_t& GBuffer::getSize() const noexcept{
		return _impl->size;
	}

	const std::size_t& GBuffer::getAllignement() const noexcept{
		return _impl->alignement;
	}

	const GBuffer::Usage& GBuffer::getUsage() const noexcept{
		return _impl->usage;
	}

	const GBuffer::Flags& GBuffer::getFlags() const noexcept{
		return _impl->flags;
	}

	const GMemory::Type::Flags& GBuffer::getMemoryFlags() const noexcept{
		return _impl->memoryFlags;
	}
}