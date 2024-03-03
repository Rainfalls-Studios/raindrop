#ifndef __RAINDROP_CORE_TASKS_TASKS_HPP__
#define __RAINDROP_CORE_TASKS_TASKS_HPP__

#include "common.hpp"
#include <Tasks/Manager.hpp>

namespace Raindrop::Core::Tasks{
	using namespace ::Tasks;

	Manager& manager();
}

#endif