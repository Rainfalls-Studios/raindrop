#include <Raindrop/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop/Exceptions/MemoryExceptions.hpp>

namespace Raindrop{
	Context Context::Create(){
		return Context();
	}

	Context::Context() : _handle{nullptr}{}
	Context::~Context(){
		if (_handle){
			release();
		}
	}

	void Context::initialize(){
		spdlog::info("Initializing Context...");
		
		try{
			_handle = new Internal::Context;
		} catch (const std::bad_alloc& e){
			spdlog::error("Failed to intialize Context : memory allocation failure | {}", e.what());
			throw;
		} catch (const std::exception& e){
			spdlog::error("Failed to initilze Context : {}", e.what());
			throw;
		}

		spdlog::info("Successfuly initialized Context !");
	}
	
	void Context::release(){
		spdlog::info("Releasing Context...");

		if (_handle){
			delete static_cast<Internal::Context*>(_handle);
			_handle = nullptr;
		} else {
        	spdlog::warn("Attempted to release an already released Context");
			throw Exceptions::DoubleFreeException("Context");
		}

		spdlog::info("Successfuly released Context !");
	}

	bool Context::isInitialized() const noexcept{
		return _handle != nullptr;
	}

	Context::Handle Context::getHandle() const noexcept{
		return _handle;
	}

	Internal::Context* Context::getInternalContext(){
		return static_cast<Internal::Context*>(_handle);
	}

	void Context::start(){
		if (_handle != nullptr){
			getInternalContext()->start();
		} else{
			spdlog::error("Attemped to start a non initialized engine context");
			throw std::runtime_error("Cannot start a non initialized context");
		}
	}

	void Context::stop(){
		if (_handle != nullptr){
			getInternalContext()->stop();
		} else{
			spdlog::error("Attemped to start a non initialized engine context");
			throw std::runtime_error("Cannot start a non initialized context");
		}
	}

}