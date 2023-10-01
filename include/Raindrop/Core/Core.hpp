#ifndef __RAINDROP_CORE_CORE_HPP__
#define __RAINDROP_CORE_CORE_HPP__

#include <Raindrop/Core/common.hpp>

namespace Raindrop::Core{
	class Core{
		public:
			Core();
			~Core();

		private:
			std::unique_ptr<Context> _context;
	};
}

#endif