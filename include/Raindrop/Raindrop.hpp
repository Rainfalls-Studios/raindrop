#pragma once

#include "Core/Context.hpp"
#include "Graphics/Context.hpp"

namespace Raindrop{
	class Raindrop{
		public:
			Raindrop();
			~Raindrop();

			void run();

		private:
			struct Setup{
				Setup();
			};

			Setup _setup;
			Core::Context _core;
			Graphics::Context _graphics;
	};
}