#include "Raindrop/Graphics/Window.hpp"
#include "Raindrop/Graphics/Context.hpp"

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