#include <Raindrop/Renderer/Core/Window.hpp>
#include <Raindrop/Renderer/Context.hpp>

#include <SDL3/SDL_vulkan.h>
#include <spdlog/spdlog.h>

namespace Raindrop::Renderer::Core{
	static constexpr uint32_t DEFAULT_WINDOW_WIDTH = 1080;
	static constexpr uint32_t DEFAULT_WINDOW_HEIGHT = 720;

	Window::Window(Context& context) : _context{context}{
		spdlog::info("Constructing window ...");
		spdlog::info("Initializing SDL2 API...");
		if (SDL_Init(SDL_INIT_VIDEO) != 0){
			spdlog::error("Failed to initialize SDL2 API : {}", SDL_GetError());
			throw std::runtime_error("Failed initialize SDL2 API");
		}
		spdlog::info("SDL2 API initialized successfully");

		spdlog::info("Initializing SDL2 Window...");
		_window = SDL_CreateWindow("Raindrop::Graphics window", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
		if (!_window){
			spdlog::error("Failed to initialize SDL2 Window : {}", SDL_GetError());
			throw std::runtime_error("Failed to create SDL2 Window");
		}
		spdlog::info("SDL2 Window initialized successfully");
	}

	Window::~Window(){
		spdlog::info("Destructing window ...");
		if (_window){
			spdlog::info("Termintating SDL2 Window...");
			SDL_DestroyWindow(_window);
		}

		spdlog::info("Terminating SDL2 API...");
		SDL_Quit();
	}

	std::vector<const char*> Window::vulkanExtensions(){
		uint32_t extensionCount = 0;
		const auto& exts = SDL_Vulkan_GetInstanceExtensions(&extensionCount);
		if (exts == nullptr){
			throw std::runtime_error("Failed to querry SDL2 vulkan instance extensions");
		}

		std::vector<const char*> extensions(&exts[0], &exts[extensionCount]);
		
		return extensions;
	}

	void Window::createSurface(){
		spdlog::debug("Creating SDL2 Vulkan surface");
		if (SDL_Vulkan_CreateSurface(_window, _context.instance.get(), _context.allocationCallbacks, &_surface) == SDL_FALSE){
			spdlog::error("Failed to create SDL2 Vulkan surface : {}", SDL_GetError());
			throw std::runtime_error("Failed to create SDL2 Vulkan surface");
		}
		spdlog::debug("SDL2 Vulkan surface created without any critical error");
	}

	void Window::destroySurface(){
		if (_surface){
			spdlog::debug("Destroying SDL2 Vulkan surface");
			vkDestroySurfaceKHR(_context.instance.get(), _surface, _context.allocationCallbacks);
			spdlog::debug("SDL2 Vulkan surface destroyed without any critical error");
		}
	}

	void Window::setTitle(const char* title){
		SDL_SetWindowTitle(_window, title);
	}

	void Window::setSize(glm::u32vec2 size){
		SDL_SetWindowSize(_window, static_cast<int>(size.x), static_cast<int>(size.y));
	}

	void Window::setPosition(glm::u32vec2 position){
		SDL_SetWindowPosition(_window, static_cast<int>(position.x), static_cast<int>(position.y));
	}

	const char* Window::getTitle() const{
		return SDL_GetWindowTitle(_window);
	}

	glm::u32vec2 Window::getSize() const{
		int w, h;
		SDL_GetWindowSize(_window, &w, &h);
		return glm::u32vec2(w, h);
	}

	glm::u32vec2 Window::getPosition() const{
		int x, y;
		SDL_GetWindowPosition(_window, &x, &y);
		return glm::u32vec2(x, y);
	}

	SDL_Window* Window::get() const{
		return _window;
	}

	bool Window::loaded() const{
		return _window != nullptr;
	}

	void Window::events(){
		SDL_Event e;

		while (SDL_PollEvent(&e)){
			switch (e.type){
				case SDL_EVENT_QUIT: break;

				case SDL_EVENT_TERMINATING: break;
				case SDL_EVENT_LOW_MEMORY: break;
				case SDL_EVENT_WILL_ENTER_BACKGROUND: break;
				case SDL_EVENT_DID_ENTER_BACKGROUND: break;
				case SDL_EVENT_WILL_ENTER_FOREGROUND: break;
				case SDL_EVENT_DID_ENTER_FOREGROUND: break;

				case SDL_EVENT_LOCALE_CHANGED: break;
				case SDL_EVENT_SYSTEM_THEME_CHANGED: break;
				
				case SDL_EVENT_DISPLAY_ORIENTATION: break;
				case SDL_EVENT_DISPLAY_ADDED: break;
				case SDL_EVENT_DISPLAY_REMOVED: break;
				case SDL_EVENT_DISPLAY_MOVED: break;
				case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED: break;
				case SDL_EVENT_DISPLAY_HDR_STATE_CHANGED: break;

				case SDL_EVENT_WINDOW_SHOWN: break;
				case SDL_EVENT_WINDOW_HIDDEN: break;
				case SDL_EVENT_WINDOW_EXPOSED: break;
				case SDL_EVENT_WINDOW_MOVED: break;
				case SDL_EVENT_WINDOW_RESIZED: break;
				case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: break;
				case SDL_EVENT_WINDOW_MINIMIZED: break;
				case SDL_EVENT_WINDOW_MAXIMIZED: break;
				case SDL_EVENT_WINDOW_RESTORED: break;
				case SDL_EVENT_WINDOW_MOUSE_ENTER: break;
				case SDL_EVENT_WINDOW_MOUSE_LEAVE: break;
				case SDL_EVENT_WINDOW_FOCUS_GAINED: break;
				case SDL_EVENT_WINDOW_FOCUS_LOST: break;
				case SDL_EVENT_WINDOW_CLOSE_REQUESTED: break;
				case SDL_EVENT_WINDOW_TAKE_FOCUS: break;
				case SDL_EVENT_WINDOW_HIT_TEST: break;
				case SDL_EVENT_WINDOW_ICCPROF_CHANGED: break;
				case SDL_EVENT_WINDOW_DISPLAY_CHANGED: break;
				case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED: break;
				case SDL_EVENT_WINDOW_OCCLUDED: break;
				case SDL_EVENT_WINDOW_ENTER_FULLSCREEN: break;
				case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN: break;
				case SDL_EVENT_WINDOW_DESTROYED: break;
				case SDL_EVENT_WINDOW_PEN_ENTER: break;
				case SDL_EVENT_WINDOW_PEN_LEAVE: break;

				case SDL_EVENT_KEY_DOWN: break;
				case SDL_EVENT_KEY_UP: break;
				case SDL_EVENT_TEXT_EDITING: break;
				case SDL_EVENT_TEXT_INPUT: break;
				case SDL_EVENT_KEYMAP_CHANGED: break;

				case SDL_EVENT_MOUSE_MOTION: break;
				case SDL_EVENT_MOUSE_BUTTON_DOWN: break;
				case SDL_EVENT_MOUSE_BUTTON_UP: break;
				case SDL_EVENT_MOUSE_WHEEL: break;

				case SDL_EVENT_JOYSTICK_AXIS_MOTION: break;
				case SDL_EVENT_JOYSTICK_HAT_MOTION: break;
				case SDL_EVENT_JOYSTICK_BUTTON_DOWN: break;
				case SDL_EVENT_JOYSTICK_BUTTON_UP: break;
				case SDL_EVENT_JOYSTICK_ADDED: break;
				case SDL_EVENT_JOYSTICK_REMOVED: break;
				case SDL_EVENT_JOYSTICK_BATTERY_UPDATED: break;
				case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE: break;

				case SDL_EVENT_GAMEPAD_AXIS_MOTION: break;
				case SDL_EVENT_GAMEPAD_BUTTON_DOWN: break;
				case SDL_EVENT_GAMEPAD_BUTTON_UP: break;
				case SDL_EVENT_GAMEPAD_ADDED: break;
				case SDL_EVENT_GAMEPAD_REMOVED: break;
				case SDL_EVENT_GAMEPAD_REMAPPED: break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN: break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION: break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_UP: break;
				case SDL_EVENT_GAMEPAD_SENSOR_UPDATE: break;
				case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE: break;
				case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED: break;

				case SDL_EVENT_FINGER_DOWN: break;
				case SDL_EVENT_FINGER_UP: break;
				case SDL_EVENT_FINGER_MOTION: break;
			
				case SDL_EVENT_CLIPBOARD_UPDATE: break;

				case SDL_EVENT_DROP_FILE: break;
				case SDL_EVENT_DROP_TEXT: break;
				case SDL_EVENT_DROP_BEGIN: break;
				case SDL_EVENT_DROP_COMPLETE: break;
				case SDL_EVENT_DROP_POSITION: break;

				case SDL_EVENT_AUDIO_DEVICE_ADDED: break;
				case SDL_EVENT_AUDIO_DEVICE_REMOVED: break;
				case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED: break;

				case SDL_EVENT_SENSOR_UPDATE: break;

				case SDL_EVENT_PEN_DOWN: break;
				case SDL_EVENT_PEN_UP: break;
				case SDL_EVENT_PEN_MOTION: break;
				case SDL_EVENT_PEN_BUTTON_DOWN: break;
				case SDL_EVENT_PEN_BUTTON_UP: break;

				case SDL_EVENT_CAMERA_DEVICE_ADDED: break;
				case SDL_EVENT_CAMERA_DEVICE_REMOVED: break;
				case SDL_EVENT_CAMERA_DEVICE_APPROVED: break;
				case SDL_EVENT_CAMERA_DEVICE_DENIED: break;
				
				case SDL_EVENT_RENDER_TARGETS_RESET: break;
				case SDL_EVENT_RENDER_DEVICE_RESET: break;
			}
		}
	}

	// void quitEvent();


	VkSurfaceKHR Window::surface(){
		return _surface;
	}
}