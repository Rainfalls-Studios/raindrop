#include <Raindrop/Renderer/Core/Window.hpp>
#include <Raindrop/Renderer/Context.hpp>
#include <Raindrop/Context.hpp>

#include <SDL3/SDL_vulkan.h>
#include <spdlog/spdlog.h>

namespace Raindrop::Renderer::Core{
	static constexpr uint32_t DEFAULT_WINDOW_WIDTH = 1080;
	static constexpr uint32_t DEFAULT_WINDOW_HEIGHT = 720;

	Window::Window(Context& context) : _context{context}{
		spdlog::info("Constructing window ...");
		spdlog::info("Initializing SDL3 API...");
		if (SDL_Init(SDL_INIT_VIDEO) != 0){
			spdlog::error("Failed to initialize SDL3 API : {}", SDL_GetError());
			throw std::runtime_error("Failed initialize SDL3 API");
		}
		spdlog::info("SDL3 API initialized successfully");

		spdlog::info("Initializing SDL3 Window...");
		_window = SDL_CreateWindow("Raindrop::Graphics window", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
		if (!_window){
			spdlog::error("Failed to initialize SDL3 Window : {}", SDL_GetError());
			throw std::runtime_error("Failed to create SDL3 Window");
		}
		spdlog::info("SDL3 Window initialized successfully");
	}

	Window::~Window(){
		spdlog::info("Destructing window ...");
		if (_window){
			spdlog::info("Termintating SDL3 Window...");
			SDL_DestroyWindow(_window);
		}

		spdlog::info("Terminating SDL3 API...");
		SDL_Quit();
	}

	std::vector<const char*> Window::vulkanExtensions(){
		uint32_t extensionCount = 0;
		const auto& exts = SDL_Vulkan_GetInstanceExtensions(&extensionCount);
		if (exts == nullptr){
			throw std::runtime_error("Failed to querry SDL3 vulkan instance extensions");
		}

		std::vector<const char*> extensions(&exts[0], &exts[extensionCount]);
		
		return extensions;
	}

	void Window::createSurface(){
		spdlog::debug("Creating SDL3 Vulkan surface");
		if (SDL_Vulkan_CreateSurface(_window, _context.instance.get(), _context.allocationCallbacks, &_surface) == SDL_FALSE){
			spdlog::error("Failed to create SDL3 Vulkan surface : {}", SDL_GetError());
			throw std::runtime_error("Failed to create SDL3 Vulkan surface");
		}
		spdlog::debug("SDL3 Vulkan surface created without any critical error");
	}

	void Window::destroySurface(){
		if (_surface){
			spdlog::debug("Destroying SDL3 Vulkan surface");
			vkDestroySurfaceKHR(_context.instance.get(), _surface, _context.allocationCallbacks);
			spdlog::debug("SDL3 Vulkan surface destroyed without any critical error");
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

	VkSurfaceKHR Window::surface(){
		return _surface;
	}

	void Window::events(){
		SDL_Event e;

		while (SDL_PollEvent(&e)){
			switch (e.type){
				case SDL_EVENT_QUIT: quitEvent(e); break;

				case SDL_EVENT_TERMINATING: terminatingEvent(e); break;
				case SDL_EVENT_LOW_MEMORY: lowMemoryEvent(e); break;

				case SDL_EVENT_WILL_ENTER_BACKGROUND: willEnterBackgroundEvent(e); break;
				case SDL_EVENT_DID_ENTER_BACKGROUND: didEnterBackgroundEvent(e); break;
				case SDL_EVENT_WILL_ENTER_FOREGROUND: willEnterForgroundEvent(e); break;
				case SDL_EVENT_DID_ENTER_FOREGROUND: didEnterForgourndEvent(e); break;

				case SDL_EVENT_LOCALE_CHANGED: localeChangedEvent(e); break;
				case SDL_EVENT_SYSTEM_THEME_CHANGED: systemThemChangedEvent(e); break;
				
				case SDL_EVENT_DISPLAY_ORIENTATION: displayOrientationEvent(e); break;
				case SDL_EVENT_DISPLAY_ADDED: displayAddedEvent(e); break;
				case SDL_EVENT_DISPLAY_REMOVED: displayRemovedEvent(e); break;
				case SDL_EVENT_DISPLAY_MOVED: displayMovedEvent(e); break;
				case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED: displayContentScaleChangedEvent(e); break;
				case SDL_EVENT_DISPLAY_HDR_STATE_CHANGED: displayHDRStateChnagedEvent(e); break;

				case SDL_EVENT_WINDOW_SHOWN: windowShownEvent(e); break;
				case SDL_EVENT_WINDOW_HIDDEN: windowHiddenEvent(e); break;
				case SDL_EVENT_WINDOW_EXPOSED: windowExposedEvent(e); break;
				case SDL_EVENT_WINDOW_MOVED: windowMovedEvent(e); break;
				case SDL_EVENT_WINDOW_RESIZED: windowResizedEvent(e); break;
				case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: windowPixelSizeChangedEvent(e); break;
				case SDL_EVENT_WINDOW_MINIMIZED: windowMinimizedEvent(e); break;
				case SDL_EVENT_WINDOW_MAXIMIZED: windowMaximizedEvent(e); break;
				case SDL_EVENT_WINDOW_RESTORED: windowRestoredEvent(e); break;
				case SDL_EVENT_WINDOW_MOUSE_ENTER: windowMouseEnterEvent(e); break;
				case SDL_EVENT_WINDOW_MOUSE_LEAVE: windowMouseLeaveEvent(e); break;
				case SDL_EVENT_WINDOW_FOCUS_GAINED: windowFocusGainedEvent(e); break;
				case SDL_EVENT_WINDOW_FOCUS_LOST: windowFocusLostEvent(e); break;
				case SDL_EVENT_WINDOW_CLOSE_REQUESTED: windowCloseRequestedEvent(e); break;
				case SDL_EVENT_WINDOW_TAKE_FOCUS: windowTakeFocusEvent(e); break;
				case SDL_EVENT_WINDOW_HIT_TEST: windowHitTestEvent(e); break;
				case SDL_EVENT_WINDOW_ICCPROF_CHANGED: windowICCPROFChangedEvent(e); break;
				case SDL_EVENT_WINDOW_DISPLAY_CHANGED: windowDisplayChangedEvent(e); break;
				case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED: windowDisplayScaleChangedEvent(e); break;
				case SDL_EVENT_WINDOW_OCCLUDED: windowOccludedEvent(e); break;
				case SDL_EVENT_WINDOW_ENTER_FULLSCREEN: windowEnterFullsreenEvent(e); break;
				case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN: windowLeaveFullscreenEvent(e); break;
				case SDL_EVENT_WINDOW_DESTROYED: windowDestroyedEvent(e); break;
				case SDL_EVENT_WINDOW_PEN_ENTER: windowPenEnterEvent(e); break;
				case SDL_EVENT_WINDOW_PEN_LEAVE: windowPenLeaveEvent(e); break;

				case SDL_EVENT_KEY_DOWN: keyDownEvent(e); break;
				case SDL_EVENT_KEY_UP: keyUpEvent(e); break;
				case SDL_EVENT_TEXT_EDITING: textEditingEvent(e); break;
				case SDL_EVENT_TEXT_INPUT: textInputEvent(e); break;
				case SDL_EVENT_KEYMAP_CHANGED: keymapChangedEvent(e); break;

				case SDL_EVENT_MOUSE_MOTION: mouseMotionEvent(e); break;
				case SDL_EVENT_MOUSE_BUTTON_DOWN: mouseButtonDownEvent(e); break;
				case SDL_EVENT_MOUSE_BUTTON_UP: mouseButtonUpEvent(e); break;
				case SDL_EVENT_MOUSE_WHEEL: mouseWheelEvent(e); break;

				case SDL_EVENT_JOYSTICK_AXIS_MOTION: joystickAxisMotionEvent(e); break;
				case SDL_EVENT_JOYSTICK_HAT_MOTION: joystickHatMotionEvent(e); break;
				case SDL_EVENT_JOYSTICK_BUTTON_DOWN: joystickButtonDownEvent(e); break;
				case SDL_EVENT_JOYSTICK_BUTTON_UP: joystickButtonUpEvent(e); break;
				case SDL_EVENT_JOYSTICK_ADDED: joystickAddedEvent(e); break;
				case SDL_EVENT_JOYSTICK_REMOVED: joystickRemovedEvent(e); break;
				case SDL_EVENT_JOYSTICK_BATTERY_UPDATED: joystickBatteryUpdatedEvent(e); break;
				case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE: joystickUpdateCompletedEvent(e); break;

				case SDL_EVENT_GAMEPAD_AXIS_MOTION: gamepadAxisMotionEvent(e); break;
				case SDL_EVENT_GAMEPAD_BUTTON_DOWN: gamepadButtonDownEvent(e); break;
				case SDL_EVENT_GAMEPAD_BUTTON_UP: gamepadButtonUpEvent(e); break;
				case SDL_EVENT_GAMEPAD_ADDED: gamepadAddedEvent(e); break;
				case SDL_EVENT_GAMEPAD_REMOVED: gamepadRemovedEvent(e); break;
				case SDL_EVENT_GAMEPAD_REMAPPED: gamepadRemappedEvent(e); break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN: gamepadTouchpadDownEvent(e); break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION: gamepadTouchpadMotionEvent(e); break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_UP: gamepadTouchpadUpEvent(e); break;
				case SDL_EVENT_GAMEPAD_SENSOR_UPDATE: gamepadSensorUpdateEvent(e); break;
				case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE: gamepadUpdateCompleteEvent(e); break;
				case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED: gamepadSteamHandleUpdatedEvent(e); break;

				case SDL_EVENT_FINGER_DOWN: fingerDownEvent(e); break;
				case SDL_EVENT_FINGER_UP: fingerUpEvent(e); break;
				case SDL_EVENT_FINGER_MOTION: fingerMotionEvent(e); break;
			
				case SDL_EVENT_CLIPBOARD_UPDATE: clipboardUpdateEvent(e); break;

				case SDL_EVENT_DROP_FILE: dropFileEvent(e); break;
				case SDL_EVENT_DROP_TEXT: dropTextEvent(e); break;
				case SDL_EVENT_DROP_BEGIN: dropBeginEvent(e); break;
				case SDL_EVENT_DROP_COMPLETE: dropCompleteEvent(e); break;
				case SDL_EVENT_DROP_POSITION: dropPositionEvent(e); break;

				case SDL_EVENT_AUDIO_DEVICE_ADDED: audioDeviceAddedEvent(e); break;
				case SDL_EVENT_AUDIO_DEVICE_REMOVED: audioDeviceRemovedEvent(e); break;
				case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED: audioDeviceFormatChangedEvent(e); break;

				case SDL_EVENT_SENSOR_UPDATE: sensorUpdateEvent(e); break;

				case SDL_EVENT_PEN_DOWN: penDownEvent(e); break;
				case SDL_EVENT_PEN_UP: penUpEvent(e); break;
				case SDL_EVENT_PEN_MOTION: penMotionEvent(e); break;
				case SDL_EVENT_PEN_BUTTON_DOWN: penButtonDownEvent(e); break;
				case SDL_EVENT_PEN_BUTTON_UP: penButtonUpEvent(e); break;

				case SDL_EVENT_CAMERA_DEVICE_ADDED: cameraDeviceAddedEvent(e); break;
				case SDL_EVENT_CAMERA_DEVICE_REMOVED: cameraDeviceRemovedEvent(e); break;
				case SDL_EVENT_CAMERA_DEVICE_APPROVED: cameraDeviceApprovedEvent(e); break;
				case SDL_EVENT_CAMERA_DEVICE_DENIED: cameraDeviceDeniedEvent(e); break;
				
				case SDL_EVENT_RENDER_TARGETS_RESET: renderTargetsResetEvent(e); break;
				case SDL_EVENT_RENDER_DEVICE_RESET: renderDeviceResetEvent(e); break;
			}
		}
	}

	void Window::quitEvent(SDL_Event &e){
		_context.core.eventManager.trigger("window.quit");
	}
	
	void Window::terminatingEvent(SDL_Event& e){
		_context.core.eventManager.trigger("terminating");
	}

	void Window::lowMemoryEvent(SDL_Event& e){
		_context.core.eventManager.trigger("memory.low");
	}

	void Window::willEnterBackgroundEvent(SDL_Event& e){
		_context.core.eventManager.trigger("will_enter_background");
	}

	void Window::didEnterBackgroundEvent(SDL_Event& e){
		_context.core.eventManager.trigger("did_enter_background");
	}

	void Window::willEnterForgroundEvent(SDL_Event& e){
		_context.core.eventManager.trigger("will_enter_forground");
	}

	void Window::didEnterForgourndEvent(SDL_Event& e){
		_context.core.eventManager.trigger("did_enter_forground");
	}

	void Window::localeChangedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("locale.changed");
	}

	void Window::systemThemChangedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("system_theme.changed");
	}

	void Window::displayOrientationEvent(SDL_Event& e){
		_context.core.eventManager.trigger("display.orientation");
	}

	void Window::displayAddedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("display.added");
	}

