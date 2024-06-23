#include <Raindrop/GMemory.hpp>
#include <Raindrop_internal/GMemory.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Graphics/Engine.hpp>
#include <Raindrop/GUID.hpp>

#define GRAPHICS_CONTEXT _impl->context->getEngine().getContext()

namespace Raindrop{
	//--------------------------------------------------------------------
	//-----------------         GMEMORY TYPE             -----------------
	//--------------------------------------------------------------------

	Internal::Graphics::MemoryType& GMemory::Type::Impl::get(){
		return context->getEngine().getContext().getMemoryTypes().get(index);
	}

	const Internal::Graphics::MemoryType& GMemory::Type::Impl::get() const{
		return context->getEngine().getContext().getMemoryTypes().get(index);
	}

	GMemory::Type GMemory::Type::Find(Context& context, const Flags& required){
		auto& type = context.getInternalContext()->getEngine().getContext().getMemoryTypes().findBest(toVulkan(static_cast<VkMemoryPropertyFlags>(required.get())));
		return Type(context, type.getIndex());
	}

	GMemory::Type::Type(Context& context, const std::size_t& index){
		_impl = std::make_unique<Impl>();

		_impl->context = context.getInternalContext();
		_impl->index = index;
	}

	bool GMemory::Type::isInitialized() const noexcept{
		return _impl != nullptr;
	}

	GMemory::Type::Impl* GMemory::Type::getImpl() const noexcept{
		return _impl.get();
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
		_impl = std::make_unique<Impl>();
		_impl->context = context.getInternalContext();
	}

	GMemory::~GMemory(){
		_impl.reset();
	}

	GMemory::GMemory(GMemory&& other) : _impl{std::move(other._impl)}{}
	GMemory& GMemory::operator=(GMemory&& other){
		if (this != &other){
			_impl = std::move(other._impl);
		}
		return *this;
	}



	bool GMemory::isInitialized() const noexcept{
		return _impl != nullptr;
	}

	void* GMemory::getNativeHandle() const{
		return _impl->internal != nullptr ? static_cast<void*>(_impl->internal.get()) : nullptr;
	}

	GMemory::Impl* GMemory::getImpl() const noexcept{
		return _impl.get();
	}

	GUID GMemory::getGUID() const noexcept{

	}

	void GMemory::allocate(const std::size_t& size, const Type& memoryType){
		_impl->internal = std::make_shared<Internal::Graphics::Memory>(
			_impl->context->getEngine().getContext(),
			memoryType.getImpl()->get(),
			size
		);

		_impl->size = size;
		_impl->typeIndex = memoryType.getIndex();
	}

	void GMemory::free(){
		_impl->internal.reset();
	}
}