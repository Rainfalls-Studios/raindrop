#ifndef __RAINDROP_CORE_REGISTRY_REGISTRY_HPP__
#define __RAINDROP_CORE_REGISTRY_REGISTRY_HPP__

#include <Raindrop/Core/Registry/common.hpp>
#include <Raindrop/Core/Registry/Node.hpp>

namespace Raindrop::Core::Registry{
	class Registry : public Node{
		public:
			Registry(Core::Context& core);
			~Registry();

		private:
			std::unique_ptr<Context> _context;
	};
}

#endif