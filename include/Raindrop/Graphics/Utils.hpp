#ifndef __RAINDROP_GRAPHISC_UTILS_HPP__
#define __RAINDROP_GRAPHISC_UTILS_HPP__

#include "Utils/type_to_format.hpp"
#include <Raindrop/Utils/alignement.hpp>

#define ALIAS(name, source) template <typename... Args> auto name(Args&&... args) -> decltype(source(std::forward<Args>(args)...)) { return source(std::forward<Args>(args)...);}

namespace Raindrop::Graphics::Utils{
	ALIAS(alignTo, Raindrop::Utils::alignTo);
	ALIAS(calculatePadding, Raindrop::Utils::calculatePadding);
}

#endif