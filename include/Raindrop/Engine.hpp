#ifndef __RAINDROP_ENGINE_HPP__
#define __RAINDROP_ENGINE_HPP__

#include "Context.hpp"
#include "Graphics/Context.hpp"

namespace Raindrop{
	class Engine{
		public:
			Engine();
			~Engine();

			enum InitFlagBits{
				INIT_GRAPHICS = 1 << 0,

				INIT_EVERYTHING = INIT_GRAPHICS
			};

			using InitFlags = std::uint32_t;

			void initialize(InitFlags flags = 0);

			void release();

			// CORE

			Context& getContext();
			const Context& getContext() const;

			// GRAPHICS

			Graphics::Context& getGraphicsContext();
			const Graphics::Context& getGraphicsContext() const;

		private:
			std::unique_ptr<Context> _context;
			std::unique_ptr<Graphics::Context> _graphics;

			void initializeCore();
			void initializeGraphics();
	};
}

#endif