	void Window::displayRemovedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("display.removed");
	}

	void Window::displayMovedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("display.moved");
	}

	void Window::displayContentScaleChangedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("display.content_scale.changed");
	}

	void Window::displayHDRStateChnagedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("display.HDR_state.changed");
	}

	void Window::windowShownEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.shown");
	}

	void Window::windowHiddenEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.hidden");
	}

	void Window::windowExposedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.exposed");
	}

	void Window::windowMovedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.moved");
	}

	void Window::windowResizedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.resized");
	}

	void Window::windowPixelSizeChangedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.pixel_size.changed");
	}

	void Window::windowMinimizedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.minimized");
	}

	void Window::windowMaximizedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.maximized");
	}

	void Window::windowRestoredEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.restored");
	}

	void Window::windowMouseEnterEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.mouse.enter");
	}

	void Window::windowMouseLeaveEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.mouse.leave");
	}

	void Window::windowFocusGainedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.focus.gained");
	}

	void Window::windowFocusLostEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.focus.lost");
	}

	void Window::windowCloseRequestedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.close.requested");
	}

	void Window::windowTakeFocusEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.focus.take");
	}

	void Window::windowHitTestEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.hit_test");
	}

	void Window::windowICCPROFChangedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.ICCPROF.changed");
	}

	void Window::windowDisplayChangedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.display.changed");
	}

	void Window::windowDisplayScaleChangedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.display.scale.changed");
	}

	void Window::windowOccludedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.occluded");
	}

	void Window::windowEnterFullsreenEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.fullscreen.enter");
	}

	void Window::windowLeaveFullscreenEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.fullscreen.leave");
	}

	void Window::windowDestroyedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.destroyed");
	}

	void Window::windowPenEnterEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.pen.enter");
	}

	void Window::windowPenLeaveEvent(SDL_Event& e){
		_context.core.eventManager.trigger("window.pen.leave");
	}

	void Window::keyDownEvent(SDL_Event& e){
		_context.core.eventManager.trigger("key.down");
	}

	void Window::keyUpEvent(SDL_Event& e){
		_context.core.eventManager.trigger("key.up");
	}

	void Window::textEditingEvent(SDL_Event& e){
		_context.core.eventManager.trigger("text.editing");
	}

	void Window::textInputEvent(SDL_Event& e){
		_context.core.eventManager.trigger("text.input");
	}

	void Window::keymapChangedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("keymap.changed");
	}

	void Window::mouseMotionEvent(SDL_Event& e){
		_context.core.eventManager.trigger("mouse.motion");
	}

	void Window::mouseButtonDownEvent(SDL_Event& e){
		_context.core.eventManager.trigger("mouse.button.down");
	}

	void Window::mouseButtonUpEvent(SDL_Event& e){
		_context.core.eventManager.trigger("mouse.button.up");
	}

	void Window::mouseWheelEvent(SDL_Event& e){
		_context.core.eventManager.trigger("mouse.wheel");
	}

	void Window::joystickAxisMotionEvent(SDL_Event& e){
		_context.core.eventManager.trigger("joystick.axis.motion");
	}

	void Window::joystickHatMotionEvent(SDL_Event& e){
		_context.core.eventManager.trigger("joystick.hat.motion");
	}

	void Window::joystickButtonDownEvent(SDL_Event& e){
		_context.core.eventManager.trigger("joystick.button.down");
	}

	void Window::joystickButtonUpEvent(SDL_Event& e){
		_context.core.eventManager.trigger("joystick.button.up");
	}

	void Window::joystickAddedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("joystick.added");
	}

	void Window::joystickRemovedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("joystick.removed");
	}

	void Window::joystickBatteryUpdatedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("joystick.battery.updated");
	}

	void Window::joystickUpdateCompletedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("joystick.update.complete");
	}

	void Window::gamepadAxisMotionEvent(SDL_Event& e){
		_context.core.eventManager.trigger("gamepad.axis.motion");
	}

	void Window::gamepadButtonDownEvent(SDL_Event& e){
		_context.core.eventManager.trigger("gamepad.button.down");
	}

	void Window::gamepadButtonUpEvent(SDL_Event& e){
		_context.core.eventManager.trigger("gamepad.button.up");
	}

	void Window::gamepadAddedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("gamepad.added");
	}

	void Window::gamepadRemovedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("gamepad.removed");
	}

	void Window::gamepadRemappedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("gamepad.remapped");
	}

	void Window::gamepadTouchpadDownEvent(SDL_Event& e){
		_context.core.eventManager.trigger("gamepad.touchpad.down");
	}
	
	void Window::gamepadTouchpadMotionEvent(SDL_Event& e){
		_context.core.eventManager.trigger("gamepad.touchpad.motion");
	}

	void Window::gamepadTouchpadUpEvent(SDL_Event& e){
		_context.core.eventManager.trigger("gamepad.touchpad.up");
	}

	void Window::gamepadSensorUpdateEvent(SDL_Event& e){
		_context.core.eventManager.trigger("gamepad.sensor.update");
	}

	void Window::gamepadUpdateCompleteEvent(SDL_Event& e){
		_context.core.eventManager.trigger("gamepad.update.complete");
	}

	void Window::gamepadSteamHandleUpdatedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("gamepad.steam_handle_updated");
	}

	void Window::fingerDownEvent(SDL_Event& e){
		_context.core.eventManager.trigger("finger.down");
	}

	void Window::fingerUpEvent(SDL_Event& e){
		_context.core.eventManager.trigger("finger.up");
	}

	void Window::fingerMotionEvent(SDL_Event& e){
		_context.core.eventManager.trigger("finger.motion");
	}

	void Window::clipboardUpdateEvent(SDL_Event& e){
		_context.core.eventManager.trigger("clipboard.update");
	}

	void Window::dropFileEvent(SDL_Event& e){
		_context.core.eventManager.trigger("drop.file");
	}

	void Window::dropTextEvent(SDL_Event& e){
		_context.core.eventManager.trigger("drop.text");
	}

	void Window::dropBeginEvent(SDL_Event& e){
		_context.core.eventManager.trigger("drop.beign");
	}

	void Window::dropCompleteEvent(SDL_Event& e){
		_context.core.eventManager.trigger("drop.complete");
	}

	void Window::dropPositionEvent(SDL_Event& e){
		_context.core.eventManager.trigger("drop.position");
	}

	void Window::audioDeviceAddedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("audio_device.added");
	}

	void Window::audioDeviceRemovedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("audio_device.removed");
	}

	void Window::audioDeviceFormatChangedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("audio_device.format_changed");
	}

	void Window::sensorUpdateEvent(SDL_Event& e){
		_context.core.eventManager.trigger("sensor.update");
	}

	void Window::penDownEvent(SDL_Event& e){
		_context.core.eventManager.trigger("pen.down");
	}

	void Window::penUpEvent(SDL_Event& e){
		_context.core.eventManager.trigger("pen.up");
	}

	void Window::penMotionEvent(SDL_Event& e){
		_context.core.eventManager.trigger("pen.motion");
	}

	void Window::penButtonDownEvent(SDL_Event& e){
		_context.core.eventManager.trigger("pen.button.down");
	}

	void Window::penButtonUpEvent(SDL_Event& e){
		_context.core.eventManager.trigger("pen.button.up");

	}

	void Window::cameraDeviceAddedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("camera_device.added");
	}

	void Window::cameraDeviceRemovedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("camera_device.removed");
	}

	void Window::cameraDeviceApprovedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("camera_device.approved");
	}

	void Window::cameraDeviceDeniedEvent(SDL_Event& e){
		_context.core.eventManager.trigger("camera_device.denied");
	}

	void Window::renderTargetsResetEvent(SDL_Event& e){
		_context.core.eventManager.trigger("render_targets.reset");
	}

	void Window::renderDeviceResetEvent(SDL_Event& e){
		_context.core.eventManager.trigger("render_device.reset");
	}
}