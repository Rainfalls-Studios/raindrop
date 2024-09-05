#pragma once

#include "Raindrop/pch.pch"

namespace Raindrop::Graphics{
	class Context;

	class Window{
		public:
			Window(Context& context, const char* title, std::uint32_t width, std::uint32_t height);
			~Window();

			Context& getContext() noexcept;
			const Context& getContext() const noexcept;

			SDL_Window* get() const noexcept;

		private:
			#if 0
				struct SDL_Window;
			#endif

			Context& _context;
			SDL_Window* _window;
	};
}