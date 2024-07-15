#include <Raindrop/Graphics/Window/Window.hpp>
#include <Raindrop/Graphics/Window/Context.hpp>

#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>
#include <SDL3/SDL_vulkan.h>

#define EVENT_INFO true
#define EVENT_DISPLAY true
#define EVENT_WINDOW true
#define EVENT_KEY true
#define EVENT_MOUSE true
#define EVENT_JOYSTICK false
#define EVENT_GAMEPAD false
#define EVENT_FINGER false
#define EVENT_CLIPBOARD false
#define EVENT_DROP false
#define EVENT_AUDIO false
#define EVENT_SENSOR false
#define EVENT_PEN false
#define EVENT_CAMERA false
#define EVENT_RENDER false

namespace Raindrop::Graphics::Window{
	static constexpr uint32_t DEFAULT_WINDOW_WIDTH = 1080;
	static constexpr uint32_t DEFAULT_WINDOW_HEIGHT = 720;

	Window::Window() noexcept :
		_context{nullptr},
		_window{nullptr}
	{}

	Window::~Window(){
		release();
	}

	void Window::prepare(Context& context){
		_context = &context;
	}

	void Window::initialize(){
		
		_context->logger->info("Constructing window ...");
		_context->logger->info("Initializing SDL3...");
		if (SDL_Init(SDL_INIT_VIDEO) != 0){
			_context->logger->error("Failed to initialize SDL3 : {}", SDL_GetError());
			throw std::runtime_error("Failed initialize SDL3");
		}
		_context->logger->info("SDL3 initialized successfully");

		_context->logger->info("Initializing SDL Window...");
		_window = SDL_CreateWindow("Raindrop", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

		if (!_window){
			_context->logger->error("Failed to initialize SDL Window : {}", SDL_GetError());
			throw std::runtime_error("Failed to create SDL Window");
		}

		// registerEvents();

		_context->logger->info("SDL3 Window created successfully");
	}

	void Window::release(){
		if (_window){
			SDL_DestroyWindow(_window);
		}

		if (SDL_WasInit(SDL_INIT_VIDEO)){
			SDL_Quit();
		}

		_window = nullptr;
		_context = nullptr;
	}

	SDL_Window* Window::get() const noexcept{
		return _window;
	}

	std::vector<const char*> Window::getRequiredInstanceExtensions() const{
		Uint32 count;
		const char* const* rawExtensions = SDL_Vulkan_GetInstanceExtensions(&count);

		if (rawExtensions == nullptr){
			_context->logger->error("Failed to query SDL Vulkan instance extensions : {}", SDL_GetError());
			throw std::runtime_error("Failed to query SDL Vulkan instance extensions");
		}

		return std::vector<const char*>(rawExtensions, rawExtensions + count);
	}

	glm::u32vec2 Window::getSize() const noexcept{
		int width, height;
		SDL_GetWindowSizeInPixels(_window, &width, &height);
		return glm::u32vec2(width, height);
	}


	VkExtent2D Window::getExtent() const noexcept{
		glm::u32vec2 size = getSize();
		return VkExtent2D{
			.width = size.x,
			.height = size.y
		};
	}


	// void Window::registerEvents(){
	// 	auto& event = _context.getInternalContext().getEventManager();

	// 	#if EVENT_INFO
	// 		event.create("quit");
	// 		event.create("terminating");
	// 		event.create("memory.low");
	// 		event.create("will_enter_background");
	// 		event.create("did_enter_background");
	// 		event.create("will_enter_forground");
	// 		event.create("did_enter_forground");
	// 		event.create("locale.changed");
	// 		event.create("system_theme.changed");
	// 	#endif

	// 	#if EVENT_DISPLAY
	// 		event.create<unsigned int>("display.orientation");
	// 		event.create("display.added");
	// 		event.create("display.removed");
	// 		event.create("display.moved");
	// 		event.create("display.content_scale.changed");
	// 		event.create("display.HDR_state.changed");
	// 	#endif

	// 	#if EVENT_WINDOW
	// 		event.create("window.shown");
	// 		event.create("window.hidden");
	// 		event.create("window.exposed");
	// 		event.create("window.moved");
	// 		event.create<Maths::ivec2>("window.resized");
	// 		event.create<Maths::ivec2>("window.pixel_size.changed");
	// 		event.create("window.minimized");
	// 		event.create("window.maximized");
	// 		event.create("window.restored");
	// 		event.create("window.mouse.enter");
	// 		event.create("window.mouse.leave");
	// 		event.create("window.focus.gained");
	// 		event.create("window.focus.lost");
	// 		event.create("window.close.requested");
	// 		event.create("window.focus.take");
	// 		event.create("window.hit_test");
	// 		event.create("window.ICCPROF.changed");
	// 		event.create("window.display.changed");
	// 		event.create("window.display.scale.changed");
	// 		event.create("window.occluded");
	// 		event.create("window.fullscreen.leave");
	// 		event.create("window.fullscreen.enter");
	// 		event.create("window.destroyed");
	// 		event.create("window.pen.enter");
	// 		event.create("window.pen.leave");
	// 	#endif

	// 	#if EVENT_KEY
	// 		using Key = ::Raindrop::Event::Key;
	// 		using KeyMod = ::Raindrop::Event::Keymod;

	// 		event.create<Key, Key, KeyMod>("key.down");
	// 		event.create<Key, Key, KeyMod>("key.up");
	// 		event.create<const char*, unsigned int, unsigned int>("text.editing");
	// 		event.create<const char*>("text.input");
	// 		event.create<const char*>("keymap.changed");

	// 		using Key = ::Raindrop::Event::Key;
	// 		using KeyState = ::Raindrop::Event::KeyState;
			
	// 		// event.subscribe(
	// 		// 	event.getEventID("key.down"),
	// 		// 	[&event](Key scancode, Key keysym, KeyMod modifier){
	// 		// 		event.keyEvents().state(scancode) = KeyState::PRESSED;
	// 		// 	}
	// 		// );

	// 		// event.subscribe(
	// 		// 	event.getEventID("key.up"),
	// 		// 	[&event](Key scancode, Key keysym, KeyMod modifier){
	// 		// 		event.keyEvents().state(scancode) = KeyState::RELEASED;
	// 		// 	}
	// 		// );

	// 	#endif
		
	// 	#if EVENT_MOUSE
	// 		event.create<Maths::vec2, Maths::vec2>("mouse.motion");
	// 		event.create<uint8_t, uint8_t, Maths::vec2>("mouse.button.down");
	// 		event.create<uint8_t, Maths::vec2>("mouse.button.up");
	// 		event.create<Maths::vec2, Maths::vec2>("mouse.wheel");

	// 		// event.subscribe(
	// 		// 	event.getEventID("mouse.motion"),
	// 		// 	[&event](Maths::vec2 position, Maths::vec2 relativePosition){
	// 		// 		event.mouseEvents().pos() = position;
	// 		// 		event.mouseEvents().relPos() = relativePosition;
	// 		// 	}
	// 		// );

	// 		// using Button = ::Raindrop::Event::MouseButton;
	// 		// using ButtonState = ::Raindrop::Event::ButtonState;

	// 		// event.subscribe(
	// 		// 	event.getEventID("mouse.button.down"),
	// 		// 	[&event](uint8_t button, uint8_t clicks, Maths::vec2 position){
	// 		// 		event.mouseEvents().state(static_cast<Button>(button)) = ButtonState::DOWN;
	// 		// 	}
	// 		// );

	// 		// event.subscribe(
	// 		// 	event.getEventID("mouse.button.up"),
	// 		// 	[&event](uint8_t button, Maths::vec2 position){
	// 		// 		event.mouseEvents().state(static_cast<Button>(button)) = ButtonState::UP;
	// 		// 	}
	// 		// );
	// 	#endif

	// 	#if EVENT_JOYSTICK
	// 		event.create<uint8_t, int16_t>("joystick.axis.motion");
	// 		event.create<uint8_t, int16_t>("joystick.hat.motion");
	// 		event.create<uint8_t>("joystick.button.down");
	// 		event.create<uint8_t>("joystick.button.up");
	// 		event.create("joystick.added");
	// 		event.create("joystick.removed");
	// 		event.create<SDL_JoystickPowerLevel>("joystick.battery.updated");
	// 		event.create("joystick.update.complete");
	// 	#endif

	// 	#if EVENT_GAMEPAD
	// 		event.create<uint8_t, int16_t>("gamepad.axis.motion");
	// 		event.create<uint8_t>("gamepad.button.down");
	// 		event.create<uint8_t>("gamepad.button.up");
	// 		event.create("gamepad.added");
	// 		event.create("gamepad.removed");
	// 		event.create("gamepad.remapped");
	// 		event.create<Maths::vec2, float>("gamepad.touchpad.down");
	// 		event.create<Maths::vec2, float>("gamepad.touchpad.motion");
	// 		event.create<Maths::vec2, float>("gamepad.touchpad.up");
	// 		event.create<uint64_t, Maths::vec3>("gamepad.sensor.update");
	// 		event.create("gamepad.update.complete");
	// 		event.create("gamepad.steam_handle_updated");
	// 	#endif

	// 	#if EVENT_FINGER
	// 		event.create<SDL_FingerID, Maths::vec2>("finger.down");
	// 		event.create<SDL_FingerID, Maths::vec2>("finger.up");
	// 		event.create<SDL_FingerID, Maths::vec2, Maths::vec2>("finger.motion");
	// 	#endif

	// 	#if EVENT_CLIPBOARD
	// 		event.create("clipboard.update");
	// 	#endif

	// 	#if EVENT_DROP
	// 		event.create("drop.file");
	// 		event.create("drop.text");
	// 		event.create("drop.begin");
	// 		event.create("drop.complete");
	// 		event.create("drop.position");
	// 	#endif

	// 	#if EVENT_AUDIO
	// 		event.create("audio_device.added");
	// 		event.create("audio_device.removed");
	// 		event.create("audio_device.fromta_changed");
	// 	#endif

	// 	#if EVENT_SENSOR
	// 		event.create("sensor.update");
	// 	#endif

	// 	#if EVENT_PEN
	// 		event.create("pen.down");
	// 		event.create("pen.up");
	// 		event.create("pen.motion");
	// 		event.create("pen.button.down");
	// 		event.create("pen.button.up");
	// 	#endif

	// 	#if EVENT_CAMERA
	// 		event.create("camera_device.added");
	// 		event.create("camera_device.removed");
	// 		event.create("camera_device.approved");
	// 		event.create("camera_device.denied");
	// 	#endif

	// 	#if EVENT_RENDER
	// 		event.create("render_target.reset");
	// 		event.create("render_device.reset");
	// 	#endif
	// }

	// Window::~Window(){
	// 	_context->logger->info("Destroying window ...");
	// 	if (_window){
	// 		_context->logger->info("Termintating SDL Window...");
	// 		SDL_DestroyWindow(_window);
	// 	}

	// 	_context->logger->info("Terminating SDL API...");
	// 	SDL_Quit();
	// }

	// std::vector<const char*> Window::vulkanExtensions(){
	// 	uint32_t extensionCount = 0;
	// 	const auto& exts = SDL_Vulkan_GetInstanceExtensions(&extensionCount);
	// 	if (exts == nullptr){
	// 		throw std::runtime_error("Failed to querry SDL vulkan instance extensions");
	// 	}

	// 	std::vector<const char*> extensions(&exts[0], &exts[extensionCount]);
		
	// 	return extensions;
	// }

	// void Window::createSurface(){
	// 	_context->logger->debug("Constructing SDL Vulkan surface");
	// 	if (SDL_Vulkan_CreateSurface(_window, _context.getInstance().get(), _context.getAllocationCallbacks(), &_surface) == SDL_FALSE){
	// 		_context->logger->error("Failed to create SDL Vulkan surface : {}", SDL_GetError());
	// 		throw std::runtime_error("Failed to create SDL Vulkan surface");
	// 	}
	// 	_context->logger->debug("SDL Vulkan surface created without any critical error");
	// }

	// void Window::destroySurface(){
	// 	if (_surface){
	// 		_context->logger->debug("Destroying SDL Vulkan surface");
	// 		vkDestroySurfaceKHR(_context.getInstance().get(), _surface, _context.getAllocationCallbacks());
	// 		_context->logger->debug("SDL Vulkan surface destroyed without any critical error");
	// 	}
	// }

	// void Window::setTitle(const char* title){
	// 	SDL_SetWindowTitle(_window, title);
	// }

	// void Window::setSize(Maths::u32vec2 size){
	// 	SDL_SetWindowSize(_window, static_cast<int>(size.x), static_cast<int>(size.y));
	// }

	// void Window::setPosition(Maths::u32vec2 position){
	// 	SDL_SetWindowPosition(_window, static_cast<int>(position.x), static_cast<int>(position.y));
	// }

	// const char* Window::getTitle() const{
	// 	return SDL_GetWindowTitle(_window);
	// }

	// Maths::u32vec2 Window::getSize() const{
	// 	int w, h;
	// 	SDL_GetWindowSize(_window, &w, &h);
	// 	return Maths::u32vec2(w, h);
	// }

	// Maths::u32vec2 Window::getPosition() const{
	// 	int x, y;
	// 	SDL_GetWindowPosition(_window, &x, &y);
	// 	return Maths::u32vec2(x, y);
	// }

	// SDL_Window* Window::get() const{
	// 	return _window;
	// }

	// bool Window::loaded() const{
	// 	return _window != nullptr;
	// }

	// VkSurfaceKHR Window::surface(){
	// 	return _surface;
	// }

	// void Window::events(){
	// 	SDL_Event e;

	// 	while (SDL_PollEvent(&e)){
	// 		switch (e.type){

	// 			#if EVENT_INFO
	// 				case SDL_EVENT_QUIT: quitEvent(e); break;

	// 				case SDL_EVENT_TERMINATING: terminatingEvent(e); break;
	// 				case SDL_EVENT_LOW_MEMORY: lowMemoryEvent(e); break;

	// 				case SDL_EVENT_WILL_ENTER_BACKGROUND: willEnterBackgroundEvent(e); break;
	// 				case SDL_EVENT_DID_ENTER_BACKGROUND: didEnterBackgroundEvent(e); break;
	// 				case SDL_EVENT_WILL_ENTER_FOREGROUND: willEnterForgroundEvent(e); break;
	// 				case SDL_EVENT_DID_ENTER_FOREGROUND: didEnterForgourndEvent(e); break;

	// 				case SDL_EVENT_LOCALE_CHANGED: localeChangedEvent(e); break;
	// 				case SDL_EVENT_SYSTEM_THEME_CHANGED: systemThemChangedEvent(e); break;
	// 			#endif
				
	// 			#if EVENT_DISPLAY
	// 				case SDL_EVENT_DISPLAY_ORIENTATION: displayOrientationEvent(e); break;
	// 				case SDL_EVENT_DISPLAY_ADDED: displayAddedEvent(e); break;
	// 				case SDL_EVENT_DISPLAY_REMOVED: displayRemovedEvent(e); break;
	// 				case SDL_EVENT_DISPLAY_MOVED: displayMovedEvent(e); break;
	// 				case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED: displayContentScaleChangedEvent(e); break;
	// 				case SDL_EVENT_DISPLAY_HDR_STATE_CHANGED: displayHDRStateChnagedEvent(e); break;
	// 			#endif

	// 			#if EVENT_WINDOW
	// 				case SDL_EVENT_WINDOW_SHOWN: windowShownEvent(e); break;
	// 				case SDL_EVENT_WINDOW_HIDDEN: windowHiddenEvent(e); break;
	// 				case SDL_EVENT_WINDOW_EXPOSED: windowExposedEvent(e); break;
	// 				case SDL_EVENT_WINDOW_MOVED: windowMovedEvent(e); break;
	// 				case SDL_EVENT_WINDOW_RESIZED: windowResizedEvent(e); break;
	// 				case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: windowPixelSizeChangedEvent(e); break;
	// 				case SDL_EVENT_WINDOW_MINIMIZED: windowMinimizedEvent(e); break;
	// 				case SDL_EVENT_WINDOW_MAXIMIZED: windowMaximizedEvent(e); break;
	// 				case SDL_EVENT_WINDOW_RESTORED: windowRestoredEvent(e); break;
	// 				case SDL_EVENT_WINDOW_MOUSE_ENTER: windowMouseEnterEvent(e); break;
	// 				case SDL_EVENT_WINDOW_MOUSE_LEAVE: windowMouseLeaveEvent(e); break;
	// 				case SDL_EVENT_WINDOW_FOCUS_GAINED: windowFocusGainedEvent(e); break;
	// 				case SDL_EVENT_WINDOW_FOCUS_LOST: windowFocusLostEvent(e); break;
	// 				case SDL_EVENT_WINDOW_CLOSE_REQUESTED: windowCloseRequestedEvent(e); break;
	// 				case SDL_EVENT_WINDOW_TAKE_FOCUS: windowTakeFocusEvent(e); break;
	// 				case SDL_EVENT_WINDOW_HIT_TEST: windowHitTestEvent(e); break;
	// 				case SDL_EVENT_WINDOW_ICCPROF_CHANGED: windowICCPROFChangedEvent(e); break;
	// 				case SDL_EVENT_WINDOW_DISPLAY_CHANGED: windowDisplayChangedEvent(e); break;
	// 				case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED: windowDisplayScaleChangedEvent(e); break;
	// 				case SDL_EVENT_WINDOW_OCCLUDED: windowOccludedEvent(e); break;
	// 				case SDL_EVENT_WINDOW_ENTER_FULLSCREEN: windowEnterFullsreenEvent(e); break;
	// 				case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN: windowLeaveFullscreenEvent(e); break;
	// 				case SDL_EVENT_WINDOW_DESTROYED: windowDestroyedEvent(e); break;
	// 				case SDL_EVENT_WINDOW_PEN_ENTER: windowPenEnterEvent(e); break;
	// 				case SDL_EVENT_WINDOW_PEN_LEAVE: windowPenLeaveEvent(e); break;
	// 			#endif

	// 			#if EVENT_KEY
	// 				case SDL_EVENT_KEY_DOWN: keyDownEvent(e); break;
	// 				case SDL_EVENT_KEY_UP: keyUpEvent(e); break;
	// 				case SDL_EVENT_TEXT_EDITING: textEditingEvent(e); break;
	// 				case SDL_EVENT_TEXT_INPUT: textInputEvent(e); break;
	// 				case SDL_EVENT_KEYMAP_CHANGED: keymapChangedEvent(e); break;
	// 			#endif
				
	// 			#if EVENT_MOUSE
	// 				case SDL_EVENT_MOUSE_MOTION: mouseMotionEvent(e); break;
	// 				case SDL_EVENT_MOUSE_BUTTON_DOWN: mouseButtonDownEvent(e); break;
	// 				case SDL_EVENT_MOUSE_BUTTON_UP: mouseButtonUpEvent(e); break;
	// 				case SDL_EVENT_MOUSE_WHEEL: mouseWheelEvent(e); break;
	// 			#endif

	// 			#if EVENT_JOYSTICK
	// 				case SDL_EVENT_JOYSTICK_AXIS_MOTION: joystickAxisMotionEvent(e); break;
	// 				case SDL_EVENT_JOYSTICK_HAT_MOTION: joystickHatMotionEvent(e); break;
	// 				case SDL_EVENT_JOYSTICK_BUTTON_DOWN: joystickButtonDownEvent(e); break;
	// 				case SDL_EVENT_JOYSTICK_BUTTON_UP: joystickButtonUpEvent(e); break;
	// 				case SDL_EVENT_JOYSTICK_ADDED: joystickAddedEvent(e); break;
	// 				case SDL_EVENT_JOYSTICK_REMOVED: joystickRemovedEvent(e); break;
	// 				case SDL_EVENT_JOYSTICK_BATTERY_UPDATED: joystickBatteryUpdatedEvent(e); break;
	// 				case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE: joystickUpdateCompletedEvent(e); break;
	// 			#endif

	// 			#if EVENT_GAMEPAD
	// 				case SDL_EVENT_GAMEPAD_AXIS_MOTION: gamepadAxisMotionEvent(e); break;
	// 				case SDL_EVENT_GAMEPAD_BUTTON_DOWN: gamepadButtonDownEvent(e); break;
	// 				case SDL_EVENT_GAMEPAD_BUTTON_UP: gamepadButtonUpEvent(e); break;
	// 				case SDL_EVENT_GAMEPAD_ADDED: gamepadAddedEvent(e); break;
	// 				case SDL_EVENT_GAMEPAD_REMOVED: gamepadRemovedEvent(e); break;
	// 				case SDL_EVENT_GAMEPAD_REMAPPED: gamepadRemappedEvent(e); break;
	// 				case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN: gamepadTouchpadDownEvent(e); break;
	// 				case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION: gamepadTouchpadMotionEvent(e); break;
	// 				case SDL_EVENT_GAMEPAD_TOUCHPAD_UP: gamepadTouchpadUpEvent(e); break;
	// 				case SDL_EVENT_GAMEPAD_SENSOR_UPDATE: gamepadSensorUpdateEvent(e); break;
	// 				case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE: gamepadUpdateCompleteEvent(e); break;
	// 				case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED: gamepadSteamHandleUpdatedEvent(e); break;
	// 			#endif
				
	// 			#if EVENT_FINGER
	// 				case SDL_EVENT_FINGER_DOWN: fingerDownEvent(e); break;
	// 				case SDL_EVENT_FINGER_UP: fingerUpEvent(e); break;
	// 				case SDL_EVENT_FINGER_MOTION: fingerMotionEvent(e); break;
	// 			#endif
			
	// 			#if EVENT_CLIPBOARD
	// 				case SDL_EVENT_CLIPBOARD_UPDATE: clipboardUpdateEvent(e); break;
	// 			#endif

	// 			#if EVENT_TEXT
	// 				case SDL_EVENT_DROP_FILE: dropFileEvent(e); break;
	// 				case SDL_EVENT_DROP_TEXT: dropTextEvent(e); break;
	// 				case SDL_EVENT_DROP_BEGIN: dropBeginEvent(e); break;
	// 				case SDL_EVENT_DROP_COMPLETE: dropCompleteEvent(e); break;
	// 				case SDL_EVENT_DROP_POSITION: dropPositionEvent(e); break;
	// 			#endif

	// 			#if EVENT_AUDIO
	// 				case SDL_EVENT_AUDIO_DEVICE_ADDED: audioDeviceAddedEvent(e); break;
	// 				case SDL_EVENT_AUDIO_DEVICE_REMOVED: audioDeviceRemovedEvent(e); break;
	// 				case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED: audioDeviceFormatChangedEvent(e); break;
	// 			#endif

	// 			#if EVENR_SENSOR
	// 				case SDL_EVENT_SENSOR_UPDATE: sensorUpdateEvent(e); break;
	// 			#endif

	// 			#if EVENT_PEN
	// 				case SDL_EVENT_PEN_DOWN: penDownEvent(e); break;
	// 				case SDL_EVENT_PEN_UP: penUpEvent(e); break;
	// 				case SDL_EVENT_PEN_MOTION: penMotionEvent(e); break;
	// 				case SDL_EVENT_PEN_BUTTON_DOWN: penButtonDownEvent(e); break;
	// 				case SDL_EVENT_PEN_BUTTON_UP: penButtonUpEvent(e); break;
	// 			#endif

	// 			#if EVENT_CAMERA
	// 				case SDL_EVENT_CAMERA_DEVICE_ADDED: cameraDeviceAddedEvent(e); break;
	// 				case SDL_EVENT_CAMERA_DEVICE_REMOVED: cameraDeviceRemovedEvent(e); break;
	// 				case SDL_EVENT_CAMERA_DEVICE_APPROVED: cameraDeviceApprovedEvent(e); break;
	// 				case SDL_EVENT_CAMERA_DEVICE_DENIED: cameraDeviceDeniedEvent(e); break;
	// 			#endif
				
	// 			#if EVENT_RENDER
	// 				case SDL_EVENT_RENDER_TARGETS_RESET: renderTargetsResetEvent(e); break;
	// 				case SDL_EVENT_RENDER_DEVICE_RESET: renderDeviceResetEvent(e); break;
	// 			#endif
	// 		}
	// 	}
	// }

	// ::Raindrop::Event::Key SDL_ScancodeToKey(SDL_Scancode code){
	// 	return static_cast<::Raindrop::Event::Key>(code);
	// }

	// ::Raindrop::Event::Key SDL_KeycodeToKey(SDL_Keycode code){
	// 	return SDL_ScancodeToKey(SDL_GetScancodeFromKey(code));
	// }

	// ::Raindrop::Event::Keymod SDL_KeyModeToKeyMode(Uint16 code){
	// 	return Raindrop::Event::Keymod(code);
	// }

	// void Window::quitEvent(SDL_Event &e){
	// 	_context.getInternalContext().getEventManager().get("quit").trigger();
	// }
	
	// void Window::terminatingEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("terminating").trigger();
	// }

	// void Window::lowMemoryEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("memory.low").trigger();
	// }

	// void Window::willEnterBackgroundEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("will_enter_background").trigger();
	// }

	// void Window::didEnterBackgroundEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("did_enter_background").trigger();
	// }

	// void Window::willEnterForgroundEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("will_enter_forground").trigger();
	// }

	// void Window::didEnterForgourndEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("did_enter_forground").trigger();
	// }

	// void Window::localeChangedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("locale.changed").trigger();
	// }

	// void Window::systemThemChangedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("system_theme.changed").trigger();
	// }

	// void Window::displayOrientationEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("display.orientation").trigger(e.display.data1);
	// }

	// void Window::displayAddedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("display.added").trigger();
	// }

	// void Window::displayRemovedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("display.removed").trigger();
	// }

	// void Window::displayMovedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("display.moved").trigger();
	// }

	// void Window::displayContentScaleChangedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("display.content_scale.changed").trigger();
	// }

	// void Window::displayHDRStateChnagedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("display.HDR_state.changed").trigger();
	// }

	// void Window::windowShownEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.shown").trigger();
	// }

	// void Window::windowHiddenEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.hidden").trigger();
	// }

	// void Window::windowExposedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.exposed").trigger();
	// }

	// void Window::windowMovedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.moved").trigger();
	// }

	// void Window::windowResizedEvent(SDL_Event& e){
	// 	_resized = true;
	// 	_context.getInternalContext().getEventManager().get("window.resized").trigger(Maths::ivec2(e.window.data1, e.window.data2));
	// }

	// void Window::windowPixelSizeChangedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.pixel_size.changed").trigger(Maths::ivec2(e.window.data1, e.window.data2));
	// }

	// void Window::windowMinimizedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.minimized").trigger();
	// }

	// void Window::windowMaximizedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.maximized").trigger();
	// }

	// void Window::windowRestoredEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.restored").trigger();
	// }

	// void Window::windowMouseEnterEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.mouse.enter").trigger();
	// }

	// void Window::windowMouseLeaveEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.mouse.leave").trigger();
	// }

	// void Window::windowFocusGainedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.focus.gained").trigger();
	// }

	// void Window::windowFocusLostEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.focus.lost").trigger();
	// }

	// void Window::windowCloseRequestedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.close.requested").trigger();
	// }

	// void Window::windowTakeFocusEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.focus.take").trigger();
	// }

	// void Window::windowHitTestEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.hit_test").trigger();
	// }

	// void Window::windowICCPROFChangedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.ICCPROF.changed").trigger();
	// }

	// void Window::windowDisplayChangedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.display.changed").trigger(e.window.data1);
	// }

	// void Window::windowDisplayScaleChangedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.display.scale.changed").trigger();
	// }

	// void Window::windowOccludedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.occluded").trigger();
	// }

	// void Window::windowEnterFullsreenEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.fullscreen.enter").trigger();
	// }

	// void Window::windowLeaveFullscreenEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.fullscreen.leave").trigger();
	// }

	// void Window::windowDestroyedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.destroyed").trigger();
	// }

	// void Window::windowPenEnterEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.pen.enter").trigger();
	// }

	// void Window::windowPenLeaveEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("window.pen.leave").trigger();
	// }

	// void Window::keyDownEvent(SDL_Event& e){
	// 	const auto& keysym = e.key.keysym;
	// 	_context.getInternalContext().getEventManager().get("key.down").trigger(SDL_ScancodeToKey(keysym.scancode), SDL_KeycodeToKey(keysym.sym), keysym.mod);
	// }

	// void Window::keyUpEvent(SDL_Event& e){
	// 	const auto& keysym = e.key.keysym;
	// 	_context.getInternalContext().getEventManager().get("key.up").trigger(SDL_ScancodeToKey(keysym.scancode), SDL_KeycodeToKey(keysym.sym), keysym.mod);
	// }

	// void Window::textEditingEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("text.editing").trigger(e.edit.text, e.edit.start, e.edit->length);
	// }

	// void Window::textInputEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("text.input").trigger(e.text.text);
	// }

	// void Window::keymapChangedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("keymap.changed").trigger();
	// }

	// void Window::mouseMotionEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("mouse.motion").trigger(Maths::vec2(e.motion.x, e.motion.y), Maths::vec2(e.motion.xrel, e.motion.yrel));
	// }

	// void Window::mouseButtonDownEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("mouse.button.down").trigger(e.button.button, e.button.clicks, Maths::vec2(e.button.x, e.button.y));
	// }

	// void Window::mouseButtonUpEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("mouse.button.up").trigger(e.button.button, Maths::vec2(e.button.x, e.button.y));
	// }

	// void Window::mouseWheelEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("mouse.wheel").trigger(Maths::vec2(e.wheel.x, e.wheel.y), Maths::vec2(e.wheel.mouse_x, e.wheel.mouse_y));
	// }

	// void Window::joystickAxisMotionEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("joystick.axis.motion").trigger(e.jaxis.axis, e.jaxis.value);
	// }

	// void Window::joystickHatMotionEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("joystick.hat.motion").trigger(e.jhat.hat, e.jhat.value);
	// }

	// void Window::joystickButtonDownEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("joystick.button.down").trigger(e.jbutton.button);
	// }

	// void Window::joystickButtonUpEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("joystick.button.up").trigger(e.jbutton.button);
	// }

	// void Window::joystickAddedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("joystick.added").trigger();
	// }

	// void Window::joystickRemovedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("joystick.removed").trigger();
	// }

	// void Window::joystickBatteryUpdatedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("joystick.battery.updated").trigger(e.jbattery.level);
	// }

	// void Window::joystickUpdateCompletedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("joystick.update.complete").trigger();
	// }

	// void Window::gamepadAxisMotionEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("gamepad.axis.motion").trigger(e.gaxis.axis, e.gaxis.value);
	// }

	// void Window::gamepadButtonDownEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("gamepad.button.down").trigger(e.gbutton.button);
	// }

	// void Window::gamepadButtonUpEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("gamepad.button.up").trigger(e.gbutton.button);
	// }

	// void Window::gamepadAddedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("gamepad.added").trigger();
	// }

	// void Window::gamepadRemovedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("gamepad.removed").trigger();
	// }

	// void Window::gamepadRemappedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("gamepad.remapped").trigger();
	// }

	// void Window::gamepadTouchpadDownEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("gamepad.touchpad.down").trigger(Maths::vec2(e.gtouchpad.x, e.gtouchpad.y), e.gtouchpad.pressure);
	// }
	
	// void Window::gamepadTouchpadMotionEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("gamepad.touchpad.motion").trigger(Maths::vec2(e.gtouchpad.x, e.gtouchpad.y), e.gtouchpad.pressure);
	// }

	// void Window::gamepadTouchpadUpEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("gamepad.touchpad.up").trigger(Maths::vec2(e.gtouchpad.x, e.gtouchpad.y), e.gtouchpad.pressure);
	// }

	// void Window::gamepadSensorUpdateEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("gamepad.sensor.update").trigger(e.gsensor.sensor_timestamp, Maths::vec3(e.gsensor.data[0], e.gsensor.data[1], e.gsensor.data[2]));
	// }

	// void Window::gamepadUpdateCompleteEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("gamepad.update.complete").trigger();
	// }

	// void Window::gamepadSteamHandleUpdatedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("gamepad.steam_handle_updated").trigger();
	// }

	// void Window::fingerDownEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("finger.down").trigger(e.tfinger.fingerID, Maths::vec2(e.tfinger.x, e.tfinger.y));
	// }

	// void Window::fingerUpEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("finger.up").trigger(e.tfinger.fingerID, Maths::vec2(e.tfinger.x, e.tfinger.y));
	// }

	// void Window::fingerMotionEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("finger.motion").trigger(e.tfinger.fingerID, Maths::vec2(e.tfinger.x, e.tfinger.y), Maths::vec2(e.tfinger.dx, e.tfinger.dy));
	// }

	// void Window::clipboardUpdateEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("clipboard.update").trigger();
	// }

	// void Window::dropFileEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("drop.file").trigger();
	// }

	// void Window::dropTextEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("drop.text").trigger();
	// }

	// void Window::dropBeginEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("drop.begin").trigger();
	// }

	// void Window::dropCompleteEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("drop.complete").trigger();
	// }

	// void Window::dropPositionEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("drop.position").trigger();
	// }

	// void Window::audioDeviceAddedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("audio_device.added").trigger();
	// }

	// void Window::audioDeviceRemovedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("audio_device.removed").trigger();
	// }

	// void Window::audioDeviceFormatChangedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("audio_device.format_changed").trigger();
	// }

	// void Window::sensorUpdateEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("sensor.update").trigger();
	// }

	// void Window::penDownEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("pen.down").trigger();
	// }

	// void Window::penUpEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("pen.up").trigger();
	// }

	// void Window::penMotionEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("pen.motion").trigger();
	// }

	// void Window::penButtonDownEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("pen.button.down").trigger();
	// }

	// void Window::penButtonUpEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("pen.button.up").trigger();
	// }

	// void Window::cameraDeviceAddedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("camera_device.added").trigger();
	// }

	// void Window::cameraDeviceRemovedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("camera_device.removed").trigger();
	// }

	// void Window::cameraDeviceApprovedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("camera_device.approved").trigger();
	// }

	// void Window::cameraDeviceDeniedEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("camera_device.denied").trigger();
	// }

	// void Window::renderTargetsResetEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("render_targets.reset").trigger();
	// }

	// void Window::renderDeviceResetEvent(SDL_Event& e){
	// 	_context.getInternalContext().getEventManager().get("render_device.reset").trigger();
	// }
}