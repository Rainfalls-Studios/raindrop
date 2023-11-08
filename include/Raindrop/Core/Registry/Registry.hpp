#ifndef __RAINDROP_CORE_REGISTRY_REGISTRY_HPP__
#define __RAINDROP_CORE_REGISTRY_REGISTRY_HPP__

#include <Raindrop/Core/Registry/common.hpp>
#include <boost/property_tree/ptree.hpp>

namespace Raindrop::Core::Registry{
	class Registry : public ::boost::property_tree::ptree{};
}

#endif