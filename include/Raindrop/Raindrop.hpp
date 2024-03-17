#ifndef __RAINDROP_RAINDROP_HPP__
#define __RAINDROP_RAINDROP_HPP__

#include "common.hpp"

namespace Raindrop{
	class Raindrop{
		public:
			Raindrop();
			~Raindrop();

			void run();

		private:
			Context* _context;

			void render();
			void events();

			void updateCameraPosition();
	};
}

#endif