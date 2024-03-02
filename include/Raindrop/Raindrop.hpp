#ifndef __RAINDROP_RAINDROP_HPP__
#define __RAINDROP_RAINDROP_HPP__

#include "common.hpp"

namespace Raindrop{
	class Raindrop{
		public:
			Raindrop();
			~Raindrop();

		private:
			Context* _context;
	};
}

#endif