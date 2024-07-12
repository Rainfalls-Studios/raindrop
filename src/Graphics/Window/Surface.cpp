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

	void Surface::initialize(Context& context, Core::Context& core){
		_context = &context;
		_core = &core;

		if (SDL_Vulkan_CreateSurface(_context->window.get(), _core->instance.get(), _core->allocationCallbacks, &_surface) != VK_TRUE){
			_context->logger->error("Failed to create SDL Vulkan surface : {}", SDL_GetError());
			throw std::runtime_error("Failed to create SDL Vulkan surface");
		}
	}

	void Surface::release(){
		if (_core && _surface != VK_NULL_HANDLE){
			vkDestroySurfaceKHR(_core->instance.get(), _surface, _core->allocationCallbacks);
		}

		_context = nullptr;
		_core = nullptr;
		_surface = nullptr;
	}

	const VkSurfaceKHR& Surface::get() const noexcept{
		return _surface;
	}
}