#ifndef __RAINDROP_RENDERER_CORE_WINDOW_HPP__
#define __RAINDROP_RENDERER_CORE_WINDOW_HPP__

#include <SDL3/SDL.h>
#include "common.hpp"

namespace Raindrop::Renderer::Core{
	class Window{
		public:
			Window(Context& context);
			virtual ~Window();
			
			void createSurface();
			void destroySurface();

			void setTitle(const char* title);
			void setSize(glm::u32vec2 size);
			void setPosition(glm::u32vec2 position);
			const char* getTitle() const;
			glm::u32vec2 getSize() const;
			glm::u32vec2 getPosition() const;
			bool loaded() const;
			SDL_Window* get() const;

			void events();

			VkSurfaceKHR surface();
			std::vector<const char*> vulkanExtensions();

			bool resized() const;
			void resetResizedFlags();
		
		private:
			Context& _context;
			SDL_Window* _window;
			VkSurfaceKHR _surface;

			bool _resized;

			void quitEvent(SDL_Event& e);

			void terminatingEvent(SDL_Event& e);
			void lowMemoryEvent(SDL_Event& e);

			void willEnterBackgroundEvent(SDL_Event& e);
			void didEnterBackgroundEvent(SDL_Event& e);
			void willEnterForgroundEvent(SDL_Event& e);
			void didEnterForgourndEvent(SDL_Event& e);

			void localeChangedEvent(SDL_Event& e);
			void systemThemChangedEvent(SDL_Event& e);

			void displayOrientationEvent(SDL_Event& e);
			void displayAddedEvent(SDL_Event& e);
			void displayRemovedEvent(SDL_Event& e);
			void displayMovedEvent(SDL_Event& e);
			void displayContentScaleChangedEvent(SDL_Event& e);
			void displayHDRStateChnagedEvent(SDL_Event& e);

			void windowShownEvent(SDL_Event& e);
			void windowHiddenEvent(SDL_Event& e);
			void windowExposedEvent(SDL_Event& e);
			void windowMovedEvent(SDL_Event& e);
			void windowResizedEvent(SDL_Event& e);
			void windowPixelSizeChangedEvent(SDL_Event& e);
			void windowMinimizedEvent(SDL_Event& e);
			void windowMaximizedEvent(SDL_Event& e);
			void windowRestoredEvent(SDL_Event& e);
			void windowMouseEnterEvent(SDL_Event& e);
			void windowMouseLeaveEvent(SDL_Event& e);
			void windowFocusGainedEvent(SDL_Event& e);
			void windowFocusLostEvent(SDL_Event& e);
			void windowCloseRequestedEvent(SDL_Event& e);
			void windowTakeFocusEvent(SDL_Event& e);
			void windowHitTestEvent(SDL_Event& e);
			void windowICCPROFChangedEvent(SDL_Event& e);
			void windowDisplayChangedEvent(SDL_Event& e);
			void windowDisplayScaleChangedEvent(SDL_Event& e);
			void windowOccludedEvent(SDL_Event& e);
			void windowEnterFullsreenEvent(SDL_Event& e);
			void windowLeaveFullscreenEvent(SDL_Event& e);
			void windowDestroyedEvent(SDL_Event& e);
			void windowPenEnterEvent(SDL_Event& e);
			void windowPenLeaveEvent(SDL_Event& e);

			void keyDownEvent(SDL_Event& e);
			void keyUpEvent(SDL_Event& e);
			void textEditingEvent(SDL_Event& e);
			void textInputEvent(SDL_Event& e);
			void keymapChangedEvent(SDL_Event& e);

			void mouseMotionEvent(SDL_Event& e);
			void mouseButtonDownEvent(SDL_Event& e);
			void mouseButtonUpEvent(SDL_Event& e);
			void mouseWheelEvent(SDL_Event& e);

			void joystickAxisMotionEvent(SDL_Event& e);
			void joystickHatMotionEvent(SDL_Event& e);
			void joystickButtonDownEvent(SDL_Event& e);
			void joystickButtonUpEvent(SDL_Event& e);
			void joystickAddedEvent(SDL_Event& e);
			void joystickRemovedEvent(SDL_Event& e);
			void joystickBatteryUpdatedEvent(SDL_Event& e);
			void joystickUpdateCompletedEvent(SDL_Event& e);

			void gamepadAxisMotionEvent(SDL_Event& e);
			void gamepadButtonDownEvent(SDL_Event& e);
			void gamepadButtonUpEvent(SDL_Event& e);
			void gamepadAddedEvent(SDL_Event& e);
			void gamepadRemovedEvent(SDL_Event& e);
			void gamepadRemappedEvent(SDL_Event& e);
			void gamepadTouchpadDownEvent(SDL_Event& e);
			void gamepadTouchpadMotionEvent(SDL_Event& e);
			void gamepadTouchpadUpEvent(SDL_Event& e);
			void gamepadSensorUpdateEvent(SDL_Event& e);
			void gamepadUpdateCompleteEvent(SDL_Event& e);
			void gamepadSteamHandleUpdatedEvent(SDL_Event& e);

			void fingerDownEvent(SDL_Event& e);
			void fingerUpEvent(SDL_Event& e);
			void fingerMotionEvent(SDL_Event& e);

			void clipboardUpdateEvent(SDL_Event& e);

			void dropFileEvent(SDL_Event& e);
			void dropTextEvent(SDL_Event& e);
			void dropBeginEvent(SDL_Event& e);
			void dropCompleteEvent(SDL_Event& e);
			void dropPositionEvent(SDL_Event& e);

			void audioDeviceAddedEvent(SDL_Event& e);
			void audioDeviceRemovedEvent(SDL_Event& e);
			void audioDeviceFormatChangedEvent(SDL_Event& e);

			void sensorUpdateEvent(SDL_Event& e);

			void penDownEvent(SDL_Event& e);
			void penUpEvent(SDL_Event& e);
			void penMotionEvent(SDL_Event& e);
			void penButtonDownEvent(SDL_Event& e);
			void penButtonUpEvent(SDL_Event& e);

			void cameraDeviceAddedEvent(SDL_Event& e);
			void cameraDeviceRemovedEvent(SDL_Event& e);
			void cameraDeviceApprovedEvent(SDL_Event& e);
			void cameraDeviceDeniedEvent(SDL_Event& e);

			void renderTargetsResetEvent(SDL_Event& e);
			void renderDeviceResetEvent(SDL_Event& e);

			void registerEvents();

	};
}

#endif