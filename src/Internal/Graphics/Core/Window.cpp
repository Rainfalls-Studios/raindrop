#include <Raindrop_internal/Graphics/Core/Window.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Events/Manager.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop/Event.hpp>

#include <SDL3/SDL_vulkan.h>
#include <spdlog/spdlog.h>

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

namespace Raindrop::Internal::Graphics::Core{
	static constexpr uint32_t DEFAULT_WINDOW_WIDTH = 1080;
	static constexpr uint32_t DEFAULT_WINDOW_HEIGHT = 720;

	Window::Window(Context& context) : _context{context}{
		_context.getLogger()->info("Constructing window ...");
		_context.getLogger()->info("Initializing SDL3 API...");
		if (SDL_Init(SDL_INIT_VIDEO) != 0){
			_context.getLogger()->error("Failed to initialize SDL3 API : {}", SDL_GetError());
			throw std::runtime_error("Failed initialize SDL3 API");
		}
		_context.getLogger()->info("SDL3 API initialized successfully");

		_context.getLogger()->info("Initializing SDL3 Window...");
		_window = SDL_CreateWindow("Raindrop::Internal::Graphics window", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
		if (!_window){
			_context.getLogger()->error("Failed to initialize SDL3 Window : {}", SDL_GetError());
			throw std::runtime_error("Failed to create SDL3 Window");
		}

		registerEvents();

		_context.getLogger()->info("SDL3 Window initialized successfully");
	}

	bool Window::resized() const{
		return _resized;
	}

	void Window::resetResizedFlags(){
		_resized = false;
	}

	/// @brief 
	void Window::registerEvents(){
		auto& event = _context.getInternalContext().getEventManager();

		#if EVENT_INFO
			event.registerEvent<>("quit");
			event.registerEvent<>("terminating");
			event.registerEvent<>("memory.low");
			event.registerEvent<>("will_enter_background");
			event.registerEvent<>("did_enter_background");
			event.registerEvent<>("will_enter_forground");
			event.registerEvent<>("did_enter_forground");
			event.registerEvent<>("locale.changed");
			event.registerEvent<>("system_theme.changed");
		#endif

		#if EVENT_DISPLAY
			event.registerEvent<unsigned int>("display.orientation");
			event.registerEvent<>("display.added");
			event.registerEvent<>("display.removed");
			event.registerEvent<>("display.moved");
			event.registerEvent<>("display.content_scale.changed");
			event.registerEvent<>("display.HDR_state.changed");
		#endif

		#if EVENT_WINDOW
			event.registerEvent<>("window.shown");
			event.registerEvent<>("window.hidden");
			event.registerEvent<>("window.exposed");
			event.registerEvent<>("window.moved");
			event.registerEvent<Maths::ivec2>("window.resized");
			event.registerEvent<Maths::ivec2>("window.pixel_size.changed");
			event.registerEvent<>("window.minimized");
			event.registerEvent<>("window.maximized");
			event.registerEvent<>("window.restored");
			event.registerEvent<>("window.mouse.enter");
			event.registerEvent<>("window.mouse.leave");
			event.registerEvent<>("window.focus.gained");
			event.registerEvent<>("window.focus.lost");
			event.registerEvent<>("window.close.requested");
			event.registerEvent<>("window.focus.take");
			event.registerEvent<>("window.hit_test");
			event.registerEvent<>("window.ICCPROF.changed");
			event.registerEvent<>("window.display.changed");
			event.registerEvent<>("window.display.scale.changed");
			event.registerEvent<>("window.occluded");
			event.registerEvent<>("window.fullscreen.leave");
			event.registerEvent<>("window.fullscreen.enter");
			event.registerEvent<>("window.destroyed");
			event.registerEvent<>("window.pen.enter");
			event.registerEvent<>("window.pen.leave");
		#endif

		#if EVENT_KEY
			using Key = ::Raindrop::Event::Key;
			using KeyMod = ::Raindrop::Event::Keymod;

			event.registerEvent<Key, Key, KeyMod>("key.down");
			event.registerEvent<Key, Key, KeyMod>("key.up");
			event.registerEvent<const char*, unsigned int, unsigned int>("text.editing");
			event.registerEvent<const char*>("text.input");
			event.registerEvent<const char*>("keymap.changed");

			using Key = ::Raindrop::Event::Key;
			using KeyState = ::Raindrop::Event::KeyState;
			
			// event.subscribe(
			// 	event.getEventID("key.down"),
			// 	[&event](Key scancode, Key keysym, KeyMod modifier){
			// 		event.keyEvents().state(scancode) = KeyState::PRESSED;
			// 	}
			// );

			// event.subscribe(
			// 	event.getEventID("key.up"),
			// 	[&event](Key scancode, Key keysym, KeyMod modifier){
			// 		event.keyEvents().state(scancode) = KeyState::RELEASED;
			// 	}
			// );

		#endif
		
		#if EVENT_MOUSE
			event.registerEvent<Maths::vec2, Maths::vec2>("mouse.motion");
			event.registerEvent<uint8_t, uint8_t, Maths::vec2>("mouse.button.down");
			event.registerEvent<uint8_t, Maths::vec2>("mouse.button.up");
			event.registerEvent<Maths::vec2, Maths::vec2>("mouse.wheel");

			// event.subscribe(
			// 	event.getEventID("mouse.motion"),
			// 	[&event](Maths::vec2 position, Maths::vec2 relativePosition){
			// 		event.mouseEvents().pos() = position;
			// 		event.mouseEvents().relPos() = relativePosition;
			// 	}
			// );

			// using Button = ::Raindrop::Event::MouseButton;
			// using ButtonState = ::Raindrop::Event::ButtonState;

			// event.subscribe(
			// 	event.getEventID("mouse.button.down"),
			// 	[&event](uint8_t button, uint8_t clicks, Maths::vec2 position){
			// 		event.mouseEvents().state(static_cast<Button>(button)) = ButtonState::DOWN;
			// 	}
			// );

			// event.subscribe(
			// 	event.getEventID("mouse.button.up"),
			// 	[&event](uint8_t button, Maths::vec2 position){
			// 		event.mouseEvents().state(static_cast<Button>(button)) = ButtonState::UP;
			// 	}
			// );
		#endif

		#if EVENT_JOYSTICK
			event.registerEvent<uint8_t, int16_t>("joystick.axis.motion");
			event.registerEvent<uint8_t, int16_t>("joystick.hat.motion");
			event.registerEvent<uint8_t>("joystick.button.down");
			event.registerEvent<uint8_t>("joystick.button.up");
			event.registerEvent<>("joystick.added");
			event.registerEvent<>("joystick.removed");
			event.registerEvent<SDL_JoystickPowerLevel>("joystick.battery.updated");
			event.registerEvent<>("joystick.update.complete");
		#endif

		#if EVENT_GAMEPAD
			event.registerEvent<uint8_t, int16_t>("gamepad.axis.motion");
			event.registerEvent<uint8_t>("gamepad.button.down");
			event.registerEvent<uint8_t>("gamepad.button.up");
			event.registerEvent<>("gamepad.added");
			event.registerEvent<>("gamepad.removed");
			event.registerEvent<>("gamepad.remapped");
			event.registerEvent<Maths::vec2, float>("gamepad.touchpad.down");
			event.registerEvent<Maths::vec2, float>("gamepad.touchpad.motion");
			event.registerEvent<Maths::vec2, float>("gamepad.touchpad.up");
			event.registerEvent<uint64_t, Maths::vec3>("gamepad.sensor.update");
			event.registerEvent<>("gamepad.update.complete");
			event.registerEvent<>("gamepad.steam_handle_updated");
		#endif

		#if EVENT_FINGER
			event.registerEvent<SDL_FingerID, Maths::vec2>("finger.down");
			event.registerEvent<SDL_FingerID, Maths::vec2>("finger.up");
			event.registerEvent<SDL_FingerID, Maths::vec2, Maths::vec2>("finger.motion");
		#endif

		#if EVENT_CLIPBOARD
			event.registerEvent<>("clipboard.update");
		#endif

		#if EVENT_DROP
			event.registerEvent<>("drop.file");
			event.registerEvent<>("drop.text");
			event.registerEvent<>("drop.begin");
			event.registerEvent<>("drop.complete");
			event.registerEvent<>("drop.position");
		#endif

		#if EVENT_AUDIO
			event.registerEvent<>("audio_device.added");
			event.registerEvent<>("audio_device.removed");
			event.registerEvent<>("audio_device.fromta_changed");
		#endif

		#if EVENT_SENSOR
			event.registerEvent<>("sensor.update");
		#endif

		#if EVENT_PEN
			event.registerEvent<>("pen.down");
			event.registerEvent<>("pen.up");
			event.registerEvent<>("pen.motion");
			event.registerEvent<>("pen.button.down");
			event.registerEvent<>("pen.button.up");
		#endif

		#if EVENT_CAMERA
			event.registerEvent<>("camera_device.added");
			event.registerEvent<>("camera_device.removed");
			event.registerEvent<>("camera_device.approved");
			event.registerEvent<>("camera_device.denied");
		#endif

		#if EVENT_RENDER
			event.registerEvent<>("render_target.reset");
			event.registerEvent<>("render_device.reset");
		#endif
	}

	Window::~Window(){
		_context.getLogger()->info("Destroying window ...");
		if (_window){
			_context.getLogger()->info("Termintating SDL3 Window...");
			SDL_DestroyWindow(_window);
		}

		_context.getLogger()->info("Terminating SDL3 API...");
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
		_context.getLogger()->debug("Constructing SDL3 Vulkan surface");
		if (SDL_Vulkan_CreateSurface(_window, _context.getInstance().get(), _context.getAllocationCallbacks(), &_surface) == SDL_FALSE){
			_context.getLogger()->error("Failed to create SDL3 Vulkan surface : {}", SDL_GetError());
			throw std::runtime_error("Failed to create SDL3 Vulkan surface");
		}
		_context.getLogger()->debug("SDL3 Vulkan surface created without any critical error");
	}

	void Window::destroySurface(){
		if (_surface){
			_context.getLogger()->debug("Destroying SDL3 Vulkan surface");
			vkDestroySurfaceKHR(_context.getInstance().get(), _surface, _context.getAllocationCallbacks());
			_context.getLogger()->debug("SDL3 Vulkan surface destroyed without any critical error");
		}
	}

	void Window::setTitle(const char* title){
		SDL_SetWindowTitle(_window, title);
	}

	void Window::setSize(Maths::u32vec2 size){
		SDL_SetWindowSize(_window, static_cast<int>(size.x), static_cast<int>(size.y));
	}

	void Window::setPosition(Maths::u32vec2 position){
		SDL_SetWindowPosition(_window, static_cast<int>(position.x), static_cast<int>(position.y));
	}

	const char* Window::getTitle() const{
		return SDL_GetWindowTitle(_window);
	}

	Maths::u32vec2 Window::getSize() const{
		int w, h;
		SDL_GetWindowSize(_window, &w, &h);
		return Maths::u32vec2(w, h);
	}

	Maths::u32vec2 Window::getPosition() const{
		int x, y;
		SDL_GetWindowPosition(_window, &x, &y);
		return Maths::u32vec2(x, y);
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

				#if EVENT_INFO
					case SDL_EVENT_QUIT: quitEvent(e); break;

					case SDL_EVENT_TERMINATING: terminatingEvent(e); break;
					case SDL_EVENT_LOW_MEMORY: lowMemoryEvent(e); break;

					case SDL_EVENT_WILL_ENTER_BACKGROUND: willEnterBackgroundEvent(e); break;
					case SDL_EVENT_DID_ENTER_BACKGROUND: didEnterBackgroundEvent(e); break;
					case SDL_EVENT_WILL_ENTER_FOREGROUND: willEnterForgroundEvent(e); break;
					case SDL_EVENT_DID_ENTER_FOREGROUND: didEnterForgourndEvent(e); break;

					case SDL_EVENT_LOCALE_CHANGED: localeChangedEvent(e); break;
					case SDL_EVENT_SYSTEM_THEME_CHANGED: systemThemChangedEvent(e); break;
				#endif
				
				#if EVENT_DISPLAY
					case SDL_EVENT_DISPLAY_ORIENTATION: displayOrientationEvent(e); break;
					case SDL_EVENT_DISPLAY_ADDED: displayAddedEvent(e); break;
					case SDL_EVENT_DISPLAY_REMOVED: displayRemovedEvent(e); break;
					case SDL_EVENT_DISPLAY_MOVED: displayMovedEvent(e); break;
					case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED: displayContentScaleChangedEvent(e); break;
					case SDL_EVENT_DISPLAY_HDR_STATE_CHANGED: displayHDRStateChnagedEvent(e); break;
				#endif

				#if EVENT_WINDOW
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
				#endif

				#if EVENT_KEY
					case SDL_EVENT_KEY_DOWN: keyDownEvent(e); break;
					case SDL_EVENT_KEY_UP: keyUpEvent(e); break;
					case SDL_EVENT_TEXT_EDITING: textEditingEvent(e); break;
					case SDL_EVENT_TEXT_INPUT: textInputEvent(e); break;
					case SDL_EVENT_KEYMAP_CHANGED: keymapChangedEvent(e); break;
				#endif
				
				#if EVENT_MOUSE
					case SDL_EVENT_MOUSE_MOTION: mouseMotionEvent(e); break;
					case SDL_EVENT_MOUSE_BUTTON_DOWN: mouseButtonDownEvent(e); break;
					case SDL_EVENT_MOUSE_BUTTON_UP: mouseButtonUpEvent(e); break;
					case SDL_EVENT_MOUSE_WHEEL: mouseWheelEvent(e); break;
				#endif

				#if EVENT_JOYSTICK
					case SDL_EVENT_JOYSTICK_AXIS_MOTION: joystickAxisMotionEvent(e); break;
					case SDL_EVENT_JOYSTICK_HAT_MOTION: joystickHatMotionEvent(e); break;
					case SDL_EVENT_JOYSTICK_BUTTON_DOWN: joystickButtonDownEvent(e); break;
					case SDL_EVENT_JOYSTICK_BUTTON_UP: joystickButtonUpEvent(e); break;
					case SDL_EVENT_JOYSTICK_ADDED: joystickAddedEvent(e); break;
					case SDL_EVENT_JOYSTICK_REMOVED: joystickRemovedEvent(e); break;
					case SDL_EVENT_JOYSTICK_BATTERY_UPDATED: joystickBatteryUpdatedEvent(e); break;
					case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE: joystickUpdateCompletedEvent(e); break;
				#endif

				#if EVENT_GAMEPAD
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
				#endif
				
				#if EVENT_FINGER
					case SDL_EVENT_FINGER_DOWN: fingerDownEvent(e); break;
					case SDL_EVENT_FINGER_UP: fingerUpEvent(e); break;
					case SDL_EVENT_FINGER_MOTION: fingerMotionEvent(e); break;
				#endif
			
				#if EVENT_CLIPBOARD
					case SDL_EVENT_CLIPBOARD_UPDATE: clipboardUpdateEvent(e); break;
				#endif

				#if EVENT_TEXT
					case SDL_EVENT_DROP_FILE: dropFileEvent(e); break;
					case SDL_EVENT_DROP_TEXT: dropTextEvent(e); break;
					case SDL_EVENT_DROP_BEGIN: dropBeginEvent(e); break;
					case SDL_EVENT_DROP_COMPLETE: dropCompleteEvent(e); break;
					case SDL_EVENT_DROP_POSITION: dropPositionEvent(e); break;
				#endif

				#if EVENT_AUDIO
					case SDL_EVENT_AUDIO_DEVICE_ADDED: audioDeviceAddedEvent(e); break;
					case SDL_EVENT_AUDIO_DEVICE_REMOVED: audioDeviceRemovedEvent(e); break;
					case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED: audioDeviceFormatChangedEvent(e); break;
				#endif

				#if EVENR_SENSOR
					case SDL_EVENT_SENSOR_UPDATE: sensorUpdateEvent(e); break;
				#endif

				#if EVENT_PEN
					case SDL_EVENT_PEN_DOWN: penDownEvent(e); break;
					case SDL_EVENT_PEN_UP: penUpEvent(e); break;
					case SDL_EVENT_PEN_MOTION: penMotionEvent(e); break;
					case SDL_EVENT_PEN_BUTTON_DOWN: penButtonDownEvent(e); break;
					case SDL_EVENT_PEN_BUTTON_UP: penButtonUpEvent(e); break;
				#endif

				#if EVENT_CAMERA
					case SDL_EVENT_CAMERA_DEVICE_ADDED: cameraDeviceAddedEvent(e); break;
					case SDL_EVENT_CAMERA_DEVICE_REMOVED: cameraDeviceRemovedEvent(e); break;
					case SDL_EVENT_CAMERA_DEVICE_APPROVED: cameraDeviceApprovedEvent(e); break;
					case SDL_EVENT_CAMERA_DEVICE_DENIED: cameraDeviceDeniedEvent(e); break;
				#endif
				
				#if EVENT_RENDER
					case SDL_EVENT_RENDER_TARGETS_RESET: renderTargetsResetEvent(e); break;
					case SDL_EVENT_RENDER_DEVICE_RESET: renderDeviceResetEvent(e); break;
				#endif
			}
		}
	}

	::Raindrop::Event::Key SDL_ScancodeToKey(SDL_Scancode code){
		return static_cast<::Raindrop::Event::Key>(code);
	}

	::Raindrop::Event::Key SDL_KeycodeToKey(SDL_Keycode code){
		return SDL_ScancodeToKey(SDL_GetScancodeFromKey(code));
	}

	::Raindrop::Event::Keymod SDL_KeyModeToKeyMode(Uint16 code){
		return static_cast<::Raindrop::Event::Keymod::Bitset>(code);
	}

	void Window::quitEvent(SDL_Event &e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("quit"));
	}
	
	void Window::terminatingEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("terminating"));
	}

	void Window::lowMemoryEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("memory.low"));
	}

	void Window::willEnterBackgroundEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("will_enter_background"));
	}

	void Window::didEnterBackgroundEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("did_enter_background"));
	}

	void Window::willEnterForgroundEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("will_enter_forground"));
	}

	void Window::didEnterForgourndEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("did_enter_forground"));
	}

	void Window::localeChangedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("locale.changed"));
	}

	void Window::systemThemChangedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("system_theme.changed"));
	}

	void Window::displayOrientationEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("display.orientation"), e.display.data1);
	}

	void Window::displayAddedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("display.added"));
	}

	void Window::displayRemovedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("display.removed"));
	}

	void Window::displayMovedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("display.moved"));
	}

	void Window::displayContentScaleChangedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("display.content_scale.changed"));
	}

	void Window::displayHDRStateChnagedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("display.HDR_state.changed"));
	}

	void Window::windowShownEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.shown"));
	}

	void Window::windowHiddenEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.hidden"));
	}

	void Window::windowExposedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.exposed"));
	}

	void Window::windowMovedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.moved"));
	}

	void Window::windowResizedEvent(SDL_Event& e){
		_resized = true;
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.resized"), Maths::ivec2(e.window.data1, e.window.data2));
	}

	void Window::windowPixelSizeChangedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.pixel_size.changed"), Maths::ivec2(e.window.data1, e.window.data2));
	}

	void Window::windowMinimizedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.minimized"));
	}

	void Window::windowMaximizedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.maximized"));
	}

	void Window::windowRestoredEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.restored"));
	}

	void Window::windowMouseEnterEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.mouse.enter"));
	}

	void Window::windowMouseLeaveEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.mouse.leave"));
	}

	void Window::windowFocusGainedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.focus.gained"));
	}

	void Window::windowFocusLostEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.focus.lost"));
	}

	void Window::windowCloseRequestedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.close.requested"));
	}

	void Window::windowTakeFocusEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.focus.take"));
	}

	void Window::windowHitTestEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.hit_test"));
	}

	void Window::windowICCPROFChangedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.ICCPROF.changed"));
	}

	void Window::windowDisplayChangedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.display.changed"), e.window.data1);
	}

	void Window::windowDisplayScaleChangedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.display.scale.changed"));
	}

	void Window::windowOccludedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.occluded"));
	}

	void Window::windowEnterFullsreenEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.fullscreen.enter"));
	}

	void Window::windowLeaveFullscreenEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.fullscreen.leave"));
	}

	void Window::windowDestroyedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.destroyed"));
	}

	void Window::windowPenEnterEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.pen.enter"));
	}

	void Window::windowPenLeaveEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("window.pen.leave"));
	}

	void Window::keyDownEvent(SDL_Event& e){
		const auto& keysym = e.key.keysym;
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("key.down"), SDL_ScancodeToKey(keysym.scancode), SDL_KeycodeToKey(keysym.sym), keysym.mod);
	}

	void Window::keyUpEvent(SDL_Event& e){
		const auto& keysym = e.key.keysym;
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("key.up"), SDL_ScancodeToKey(keysym.scancode), SDL_KeycodeToKey(keysym.sym), keysym.mod);
	}

	void Window::textEditingEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("text.editing"), e.edit.text, e.edit.start, e.edit.length);
	}

	void Window::textInputEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("text.input"), e.text.text);
	}

	void Window::keymapChangedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("keymap.changed"));
	}

	void Window::mouseMotionEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("mouse.motion"), Maths::vec2(e.motion.x, e.motion.y), Maths::vec2(e.motion.xrel, e.motion.yrel));
	}

	void Window::mouseButtonDownEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("mouse.button.down"), e.button.button, e.button.clicks, Maths::vec2(e.button.x, e.button.y));
	}

	void Window::mouseButtonUpEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("mouse.button.up"), e.button.button, Maths::vec2(e.button.x, e.button.y));
	}

	void Window::mouseWheelEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("mouse.wheel"), Maths::vec2(e.wheel.x, e.wheel.y), Maths::vec2(e.wheel.mouse_x, e.wheel.mouse_y));
	}

	void Window::joystickAxisMotionEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("joystick.axis.motion"), e.jaxis.axis, e.jaxis.value);
	}

	void Window::joystickHatMotionEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("joystick.hat.motion"), e.jhat.hat, e.jhat.value);
	}

	void Window::joystickButtonDownEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("joystick.button.down"), e.jbutton.button);
	}

	void Window::joystickButtonUpEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("joystick.button.up"), e.jbutton.button);
	}

	void Window::joystickAddedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("joystick.added"));
	}

	void Window::joystickRemovedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("joystick.removed"));
	}

	void Window::joystickBatteryUpdatedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("joystick.battery.updated"), e.jbattery.level);
	}

	void Window::joystickUpdateCompletedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("joystick.update.complete"));
	}

	void Window::gamepadAxisMotionEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("gamepad.axis.motion"), e.gaxis.axis, e.gaxis.value);
	}

	void Window::gamepadButtonDownEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("gamepad.button.down"), e.gbutton.button);
	}

	void Window::gamepadButtonUpEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("gamepad.button.up"), e.gbutton.button);
	}

	void Window::gamepadAddedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("gamepad.added"));
	}

	void Window::gamepadRemovedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("gamepad.removed"));
	}

	void Window::gamepadRemappedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("gamepad.remapped"));
	}

	void Window::gamepadTouchpadDownEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("gamepad.touchpad.down"), Maths::vec2(e.gtouchpad.x, e.gtouchpad.y), e.gtouchpad.pressure);
	}
	
	void Window::gamepadTouchpadMotionEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("gamepad.touchpad.motion"), Maths::vec2(e.gtouchpad.x, e.gtouchpad.y), e.gtouchpad.pressure);
	}

	void Window::gamepadTouchpadUpEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("gamepad.touchpad.up"), Maths::vec2(e.gtouchpad.x, e.gtouchpad.y), e.gtouchpad.pressure);
	}

	void Window::gamepadSensorUpdateEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("gamepad.sensor.update"), e.gsensor.sensor_timestamp, Maths::vec3(e.gsensor.data[0], e.gsensor.data[1], e.gsensor.data[2]));
	}

	void Window::gamepadUpdateCompleteEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("gamepad.update.complete"));
	}

	void Window::gamepadSteamHandleUpdatedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("gamepad.steam_handle_updated"));
	}

	void Window::fingerDownEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("finger.down"), e.tfinger.fingerID, Maths::vec2(e.tfinger.x, e.tfinger.y));
	}

	void Window::fingerUpEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("finger.up"), e.tfinger.fingerID, Maths::vec2(e.tfinger.x, e.tfinger.y));
	}

	void Window::fingerMotionEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("finger.motion"), e.tfinger.fingerID, Maths::vec2(e.tfinger.x, e.tfinger.y), Maths::vec2(e.tfinger.dx, e.tfinger.dy));
	}

	void Window::clipboardUpdateEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("clipboard.update"));
	}

	void Window::dropFileEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("drop.file"));
	}

	void Window::dropTextEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("drop.text"));
	}

	void Window::dropBeginEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("drop.begin"));
	}

	void Window::dropCompleteEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("drop.complete"));
	}

	void Window::dropPositionEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("drop.position"));
	}

	void Window::audioDeviceAddedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("audio_device.added"));
	}

	void Window::audioDeviceRemovedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("audio_device.removed"));
	}

	void Window::audioDeviceFormatChangedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("audio_device.format_changed"));
	}

	void Window::sensorUpdateEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("sensor.update"));
	}

	void Window::penDownEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("pen.down"));
	}

	void Window::penUpEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("pen.up"));
	}

	void Window::penMotionEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("pen.motion"));
	}

	void Window::penButtonDownEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("pen.button.down"));
	}

	void Window::penButtonUpEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("pen.button.up"));
	}

	void Window::cameraDeviceAddedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("camera_device.added"));
	}

	void Window::cameraDeviceRemovedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("camera_device.removed"));
	}

	void Window::cameraDeviceApprovedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("camera_device.approved"));
	}

	void Window::cameraDeviceDeniedEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("camera_device.denied"));
	}

	void Window::renderTargetsResetEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("render_targets.reset"));
	}

	void Window::renderDeviceResetEvent(SDL_Event& e){
		_context.getInternalContext().getEventManager().trigger(_context.getInternalContext().getEventManager().getEventID("render_device.reset"));
	}
}