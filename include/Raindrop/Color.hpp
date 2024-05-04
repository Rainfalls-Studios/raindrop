#ifndef __RAINDROP_COLOR_HPP__
#define __RAINDROP_COLOR_HPP__

#include "common.hpp"

namespace Raindrop{
	class Color{
		public:
			enum class Component{
				RED,
				GREEN,
				BLUE,
				ALPHA
			};

			enum class Swizzle{
				RED,
				GREEN,
				BLUE,
				ALPHA,
				ONE,
				ZERO,
				IDENTITY
			};

		private:

	};
}

#endif