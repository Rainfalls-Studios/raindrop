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
				GRAPHICS = 1 << 0
			};
			using InitFlags = std::uint32_t;

			void initialize(InitFlags flags);

			void release();

		protected:
			std::unique_ptr<Context> _context;
			std::unique_ptr<Graphics::Context> _graphics;

			void initializeCore();
			void initializeGraphics();
	};
}

#endif