#include <Raindrop/Core/Tasks/Tasks.hpp>

namespace Raindrop::Core::Tasks{
	Manager& manager(){
		static Manager instance;
		return instance;
	}

}