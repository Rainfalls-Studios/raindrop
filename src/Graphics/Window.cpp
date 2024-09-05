#include "Raindrop/Graphics/Window.hpp"
#include "Raindrop/Graphics/Context.hpp"

#include <SDL3/SDL_vulkan.h>

namespace Raindrop::Graphics{
	Window::Window(Context& context, const char* title, std::uint32_t width, std::uint32_t height) : _context{context}{
		// We suppose that this function is only called once

		if (SDL_Init(SDL_INIT_VIDEO) != SDL_TRUE){
			SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to initialize SDL3 : {}", SDL_GetError());
			throw std::runtime_error("Failed to initialize SDL3");
		}

		_window = SDL_CreateWindow(title, width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
		if (!_window){
			SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to create SDL3 window : {}", SDL_GetError());
			throw std::runtime_error("Failed to create Window");
		}
	}
	
	Window::~Window(){
		if (!_window){
			SDL_DestroyWindow(_window);
			_window = nullptr;

			SDL_Quit();
		}
	}

	std::vector<const char*> Window::vkExtensions(){
		Uint32 count;
		const char *const *extensions = SDL_Vulkan_GetInstanceExtensions(&count);

		return std::vector<const char*>(extensions, extensions + count);
	}

	VkSurfaceKHR Window::createSurface(){
		VkSurfaceKHR surface;

		SPDLOG_LOGGER_INFO(_context.getLogger(), "Creating window vulkan surface...");
		if (SDL_Vulkan_CreateSurface(_window, _context.getInstance().get(), nullptr, &surface) != SDL_TRUE){
			SPDLOG_LOGGER_ERROR(_context.getLogger(), "Failed to create window vulkan surface : {}", SDL_GetError());
			throw std::runtime_error("Failed to create surface");
		}
		
		SPDLOG_LOGGER_INFO(_context.getLogger(), "Window vulkan surface created with success !");
		return surface;
	}


	Context& Window::getContext() noexcept{
		return _context;
	}

	const Context& Window::getContext() const noexcept{
		return _context;
	}

	SDL_Window* Window::get() const noexcept{
		return _window;
	}
}