#include <Raindrop/GBuffer.hpp>
#include <Raindrop_internal/GBuffer.hpp>
#include <Raindrop_internal/GMemory.hpp>
#include <Raindrop_internal/Graphics/Renderer.hpp>
#include <Raindrop/GUID.hpp>

namespace Raindrop{
	GBuffer GBuffer::Create(Context& context){
		return GBuffer(context);
	}

	GBuffer::GBuffer(Context& context){
		_impl = std::make_unique<Impl>();
		_impl->buffer = std::make_shared<Internal::Graphics::Buffer>(context.getInternalContext()->getRenderer().getContext());
		_impl->context = &context;
	}

	GBuffer::~GBuffer(){
		_impl.reset();
	}

	bool GBuffer::isInitialized() const noexcept{
		return _impl != nullptr;
	}

	void* GBuffer::getNativeHandle() const{
		return static_cast<void*>(_impl->buffer->get());
	}

	GBuffer::Impl* GBuffer::getImpl() const noexcept{
		return _impl.get();
	}

	GUID GBuffer::getGUID() const noexcept{

	}

	void GBuffer::allocate(const std::size_t& size, const Usage& usage, const Flags& flags, const GMemory::Type::Flags& memoryFlags, const std::size_t& alignement){
		_impl->buffer->allocate(
			static_cast<VkDeviceSize>(size),
			GBufferUsageFlagsToVulkan(usage),
			GBufferFlagsToVulkan(flags),
			GMemoryTypeFlagsToVulkan(memoryFlags),
			static_cast<VkDeviceSize>(alignement)
		);
	}

	void GBuffer::free(){
		_impl->buffer->free();
	}

	void GBuffer::map(const std::size_t& size, const std::size_t& offset){
		_impl->buffer->map(size == WHOLE_SIZE ? VK_WHOLE_SIZE : static_cast<VkDeviceSize>(size), static_cast<VkDeviceSize>(offset));
	}

	void GBuffer::unmap(){
		return _impl->buffer->unmap();
	}

	void* GBuffer::getMappedPointer(){
		return _impl->buffer->mapped();
	}

	void GBuffer::flush(const std::size_t& size, const std::size_t& offset){
		_impl->buffer->flush(size == WHOLE_SIZE ? VK_WHOLE_SIZE : static_cast<VkDeviceSize>(size), static_cast<VkDeviceSize>(offset));
	}

	void GBuffer::invalidate(const std::size_t& size, const std::size_t& offset){
		_impl->buffer->invalidate(size == WHOLE_SIZE ? VK_WHOLE_SIZE : static_cast<VkDeviceSize>(size), static_cast<VkDeviceSize>(offset));
	}

	const std::size_t& GBuffer::getSize() const noexcept{
		return _impl->buffer->getSize();
	}

	const std::size_t& GBuffer::getAllignement() const noexcept{
		return _impl->buffer->getMinAllignement();
	}

	GBuffer::Usage GBuffer::getUsage() const noexcept{
		return GBufferUsageFlagsToRaindrop(_impl->buffer->getUsageFlags());
	}

	GBuffer::Flags GBuffer::getFlags() const noexcept{
		return GBufferFlagsToRaindrop(_impl->buffer->getCreateFlags());
	}

	GMemory::Type::Flags GBuffer::getMemoryFlags() const noexcept{
		return GMemoryTypeFlagsToRaindrop(_impl->buffer->getMemoryPropertyFlags());
	}
}