#include <Raindrop/Graphics/Internal/Window.hpp>
#include <Raindrop/Graphics/Internal/Context.hpp>
#include <Raindrop/Core/Engine.hpp>

#include <SDL2/SDL_vulkan.h>

namespace Raindrop::Graphics::Internal{
	static constexpr uint32_t DEFAULT_WINDOW_WIDTH = 1080;
	static constexpr uint32_t DEFAULT_WINDOW_HEIGHT = 720;

	Window::Window(Context& context) : _context{context}{
		_context.logger().info("Initializing the SDL2 API...");
		if (SDL_Init(SDL_INIT_VIDEO) != 0){
			_context.logger().error("Failed to initialize the SDL2 API : %s", SDL_GetError());
			throw std::runtime_error("Failed initialize SDL2 API");
		}
		_context.logger().info("SDL2 API initialized without any critical error");

		_context.logger().info("Initializing SDL2 Window...");
		_window = SDL_CreateWindow("Raindrop::Graphics window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
		if (!_window){
			_context.logger().error("Failed to initialize SDL2 Window : %s", SDL_GetError());
			throw std::runtime_error("Failed to create SDL2 Window");
		}
		_context.logger().info("SDL2 Window initialized without any critical error");
	}

	Window::~Window(){
		if (_window){
			_context.logger().info("Termintating SDL2 Window...");
			SDL_DestroyWindow(_window);
			_context.logger().info("SDL2 Window terminated without any critical error");
		}

		_context.logger().info("Terminating SDL2 API...");
		SDL_Quit();
		_context.logger().info("SDL2 API terminated without any critical error");
	}

	std::vector<const char*> Window::vulkanExtensions(){
		uint32_t extensionCount = 0;
		if (!SDL_Vulkan_GetInstanceExtensions(_window, &extensionCount, nullptr)){
			throw std::runtime_error("failed to querry SDL2 vulkan instance extensions");
		}

		std::vector<const char*> extensions(extensionCount);
		SDL_Vulkan_GetInstanceExtensions(_window, &extensionCount, extensions.data());
		return extensions;
	}

	void Window::createSurface(){
		_context.logger().debug("Creating SDL2 Vulkan surface");
		if (SDL_Vulkan_CreateSurface(_window, _context.instance().get(), &_surface) == SDL_FALSE){
			_context.logger().error("Failed to create SDL2 Vulkan surface : %s", SDL_GetError());
			throw std::runtime_error("Failed to create SDL2 Vulkan surface");
		}
		_context.logger().debug("SDL2 Vulkan surface created without any critical error");
	}

	void Window::destroySurface(){
		if (_surface){
			_context.logger().debug("Destroying SDL2 Vulkan surface");
			vkDestroySurfaceKHR(_context.instance().get(), _surface, _context.graphics().allocationCallbacks());
			_context.logger().debug("SDL2 Vulkan surface destroyed without any critical error");
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
				case SDL_QUIT: quitEvent(); break;
				case SDL_WINDOWEVENT: windowEvent(e); break;
				case SDL_MOUSEMOTION: mouseMotionEvent(e); break;
				case SDL_KEYDOWN: keyDown(e); break;
				case SDL_KEYUP: keyUp(e); break;
				case SDL_MOUSEBUTTONDOWN: mouseDown(e); break;
				case SDL_MOUSEBUTTONUP: mouseUp(e); break;
			}
		}
	}

	void Window::windowEvent(SDL_Event& e){
		switch (e.window.event){
			case SDL_WINDOWEVENT_RESIZED: windowResizedEvent(e.window); break;
		}
	}

	void Window::quitEvent(){
		auto& ev = _context.graphics().core().eventManager();
		ev.trigger("Graphics.Internal.Window.Quit");
	}

	void Window::windowResizedEvent(SDL_WindowEvent& e){
		auto& ev = _context.graphics().core().eventManager();
		_resized = true;
		ev.trigger("Graphics.Internal.Window.Resized");
	}

	void Window::mouseMotionEvent(SDL_Event& e){
		auto& ev = _context.graphics().core().eventManager();
		ev.trigger("Graphics.Internal.Window.Mouse.Mouved");
		ev.mouseEvents().pos() = glm::vec2(e.motion.x, e.motion.y);
		ev.mouseEvents().relPos() = glm::vec2(e.motion.xrel, e.motion.yrel);
	}

	Core::Event::MouseButton SDLToRaindropMouseButton(Uint8 button){
		switch (button){
			case SDL_BUTTON_LEFT: return Core::Event::BUTTON_LEFT;
			case SDL_BUTTON_MIDDLE: return Core::Event::BUTTON_MIDDLE;
			case SDL_BUTTON_RIGHT: return Core::Event::BUTTON_RIGHT;
		}
		throw std::runtime_error("unknow button");
	}

	void Window::mouseDown(SDL_Event& e){
		auto& ev = _context.graphics().core().eventManager();
		ev.trigger("Graphics.Internal.Window.Mouse.ButtonDown");
		ev.mouseEvents().state(SDLToRaindropMouseButton(e.button.button)) = Core::Event::BUTTON_DOWN;
	}

	void Window::mouseUp(SDL_Event& e){
		auto& ev = _context.graphics().core().eventManager();
		ev.trigger("Graphics.Internal.Window.Mouse.ButtonUp");
		ev.mouseEvents().state(SDLToRaindropMouseButton(e.button.button)) = Core::Event::BUTTON_UP;
	}
	
	bool Window::resized() const{
		return _resized;
	}

	void Window::resetResizedFlag(){
		_resized = false;
	}

	void Window::keyDown(SDL_Event& e){
		auto& ev = _context.graphics().core().eventManager();
		ev.trigger("Graphics.Internal.Window.Keyboard.KeyDown");
		ev.keyEvents()[static_cast<Core::Event::Key>(e.key.keysym.scancode)] = Core::Event::KEY_PRESSED;
	}

	void Window::keyUp(SDL_Event& e){
		auto& ev = _context.graphics().core().eventManager();
		ev.trigger("Graphics.Internal.Window.Keyboard.KeyUp");
		ev.keyEvents()[static_cast<Core::Event::Key>(e.key.keysym.scancode)] = Core::Event::KEY_RELEASED;
	}

	VkSurfaceKHR Window::surface(){
		return _surface;
	}
}