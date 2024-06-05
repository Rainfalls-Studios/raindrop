#include <Raindrop/GMemory.hpp>
#include <Raindrop_internal/GMemory.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Graphics/Renderer.hpp>
#include <Raindrop/GUID.hpp>

#define GRAPHICS_CONTEXT context.getInternalContext()->getRenderer().getContext()

namespace Raindrop{
	VkMemoryPropertyFlags toVulkan(const GMemory::Type::Flags& flags){
		VkMemoryPropertyFlags out = 0;

		if (flags.has(GMemory::Type::Flags::GPU_LOCAL)) out |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		if (flags.has(GMemory::Type::Flags::CPU_VISIBLE)) out |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		if (flags.has(GMemory::Type::Flags::CPU_COHERENT)) out |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		if (flags.has(GMemory::Type::Flags::CPU_CACHED)) out |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
		if (flags.has(GMemory::Type::Flags::LAZY_ALLOCATION)) out |= VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
		if (flags.has(GMemory::Type::Flags::PROTECTED)) out |= VK_MEMORY_PROPERTY_PROTECTED_BIT;

		// VK_AMD_device_coherent_memory
		if (flags.has(GMemory::Type::Flags::GPU_COHERENT)) out |= VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD;
		if (flags.has(GMemory::Type::Flags::GPU_UNCACHED)) out |= VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD;

		return out;
	}


	//--------------------------------------------------------------------
	//-----------------         GMEMORY TYPE             -----------------
	//--------------------------------------------------------------------


	Internal::Graphics::MemoryType& GMemory::Type::Impl::get(){
		return context->getMemoryTypes().get(index);
	}

	const Internal::Graphics::MemoryType& GMemory::Type::Impl::get() const{
		return context->getMemoryTypes().get(index);
	}



	GMemory::Type GMemory::Type::Find(Context& context, const Flags& required){
		auto& type = GRAPHICS_CONTEXT.getMemoryTypes().findBest(toVulkan(required));
		return Type(context, type.getIndex());
	}

	GMemory::Type::Type(Context& context, const std::size_t& index){
		_impl = new Impl();

		_impl->context = &GRAPHICS_CONTEXT;
		_impl->index = index;
	}

	GMemory::Type::Type(const Type& other){
		_impl = new Impl();
		*_impl = *other._impl;
	}

	GMemory::Type::Type(Type&& other){
		_impl = other._impl;
		other._impl = nullptr;
	}

	GMemory::Type::~Type(){
		if (_impl != nullptr){
			delete _impl;
			_impl = nullptr;
		}
	}

	GMemory::Type& GMemory::Type::operator=(const Type& other){
		if (other._impl != _impl){

			// If everything is valid
			if (_impl && other._impl){
				*_impl = *other._impl;

			// If the destination implementation pointer is null
			} else if (!_impl){
				_impl = new Impl();
				*_impl = *other._impl;

			// If the source implementation pointer is null
			} else {
				delete _impl;
				_impl = nullptr;
			}
		}

		return *this;
	}

	GMemory::Type& GMemory::Type::operator=(Type&& other){
		if (_impl != other._impl){
			if (_impl){
				delete _impl;
				_impl = nullptr;
			}

			_impl = other._impl;
			other._impl = nullptr;
		}

		return *this;
	}

	bool GMemory::Type::isInitialized() const noexcept{
		return _impl != nullptr;
	}

	GMemory::Type::Impl* GMemory::Type::getImpl() const noexcept{
		return _impl;
	}

	const std::size_t& GMemory::Type::getIndex() const noexcept{
		return _impl->index;
	}

	GMemory::Type::Flags GMemory::Type::getFlags() const noexcept{
		return _impl->get().getFlags();
	}


	//--------------------------------------------------------------------
	//-----------------           GMEMORY                -----------------
	//--------------------------------------------------------------------


	GMemory GMemory::Allocate(Context& context, const std::size_t& size, const Type& memoryType){
		GMemory memory(context);
		memory.allocate(size, memoryType);
		
		return std::move(memory);
	}

	GMemory::GMemory(Context& context){
		_impl = new Impl();
		_impl->context = &context;
	}

	GMemory::GMemory(const GMemory& other){
		_impl = new Impl();
		*_impl = *other._impl;
	}

	GMemory::GMemory(GMemory&& other){
		_impl = other._impl;
		other._impl = nullptr;
	}

	GMemory& GMemory::operator=(const GMemory& other){
		if (other._impl != _impl){

			// If everything is valid
			if (_impl && other._impl){
				*_impl = *other._impl;

			// If the destination implementation pointer is null
			} else if (!_impl){
				_impl = new Impl();
				*_impl = *other._impl;

			// If the source implementation pointer is null
			} else {
				delete _impl;
				_impl = nullptr;
			}
		}

		return *this;
	}

	GMemory& GMemory::operator=(GMemory&& other){
		if (_impl != other._impl){
			if (_impl){
				delete _impl;
				_impl = nullptr;
			}

			_impl = other._impl;
			other._impl = nullptr;
		}

		return *this;
	}

	GMemory::~GMemory(){
		if (_impl){
			delete _impl;
			_impl = nullptr;
		}
	}

	bool GMemory::isInitialized() const noexcept{
		return _impl != nullptr;
	}

	void* GMemory::getNativeHandle() const{
		if (_impl->memory){
			return static_cast<void*>(_impl->memory->get());
		}

		return nullptr;
	}

	GMemory::Impl* GMemory::getImpl() const noexcept{
		return _impl;
	}

	GUID GMemory::getGUID() const noexcept{

	}

	void GMemory::allocate(const std::size_t& size, const Type& memoryType){
		_impl->memory = std::make_shared<Internal::Graphics::Memory>(
			_impl->context->getInternalContext()->getRenderer().getContext(),
			memoryType.getImpl()->get(),
			size
		);
	}

	void GMemory::free(){
		_impl->memory.reset();
	}
}