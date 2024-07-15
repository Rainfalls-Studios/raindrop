#include <Raindrop/Graphics/Window/Surface.hpp>
#include <Raindrop/Graphics/Window/Context.hpp>
#include <Raindrop/Graphics/Core/Context.hpp>

#include <SDL3/SDL_vulkan.h>

namespace Raindrop::Graphics::Window{
	Surface::Surface() noexcept : 
		_context{nullptr},
		_core{nullptr},
		_surface{VK_NULL_HANDLE}
	{}

	Surface::~Surface(){
		release();
	}

	void Surface::prepare(Context& context, Core::Context& core){
		_context = &context;
		_core = &core;
	}

	void Surface::initialize(){
		_context->logger->info("Creating SDL vulkan surface...");
		if (SDL_Vulkan_CreateSurface(_context->window.get(), _core->instance.get(), _core->allocationCallbacks, &_surface) == VK_FALSE){
			_context->logger->error("Failed to create SDL vulkan surface : {}", SDL_GetError());
			throw std::runtime_error("Failed to create SDL vulkan surface");
		}
	}

	void Surface::release(){
		if (_surface){
			_context->logger->info("Destroying SDL vulkan surface...");

			vkDestroySurfaceKHR(_core->instance.get(), _surface, _core->allocationCallbacks);
			_surface = VK_NULL_HANDLE;
		}

		_core = nullptr;
		_context = nullptr;
	}

	const VkSurfaceKHR& Surface::get() const noexcept{
		return _surface;
	}
}