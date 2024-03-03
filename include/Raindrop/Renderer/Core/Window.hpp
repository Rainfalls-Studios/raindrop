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
		
		private:
			Context& _context;
			SDL_Window* _window;
			VkSurfaceKHR _surface;

			void quitEvent();
	};
}

#endif